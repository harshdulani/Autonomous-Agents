#include "Physics.h"
#include <bitset>
#include "System.h"
#include "Game.h"
#include "ImplicitGrid.h"
#include "Debug.h"
#include "Math.h"

std::weak_ptr<GameEntity> Physics::Raycast(const sf::Vector2f& start, const sf::Vector2f& end, const GameEntity* self)
{
	auto grid = System::GetInstance()->GetGame().GetCollisionSystem().lock();
	if (!grid)
	{
		Debug::Print("Collision system not found");
		return {};
	}
	auto result = std::weak_ptr<GameEntity>();

	float startX = start.x;
	float startY = start.y;
	float endX = end.x;
	float endY = end.y;

	int i = static_cast<int>(ceilf(Math::GetVectorMagnitude(start - end)));
	int j = 0;
	std::vector<sf::Vector2f> traversed;
	while (j++ < i)
	{
		float x = Math::LerpClamped(startX, endX, (float)j / (float)i);
		float y = Math::LerpClamped(startY, endY, (float)j / (float)i);

		int cellX = Math::InverseLerpUnclamped(0, grid->gridWidth_, x);
		int cellY = Math::InverseLerpUnclamped(0, grid->gridHeight_, y);

		cellX *= grid->gridCellCountX_;
		cellY *= grid->gridCellCountY_;

		if (cellX < 0 || cellX >= grid->gridCellCountX_ ||
			cellY < 0 || cellY >= grid->gridCellCountY_)
		{
			//crossed bounds searching
			break;
		}
		sf::Vector2f nu = { static_cast<float>(cellX), static_cast<float>(cellY) };

		if (static_cast<int>(traversed.size()) > 0)
		{
			auto& item = traversed[traversed.size() - 1];
			if (Math::IsAlmostEqualTo(nu, item))
			{
				continue;
			}
		}

		traversed.push_back(nu);

		auto vec = grid->GetGridCellOccupiers((int)cellX, (int)cellY);
		for (auto& res : vec)
		{
			if (!res.expired() && res.lock().get() != self)
			{
				result = res;
				// found hit
				break;
			}
		}
	}

	return result;
}

void Physics::RaycastAll(const sf::Vector2f& start, const sf::Vector2f& end, const GameEntity* self, std::vector<std::weak_ptr<GameEntity>>& inVector)
{
	auto grid = System::GetInstance()->GetGame().GetCollisionSystem().lock();
	if (!grid)
	{
		Debug::Print("Collision system not found");
		return;
	}
	auto result = std::weak_ptr<GameEntity>();

	float startX = start.x;
	float startY = start.y;
	float endX = end.x;
	float endY = end.y;

	int i = static_cast<int>(ceilf(Math::GetVectorMagnitude(start - end)));
	int j = 0;
	std::vector<sf::Vector2f> traversed;
	while (j++ < i)
	{
		float x = Math::LerpClamped(startX, endX, (float)j / (float)i);
		float y = Math::LerpClamped(startY, endY, (float)j / (float)i);

		float cellX = Math::InverseLerpUnclamped(0.f, static_cast<float>(grid->gridWidth_), x);
		float cellY = Math::InverseLerpUnclamped(0.f, static_cast<float>(grid->gridHeight_), y);

		cellX *= grid->gridCellCountX_;
		cellY *= grid->gridCellCountY_;

		if (cellX < 0 || cellX >= grid->gridCellCountX_ ||
			cellY < 0 || cellY >= grid->gridCellCountY_)
		{
			//crossed bounds searching
			break;
		}
		sf::Vector2f nu = { static_cast<float>((int) cellX), static_cast<float>((int) cellY) };

		if ((int)traversed.size() > 0)
		{
			auto& item = traversed[traversed.size() - 1];
			if (Math::IsAlmostEqualTo(nu, item))
			{
				continue;
			}
		}

		traversed.push_back(nu);

		auto vec = grid->GetGridCellOccupiers((int)cellX, (int)cellY);
		for (auto& res : vec)
		{
			if (res.expired())
			{
				continue;
			}
			auto other = res.lock();
			if (other.get() != self)
			{
				// found hit
				auto it = std::find_if(inVector.begin(),
									   inVector.end(),
									   [&other](const std::weak_ptr<GameEntity> item) -> bool
									   {
										   return item.lock() == other;
									   });
				if (it == inVector.end())
				{
					inVector.push_back(res);
				}
			}
		}
	}
}

void Physics::CircleCast(const sf::Vector2f& position, float radius, std::vector<std::weak_ptr<GameEntity>>& inVector)
{
	auto grid = System::GetInstance()->GetGame().GetCollisionSystem().lock();
	if (!grid)
	{
		Debug::Print("Collision system not found");
		return;
	}
	auto result = std::weak_ptr<GameEntity>();

	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	grid->GetRadiusBounds(position.x, position.y, radius,
					x1, y1, x2, y2);

	int numOfObjectCollections = grid->numOfObjectCollections_;

	//for explanations go to implicit grid test entity against grid
	std::vector<int32_t> mergedRowBits(numOfObjectCollections, 0);
	std::vector<int32_t> mergedColumnBits(numOfObjectCollections, 0);

	for (int i = 0; i < numOfObjectCollections; i++)
	{
		for (int y = y1; y <= y2; y++)
		{
			mergedRowBits[i] |= grid->globalRowCollections_[i][y];
		}
		for (int x = x1; x <= x2; x++)
		{
			mergedColumnBits[i] |= grid->globalColumnCollections_[i][x];
		}
	}
	
	for (int i = 0; i < numOfObjectCollections; i++)
	{
		int32_t objectsMask = mergedRowBits[i] & mergedColumnBits[i];
		while (objectsMask)
		{
			int objectMask = objectsMask & -objectsMask;
			int32_t objectIndex = grid->GetBitIndex(objectMask) + (i * grid->collectionSize_);
			auto collidingEntity = grid->GetEntityAtIndex(objectIndex);
			if (auto other = collidingEntity.lock())
			{
				if (other->HasColliders() && !other->IsPendingKill())
				{
					auto it = std::find_if(inVector.begin(),
										   inVector.end(),
										   [&other](const std::weak_ptr<GameEntity> item) -> bool
										   {
											   return item.lock() == other;
										   });
					if (it == inVector.end())
					{
						if (Math::GetVectorMagnitude(other->GetPosition() - position)
							< radius)
						{
							inVector.push_back(other);
						}
					}
				}
			}
			// Mask out tested object, and continue with any remaining objects
			objectsMask ^= objectMask;
		}
	}
}
