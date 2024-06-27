﻿#include "ObjectManager.h"
#include "Object.h"
#include <algorithm>
#include "Component.h"
#include "Game.h"
#include "GameEntity.h"
#include "Math.h"
#include "PrimitiveComponent.h"
#include "Event.h"
#include "SceneComponent.h"

ObjectManager::ObjectManager()
{
	Objects.reserve(MaxObjects);
	
	eventHandle_levelEnd_ = System::GetInstance()->Event_LevelEnd.Subscribe(BindSubscriber(&ObjectManager::CleanUpOldObjects, this)); 
}

ObjectManager::~ObjectManager()
{
	Objects.clear();

	System::GetInstance()->Event_LevelEnd.Unsubscribe(eventHandle_levelEnd_);
}

void ObjectManager::UpdateAllObjects(float DeltaTime)
{
	ResolveUpdateDirty();
	// update Entities, Components & Drawables (separately bc drawables not [yet] integrated as objects)
	for (auto& Item : UpdateEntities)
	{
		auto Entity = Item.lock();
		if (Entity == nullptr || Entity->IsPendingKill() || !Entity->GetActive())
		{
			//if waiting to die or is inactive, we don't need to do anything
			continue;
		}
		bool isPositionDirty = Entity->GetAndClearDirtyPosition();
		bool isRotationDirty = Entity->GetAndClearDirtyAngle();

		Entity->Update(DeltaTime);
		WrapEntity(Entity.get());

		auto comps = Entity->GetComponentList();
		for (auto &compItem : *comps)
		{
			auto comp = compItem.lock();
			if (comp->IsPendingKill() || !comp->IsEnabled())
			{
				// GC will take care of me
				continue;
			}
			if(auto scene = std::dynamic_pointer_cast<SceneComponent>(comp))
			{
				// resolve dirty transforms of components
				if (isPositionDirty || isRotationDirty)
				{
					scene->RecalculatePosition();
				}
			}
			comp->Update(DeltaTime);
		}
	}
	CollectGarbage();
}

void ObjectManager::RenderAllObjects(sf::RenderWindow& Window) const
{	
	// draw entities
	for (const auto& Item : RenderEntities)
	{
		auto Entity = Item.lock();
		if (Entity == nullptr || Entity->IsPendingKill() || !Entity->GetActive())
		{
			//if waiting to die or is inactive, we don't need to do anything
			continue;
		}
		Entity->Render(Window);
		
		std::vector<std::weak_ptr<PrimitiveComponent>> Comps;
		Entity->GetComponentsOfType<PrimitiveComponent>(Comps);
		
		auto entityTransformed = Entity->getTransform();
		for (auto& CompItem : Comps)
		{
			auto comp = CompItem.lock();
			if (comp->IsPendingKill())
			{
				// GC will take care of me
				continue;
			}
			sf::RenderStates states;
			states.transform = entityTransformed * comp->getTransform();
			comp->Render(Window, states);
		}
		for (auto& Drawer : Entity->Drawables)
		{
			if (Drawer.get())
			{
				sf::RenderStates states;
				states.transform = entityTransformed;
				if (auto Transformer = std::dynamic_pointer_cast<sf::Transformable>(Drawer))
				{
					states.transform *= Transformer->getTransform();
				}
				Window.draw(*Drawer, states);
			}
		}
	}
}

void ObjectManager::CleanUpOldObjects()
{
	Objects.clear();
	UpdateEntities.clear();
	RenderEntities.clear();
}

void ObjectManager::DestroyObject(Object* Obj)
{
	// delete entity from entity lists	
	for (auto it = UpdateEntities.begin(); it != UpdateEntities.end(); ++it)
	{
		if ((*it).lock()->GetObjectIndex() == Obj->GetObjectIndex())
		{
			System::GetInstance()->Event_EntityDestroyed.Invoke(*it);
			UpdateEntities.erase(it);
			break;
		}
	}
	for (auto it = RenderEntities.begin(); it != RenderEntities.end(); ++it)
	{
		if ((*it).lock()->GetObjectIndex() == Obj->GetObjectIndex())
		{
			RenderEntities.erase(it);
			break;
		}
	}
	RemoveObjectAtIndex(Obj->GetObjectIndex());
}

void ObjectManager::CollectGarbage()
{
	for(size_t i = 0; i < Objects.size(); i++)
	{
		if (!Objects[i]->IsPendingKill())
		{
			continue;
		}

		//if waiting to die
		DestroyObject(Objects[i].get());

		// the object at the end of the list was brought at this index by destroy()
		// it also needs to be indexed
		i--;
	}
}

void ObjectManager::SetRenderDirty(bool status)
{
	bResolveRenderDirty = status;
}

void ObjectManager::SetUpdateDirty(bool status)
{
	bResolveUpdateDirty = status;
}

void ObjectManager::ResolveRenderDirty()
{
	if (!bResolveRenderDirty)
	{
		return;
	}
	for (auto it = RenderEntities.begin(); it != RenderEntities.end(); ++it)
	{
		if (auto entity = (*it).lock())
		{
			if (!entity->IsPendingKill() && entity->IsRenderDirty())
			{
				entity->SetRenderDirty(false);
				it = RenderEntities.erase(it);

				RenderEntities.insert(entity);
			}
		}
	}
	SetRenderDirty(false);
}

void ObjectManager::ResolveUpdateDirty()
{
	if (!bResolveUpdateDirty)
	{
		return;
	}
	for (auto it = UpdateEntities.begin(); it != UpdateEntities.end(); ++it)
	{
		if (auto entity = (*it).lock())
		{
			if (!entity->IsPendingKill() && entity->IsUpdateDirty())
			{
				entity->SetUpdateDirty(false);
				it = UpdateEntities.erase(it);

				UpdateEntities.insert(entity);
			}
		}
	}
	SetUpdateDirty(false);
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

bool ObjectManager::CanCreateObject() const { return Objects.size() < MaxObjects; }

void ObjectManager::SetWindowVals(float X, float Y)
{
	WindowWidth = X;
	WindowHeight = Y;
}

bool UpdatePriorityComparator::operator()(std::weak_ptr<GameEntity> x,
										  std::weak_ptr<GameEntity> y) const
{
	return x.lock()->GetUpdatePriority() < y.lock()->GetUpdatePriority();
}

bool RenderPriorityComparator::operator()(std::weak_ptr<GameEntity> x,
										  std::weak_ptr<GameEntity> y) const
{
	return x.lock()->GetRenderPriority() < y.lock()->GetRenderPriority();

}
