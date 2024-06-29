#pragma once
#include <cassert>
#include <memory>
#include <set>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Component.h"
#include "GameEntity.h"
#include "SceneComponent.h"

class Game;

namespace sf
{
	class RenderWindow;
}

struct UpdatePriorityComparator
{
public:
	bool operator() (std::weak_ptr<GameEntity> x, std::weak_ptr<GameEntity> y) const;
};
struct RenderPriorityComparator
{
public:
	bool operator() (std::weak_ptr<GameEntity> x, std::weak_ptr<GameEntity> y) const;
};

class ObjectManager
{
	friend class GameEntity;
public:
	ObjectManager();
	~ObjectManager();

	void UpdateAllObjects(float DeltaTime);
	void RenderAllObjects(sf::RenderWindow& Window) const;
	
	void CleanUpOldObjects();
	void DestroyObject(Object* Obj);

	void SetWindowVals(float X, float Y);
	
	template<typename T>
	std::weak_ptr<T> CreateObject();
	
	template<typename T>
	std::weak_ptr<T> CreateGameEntity(const sf::Vector2f& Pos);

	std::weak_ptr<GameEntity> GetWeakPtr(GameEntity* RawPtr);
	
private:
	void CollectGarbage();

	void SetRenderDirty(bool status);
	void SetUpdateDirty(bool status);

	void ResolveUpdateDirty();
	void ResolveRenderDirty();
	
	void AddToObjectList(std::shared_ptr<Object> NewObject);

	static void SetObjectIndex(const std::shared_ptr<Object>& Shared, int Index);
	/// Removes object from the index and fills the gap by bringing an object from the end of the container
	void RemoveObjectAtIndex(int Index);
	std::weak_ptr<Object> GetObjectByIndex(int Index);
	
	void WrapEntity(GameEntity* Entity) const;

	bool CanCreateObject() const;
	
	template<typename T>
	std::weak_ptr<T> CreateComponent(GameEntity* OwningEntity);
	
public:
private:
	// Array of pointers to all Objects
	std::vector<std::shared_ptr<Object>> Objects;
	
	//  Priority Array of pointers to all Entities
	std::multiset<std::weak_ptr<GameEntity>, UpdatePriorityComparator> UpdateEntities;
	std::multiset<std::weak_ptr<GameEntity>, RenderPriorityComparator> RenderEntities;
		
	int MaxObjects = 10000;
	
	float WindowWidth = 0;
	float WindowHeight = 0;

	bool bResolveRenderDirty = false;
	bool bResolveUpdateDirty = false;
	
	uint32_t eventHandle_levelEnd_ = 0;
};

template<typename T> std::weak_ptr<T> ObjectManager::CreateObject()
{
	assert((CanCreateObject() && "Pool full, can't manage any more objects"));
	assert((std::is_base_of<Object, T>::value && "Type mismatch while creating object"));
	
	auto newObject = std::make_shared<T>();
	
	AddToObjectList(newObject);

	return newObject;
}

template<typename T> std::weak_ptr<T> ObjectManager::CreateGameEntity(const sf::Vector2f& Pos)
{
	assert((std::is_base_of<GameEntity, T>::value && "Type mismatch while creating GameEntity"));

	std::weak_ptr<T> NewEntity = CreateObject<T>();
	const std::shared_ptr<GameEntity> Casted = std::static_pointer_cast<GameEntity>(NewEntity.lock());
	
	Casted->SetPosition(Pos);

	//add to update & render priority lists
	UpdateEntities.insert(NewEntity);
	RenderEntities.insert(NewEntity);

	System::GetInstance()->Event_EntityCreated.Invoke(NewEntity);
	
	return std::weak_ptr<T>(NewEntity);
}

template<typename T> std::weak_ptr<T> ObjectManager::CreateComponent(GameEntity* OwningEntity)
{
	assert((std::is_base_of<Component, T>::value && "Type mismatch while creating Component"));
	
	std::weak_ptr<T> WeakCmp = CreateObject<T>();
	
	auto OwningObject = GetObjectByIndex(OwningEntity->GetObjectIndex()).lock();
	auto entity = std::dynamic_pointer_cast<GameEntity>(OwningObject);

	std::shared_ptr<Component> casted = std::static_pointer_cast<Component>(WeakCmp.lock());
	casted->SetOwningEntity(entity);
	
	if(auto scene = std::dynamic_pointer_cast<SceneComponent>(casted))
	{
		scene->setWorldPosition(entity->GetPosition());
	}
	return WeakCmp;
}
