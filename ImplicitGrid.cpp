#include "ImplicitGrid.h"
#include "System.h"
#include "Collider.h"
#include "GameEntity.h"
#include <memory>
#include "Debug.h"
#include <algorithm>

#include "Math.h"

ImplicitGrid::~ImplicitGrid()
{
	System::GetInstance()->Event_EntityCreated.Unsubscribe(eventHandle_EntityCreated_);
	System::GetInstance()->Event_EntityDestroyed.Unsubscribe(eventHandle_EntityDestroyed_);
	System::GetInstance()->Event_LevelEnd.Unsubscribe(eventHandle_LevelEnd_);
}

void ImplicitGrid::InitGrid(int Width, int Height)
{
	gridWidth_ = Width;
	gridHeight_ = Height;
	renderColor_ = sf::Color(116, 218, 158, 100);
	RefreshGrid();

	InitDebugText();
	UpdateCollisionPairCount();
	
	//default collection to maintain first 32 entities
	Add32ObjectCollection();

	eventHandle_EntityCreated_ = System::GetInstance()->
		Event_EntityCreated.Subscribe(BindSubscriber(&ImplicitGrid::OnEntityCreated, this));

	eventHandle_EntityDestroyed_ = System::GetInstance()->
		Event_EntityDestroyed.Subscribe(BindSubscriber(&ImplicitGrid::OnEntityDestroyed, this));

	eventHandle_LevelEnd_ = System::GetInstance()->
		Event_LevelEnd.Subscribe(BindSubscriber(&ImplicitGrid::CleanUpOldObjects, this));
}

void ImplicitGrid::InitDebugText()
{
	if (!font_.loadFromFile("RecursiveSansLnrSt-Med.ttf"))
	{
		// error...
	}

	collisionPairsText_ = CreateDrawable<sf::Text>();
	if (auto debugText = collisionPairsText_.lock())
	{
		debugText->setCharacterSize(8);
		debugText->setStyle(sf::Text::Regular);
		debugText->setFont(font_);
	}
}

void ImplicitGrid::Update(const float deltaTime)
{
	collisionPairsCount_ = 0;

	CheckDebugInput();

	UpdatePositions();
	for (int i = 0; i < entities_.size(); i++)
	{
		TestEntityAgainstGrid(i);
	}
	if (bCollisionPairsVisibility_)
	{
		UpdateCollisionPairCount();
	}
}

void ImplicitGrid::TestEntityAgainstGrid(int index)
{
	std::shared_ptr<GameEntity> entity = entities_[index].lock();
	if (!entity ||
		entity->IsPendingKill() ||
		!entity->HasColliders())
	{
		return;
	}

	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	GetEntityBounds(entity, x1, y1, x2, y2);

	// Allocate temporary bit arrays for all objects and clear them
	std::vector<int32_t> mergedRowBits(numOfObjectCollections_, 0);
	std::vector<int32_t> mergedColumnBits(numOfObjectCollections_, 0);

	// Compute the merged (bitwise-or’ed) bit array of all overlapped grid rows.
	// Ditto for all overlapped grid columns

	// get all the objects present in the area where candidateEntity's calculated(^) presence is
	for (int i = 0; i < numOfObjectCollections_; i++)
	{
		for (int y = y1; y <= y2; y++)
		{
			mergedRowBits[i] |= globalRowCollections_[i][y];
			//Debug::Print("row mask " + Debug::GetNumberAsBitset(mergedRowBits[i]) + ", added " + Debug::GetNumberAsBitset(globalRowCollections_[i][y]));
		}
		for (int x = x1; x <= x2; x++)
		{
			mergedColumnBits[i] |= globalColumnCollections_[i][x];
			//Debug::Print("col mask " + Debug::GetNumberAsBitset(mergedColumnBits[i]) + ", added " + Debug::GetNumberAsBitset(globalColumnCollections_[i][x]));
		}
	}

	// Now go through the intersection of the merged bit arrays and collision test
	// those objects having their corresponding bit set
	for (int i = 0; i < numOfObjectCollections_; i++)
	{
		//find out current cell objects by ANDing cells of same x & y (got per x cell data in prev loop)
		//in all groups of 31/collectionSize_ objects (current loop)
		//for all objects in groups (inner loop)
		int32_t objectsMask = mergedRowBits[i] & mergedColumnBits[i];
		//Debug::Print("mega mask  = " + Debug::GetNumberAsBitset(objectsMask));
		while (objectsMask)
		{
			// Clear all but least signficant bit ( 011001 -> 000001 )
			int objectMask = objectsMask & -objectsMask;
			//Debug::Print("new mask " + Debug::GetNumberAsBitset(objectMask));
			// Get index number of set bit, test against corresponding object
			// (GetBitIndex(v) returns log_2(v), i.e. n such that 2∧n = v)
			int32_t objectIndex = GetBitIndex(objectMask) + (i * collectionSize_);
			/*
			Debug::Print("my mask " + std::to_string(objectIndex) + " = "
						   + Debug::GetNumberAsBitset(1 << objectIndex) + ", collection = "
						   + std::to_string(i));
			*/
			// if not testing candidate with self
			if (index != objectIndex)
			{
				auto collidingEntity = GetEntityAtIndex(objectIndex);
				if (auto other = collidingEntity.lock())
				{
					if (other->HasColliders())
					{
						TestEntityPairForCollision(entity, other);
						// exit if candidate entity (entities_[index]) is now marked pending kill
						if (entity->IsPendingKill())
						{
							return;
						}
					}
				}
			}

			// Mask out tested object, and continue with any remaining objects
			objectsMask ^= objectMask;
		}
	}
}

void ImplicitGrid::TestEntityPairForCollision(const std::shared_ptr<GameEntity>& a, const std::shared_ptr<GameEntity>& b)
{
	collisionPairsCount_++;
	auto collidersA = std::vector<std::weak_ptr<Collider>>();
	auto collidersB = std::vector<std::weak_ptr<Collider>>();

	a->GetComponentsOfType<Collider>(collidersA);
	b->GetComponentsOfType<Collider>(collidersB);

	for (const auto& itA : collidersA)
	{
		// dereferenced weak_ptr to shared_ptr to make a reference to component
		Collider& colliderA = *(itA.lock());

		for (const auto& itB : collidersB)
		{
			Collider& colliderB = *(itB.lock());
			if (!colliderB.IsValidCollider() || !colliderA.IsValidCollider())
			{
				continue;
			}
			if (AreColliding(colliderA, colliderB))
			{
				a->OnCollision(std::weak_ptr<GameEntity>(b));
				b->OnCollision(std::weak_ptr<GameEntity>(a));
			}
		}
	}
}

void ImplicitGrid::UpdatePositions()
{
	//implement isPositionDirty
	ClearGrids();
	int currentCollection = -1;
	for (size_t i = 0; i < entities_.size(); ++i)
	{
		if (i % collectionSize_ == 0)
		{
			currentCollection++;
		}
		if (auto entity = entities_[i].lock())
		{
			if (entity->IsPendingKill())
			{
				continue;
			}
			int entityCollectionId = 1 << (i % collectionSize_);
			//Debug::Print(std::string("entity ") + std::to_string(i) + std::string(", ") + Debug::GetNumberAsBitset(entityCollectionId));
			int x1, y1, x2, y2;
			GetEntityBounds(entity, x1, y1, x2, y2);
			for (int y = y1; y <= y2; y++)
			{
				globalRowCollections_[currentCollection][y] |= entityCollectionId;
			}
			for (int x = x1; x <= x2; x++)
			{
				globalColumnCollections_[currentCollection][x] |= entityCollectionId;
			}
		}
	}
}

void ImplicitGrid::ClearGrids()
{
	for (auto& collection : globalRowCollections_)
	{
		for (unsigned int& cell : collection)
		{
			//mark that no object from index (n * 32) + 0 to (n+1) * 32 is present in this entire row
			cell = 0;
		}
	}
	for (auto& collection : globalColumnCollections_)
	{
		for (unsigned int& cell : collection)
		{
			//mark that no object from index (n * 32) + 0 to (n+1) * 32 is present in this entire column
			cell = 0;
		}
	}
}


void ImplicitGrid::OnEntityCreated(std::weak_ptr<GameEntity> entity)
{
	if (std::dynamic_pointer_cast<ImplicitGrid>(entity.lock()))
	{
		// dont add self
		return;
	}
	entities_.push_back(entity);
	
	// manage maintaining div 32 collections
	int entityId = (int)entities_.size() - 1;

	if (entityId > 0 && entityId % collectionSize_ == 0)
	{
		//we have filled up all the positions in this 32 object collection
		Add32ObjectCollection();
	}
}

void ImplicitGrid::OnEntityDestroyed(std::weak_ptr<GameEntity> entity)
{
	int entityId = (int)entities_.size() - 1;
	entities_.erase(std::find_if(entities_.begin(),
								 entities_.end(),
								 [&entity](const std::weak_ptr<GameEntity>& candidate) -> bool
								 {
									 return candidate.lock() == entity.lock();
								 }));

	// manage maintaining div 32 collections
	if (entityId % collectionSize_ == 0)
	{
		//we have emptied all the positions in this 32 object collection
		Remove32ObjectCollection();
	}
}

void ImplicitGrid::CleanUpOldObjects()
{
	entities_.clear();
	while (numOfObjectCollections_ > 1)
	{
		Remove32ObjectCollection();
	}
}

void ImplicitGrid::Add32ObjectCollection()
{
	//create another collection
	numOfObjectCollections_ = numOfObjectCollections_ + 1;
	globalColumnCollections_.emplace_back();
	globalRowCollections_.emplace_back();

	//create said grid out of new gridWidth columns and gridHeight rows 
	for (int i = 0; i < gridCellCountX_; i++)
	{
		globalColumnCollections_[numOfObjectCollections_ - 1].emplace_back();
	}
	for (int i = 0; i < gridCellCountY_; i++)
	{
		globalRowCollections_[numOfObjectCollections_ - 1].emplace_back();
	}
}

void ImplicitGrid::Remove32ObjectCollection()
{
	globalRowCollections_.pop_back();
	globalColumnCollections_.pop_back();
	numOfObjectCollections_ = numOfObjectCollections_ - 1;
}

void ImplicitGrid::GenerateDebugGrid()
{
	if (auto grid = debugGrid_.lock())
	{
		grid->clear();

		float gridWidthF = static_cast<float>(gridWidth_);
		float gridHeightF = static_cast<float>(gridHeight_);
		auto intervalX = gridWidthF / static_cast<float>(GetGridCellSizeX());
		auto intervalY = gridHeightF / static_cast<float>(GetGridCellSizeY());
	
		for (int i = 1; i < gridCellCountX_; ++i)
		{
			grid->append(sf::Vertex(sf::Vector2f(intervalX * i, 0.f), renderColor_));
			grid->append(sf::Vertex(sf::Vector2f(intervalX * i, gridHeightF), renderColor_));
		}
		for (int i = 1; i < gridCellCountY_; ++i)
		{
			grid->append(sf::Vertex(sf::Vector2f(0.f, intervalY * i), renderColor_));
			grid->append(sf::Vertex(sf::Vector2f(gridWidthF, intervalY * i), renderColor_));
		}
	}
}

void ImplicitGrid::RefreshGrid()
{
	gridCellCountX_ = gridWidth_ / GetGridCellSizeX();
	gridCellCountY_ = gridHeight_ / GetGridCellSizeY();

	auto rebalanceGridSize = [](std::vector<std::vector<uint32_t>>& collections, const int newSize) -> void
	{
		for (auto& collection : collections)
		{
			int oldSize = (int)collection.size();
			if (oldSize > newSize)
			{
				int diff = oldSize - newSize;
				while (diff-- > 0)
				{
					collection.pop_back();
				}
			}
			else if (oldSize < newSize)
			{
				int diff = newSize - oldSize;
				{
					while (diff-- > 0)
					{
						collection.emplace_back();
					}
				}
			}
		}
	};

	rebalanceGridSize(globalColumnCollections_, gridCellCountX_);
	rebalanceGridSize(globalRowCollections_, gridCellCountY_);

	if (debugGrid_.expired())
		debugGrid_ = CreateDrawable<sf::VertexArray>(sf::Lines, (gridCellCountX_ + gridCellCountY_) * 2);

	GenerateDebugGrid();
}

void ImplicitGrid::CheckDebugInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8))
	{
		SetGridCellSizeY(GetGridCellSizeY() + gridSizeChanger_);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
	{
		SetGridCellSizeY(GetGridCellSizeY() - gridSizeChanger_);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6))
	{
		SetGridCellSizeX(GetGridCellSizeX() + gridSizeChanger_);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
	{
		SetGridCellSizeX(GetGridCellSizeX() - gridSizeChanger_);
	}

	// debug Grid visibility
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period))
	{
		SetDebugGridVisibility(!bDebugGridVisibility_);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0))
	{
		SetDebugTextVisibility(!bCollisionPairsVisibility_);
	}
}

std::vector<std::weak_ptr<GameEntity>> ImplicitGrid::GetGridCellOccupiers(int x, int y)
{
	std::vector<std::weak_ptr<GameEntity>> vec;
	GetGridCellOccupiers(x, y, vec);
	return vec;
}

void ImplicitGrid::GetGridCellOccupiers(int x, int y, std::vector<std::weak_ptr<GameEntity>>& inVector)
{
	// Allocate temporary bit arrays for all objects and clear them
	std::vector<int32_t> mergedRowBits(numOfObjectCollections_, 0);
	std::vector<int32_t> mergedColumnBits(numOfObjectCollections_, 0);

	// get all the objects present in the area where candidateEntity's calculated(^) presence is
	for (int i = 0; i < numOfObjectCollections_; i++)
	{
		mergedRowBits[i] |= globalRowCollections_[i][y];
		mergedColumnBits[i] |= globalColumnCollections_[i][x];
	}
	// Now go through the intersection of the merged bit arrays and collision test
	// those objects having their corresponding bit set
	for (int i = 0; i < numOfObjectCollections_; i++)
	{
		//find out current cell objects by ANDing cells of same x & y (got per x cell data in prev loop)
		//in all groups of 31/collectionSize_ objects (current loop)
		//for all objects in groups (inner loop)
		int32_t objectsMask = mergedRowBits[i] & mergedColumnBits[i];
		//Debug::Print("mega mask  = " + Debug::GetNumberAsBitset(objectsMask));
		while (objectsMask)
		{
			// Clear all but least significant bit ( 011001 -> 000001 )
			int objectMask = objectsMask & -objectsMask;
			//Debug::Print("new mask " + Debug::GetNumberAsBitset(objectMask));
			// Get index number of set bit, test against corresponding object
			// (GetBitIndex(v) returns log_2(v), i.e. n such that 2∧n = v)
			int32_t objectIndex = GetBitIndex(objectMask) + (i * collectionSize_);
			/*Debug::Print("my mask " + std::to_string(objectIndex) + " = "
						   + Debug::GetNumberAsBitset(1 << objectIndex) + ", collection = "
						   + std::to_string(i));*/

			auto collidingEntity = GetEntityAtIndex(objectIndex);
			if (auto other = collidingEntity.lock())
			{
				if (other->HasColliders())
				{
					inVector.push_back(collidingEntity);
				}
			}

			// Mask out tested object, and continue with any remaining objects
			objectsMask ^= objectMask;
		}
	}
}

void ImplicitGrid::GetEntityBounds(const std::shared_ptr<GameEntity>& entity, int& x1, int& y1, int& x2, int& y2) const
{
	// Compute the extent of grid cells the bounding sphere of A overlaps.
	// first calculate candidateEntity's presence in grid cells
	auto collider = entity->GetComponentOfType<Collider>();
	sf::Vector2f position;
	float radius = 0.0f;
	if (auto sharedCollider = collider.lock())
	{
		radius = sharedCollider->GetRadius();
		position = sharedCollider->GetWorldPosition();
	}
	GetRadiusBounds(position.x, position.y, radius, x1, y1, x2, y2);
}

void ImplicitGrid::GetRadiusBounds(const float posX, const float posY, const float radius,
								   int& x1, int& y1, int& x2, int& y2) const
{
	float cellWidth = 1.0f / GetGridCellSizeX();
	int adjustedX = (int)(posX + gridWidth_ / static_cast<float>(2));
	int adjustedY = (int)(posY + gridHeight_ / static_cast<float>(2));

	// purposely used c style casts based on functionality
	x1 = (int)floorf((adjustedX - radius) * cellWidth);
	x2 = (int)floorf((adjustedX + radius) * cellWidth);
	y1 = (int)floorf((adjustedY - radius) * cellWidth);
	y2 = (int)floorf((adjustedY + radius) * cellWidth);

	x1 = Math::ClampValue(x1, 0, gridCellCountX_ - 1);
	x2 = Math::ClampValue(x2, 0, gridCellCountX_ - 1);
	y1 = Math::ClampValue(y1, 0, gridCellCountY_ - 1);
	y2 = Math::ClampValue(y2, 0, gridCellCountY_ - 1);
}

bool ImplicitGrid::AreColliding(const Collider& a, const Collider& b)
{
	return FVectorDistance(a.GetWorldPosition(), b.GetWorldPosition()) < (a.GetRadius() + b.GetRadius());
}

std::weak_ptr<GameEntity> ImplicitGrid::GetEntityAtIndex(int index)
{
	if (entities_.empty() || index >= entities_.size() || index < 0)
	{
		return {};
	}
	return entities_.at(index);
}

int ImplicitGrid::GetBitIndex(const int32_t& mask)
{
	return (int)log2f((float)mask);
}

void ImplicitGrid::SetDebugGridVisibility(bool val)
{
	if (val)
		GenerateDebugGrid();
	else if (auto grid = debugGrid_.lock())
		grid->clear();	
}

void ImplicitGrid::SetDebugTextVisibility(bool val)
{
	if (val)
		UpdateCollisionPairCount();
	else if (auto text = collisionPairsText_.lock())
		text->setString("");
}

/// <summary>
/// Removes object from the index and fills the gap by bringing an object from the end of the container
/// </summary>
void ImplicitGrid::RemoveObjectAtIndex(int32_t index)
{
	if (index >= entities_.size() || index < 0)
	{
		return;
	}

	//pointer at index releases ownership, so if it is the last shared ptr owning it, obj is deleted
	entities_[index].reset();

	// DEFRAGMENTATION of vector by filling in gaps
	if (index == entities_.size() - 1)
	{
		// there is no need to fill any gap if the object is at the end of the list
		// just make sure that element is not accessed anymore
		entities_.pop_back();
		return;
	}

	// move pointer to last object stored in list to the new emptied index
	std::iter_swap((entities_.begin() + index), (entities_.begin() + entities_.size() - 1));
	entities_.pop_back();
}

void ImplicitGrid::UpdateCollisionPairCount()
{
	if (auto text = collisionPairsText_.lock())
		text->setString("CollisionPairs: " + std::to_string(collisionPairsCount_));
}

float ImplicitGrid::FVectorDistance(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return sqrtf(powf((a.x - b.x), 2) + powf((a.y - b.y), 2));
}
