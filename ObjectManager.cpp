#include "ObjectManager.h"
#include "Object.h"
#include <algorithm>
#include "Component.h"
#include "Game.h"
#include "GameEntity.h"
#include "Math.h"

void ObjectManager::UpdateAllObjects(float DeltaTime)
{
	// update Entities, Components & Drawables (separately bc drawables not [yet] integrated as objects)
	for (auto& Entity : Entities)
	{
		Entity->Update(DeltaTime);
		WrapEntity(Entity.get());

		auto comps = Entity->GetComponentList();
		for (auto &compItem : *comps)
		{
			auto comp = compItem.lock();
			if (comp->IsPendingKill())
			{
				// GC will take care of me
				continue;
			}
			comp->Update(DeltaTime);
		}
	}
}

void ObjectManager::RenderAllObjects(sf::RenderWindow* Window) const
{
	// draw entities
	for (const auto& Entity : Entities)
	{
		Entity->Render(Window);
		for (auto& Drawer : Entity->Drawables)
		{
			auto entityTransformed = sf::Transform::Identity * Entity->getTransform();
			if (Drawer.get())
			{
				sf::RenderStates states;
				states.transform = entityTransformed;
				if (auto Transformer = std::dynamic_pointer_cast<sf::Transformable>(Drawer))
				{
					states.transform *= Transformer->getTransform();
				}
				Window->draw(*Drawer, states);
			}
		}
	}
}

std::weak_ptr<GameEntity> ObjectManager::GetWeakPtr(GameEntity* RawPtr)
{
	if (!RawPtr)
	{
		return {};
	}
	auto Obj = static_cast<Object*>(RawPtr);

	return std::static_pointer_cast<GameEntity>(GetObjectByIndex(Obj->GetObjectIndex()).lock());
}

void ObjectManager::AddToObjectList(std::shared_ptr<Object> NewObject)
{
	//add to vector
	Objects.push_back(NewObject);
	// assign index and numElements++
	NewObject->SetObjectIndex(static_cast<int>(Objects.size()) - 1);
}

void ObjectManager::SetObjectIndex(const std::shared_ptr<Object>& Shared, int Index)
{
	Shared->SetObjectIndex(Index);
}

void ObjectManager::RemoveObjectAtIndex(int Index)
{
	if (Index >= Objects.size() || Index < 0)
	{
		return;
	}
	
	//pointer at index releases ownership, so if it is the last shared ptr owning it, obj is deleted
	Objects[Index].reset();
	
	// DEFRAGMENTATION of vector by filling in gaps
	if (Index == Objects.size() - 1)
	{
		// there is no need to fill any gap if the object is at the end of the list
		// just make sure that element is not accessed anymore
		Objects.pop_back();
		return;
	}

	// move pointer to last object stored in list to the new emptied index
	std::iter_swap((Objects.begin() + Index), (Objects.begin() + Objects.size() - 1));
	Objects.pop_back();

	std::shared_ptr<Object> object = Objects.at(Index);
	if(object != nullptr)
	{
		// assign new index to object
		SetObjectIndex(object, Index);
	}
}

std::weak_ptr<Object> ObjectManager::GetObjectByIndex(int Index)
{
	return Objects[Index];
}

void ObjectManager::WrapEntity(GameEntity* Entity) const
{
	auto EntityPosition = Entity->getPosition();
	if (EntityPosition.x > WindowWidth || EntityPosition.x < 0 ||
		EntityPosition.y > WindowHeight || EntityPosition.y < 0)
	{
		EntityPosition.x = Math::WrapModulo(EntityPosition.x, 0, WindowWidth);
		EntityPosition.y = Math::WrapModulo(EntityPosition.y, 0, WindowHeight);
		Entity->setPosition(EntityPosition);
	}
}

bool ObjectManager::CanCreateObject() const { return Entities.size() < MaxEntities; }

void ObjectManager::SetWindowVals(float X, float Y)
{
	WindowWidth = X;
	WindowHeight = Y;
}