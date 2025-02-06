#pragma once
#include <cassert>
#include <memory>
#include <set>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Component.h"
#include "SceneComponent.h"

class Game;
class GameEntity;

namespace sf
{
	class RenderWindow;
}

struct UpdatePriorityComparator
{
public:
	bool operator() (const std::weak_ptr<GameEntity>& x, const std::weak_ptr<GameEntity>& y) const;
};
struct RenderPriorityComparator
{
public:
	bool operator() (const std::weak_ptr<GameEntity>& x, const std::weak_ptr<GameEntity>& y) const;
};

class ObjectManager
{
	friend class GameEntity;
public:
	ObjectManager();
	~ObjectManager();

	void UpdateAllObjects(float DeltaTime);
	void RenderAllObjects(sf::RenderWindow& Window);
	
	void CleanUpOldObjects();
	void DestroyObject(const Object* Obj);

	void SetWindowVals(float X, float Y);
	
	template<typename T>	std::weak_ptr<T> CreateObject();
	
	template<typename T>	std::weak_ptr<T> CreateGameEntity(const sf::Vector2f& Pos);

	std::weak_ptr<GameEntity> GetWeakPtr(GameEntity* RawPtr);
	
private:
	void CollectGarbage();

	void SetRenderDirty(bool status);
	void SetUpdateDirty(bool status);

	void ResolveUpdateDirty();
	void ResolveRenderDirty();
	
	void AddToObjectList(const std::shared_ptr<Object>& NewObject);

	static void SetObjectIndex(const std::shared_ptr<Object>& Shared, int Index);
	/// Removes object from the index and fills the gap by bringing an object from the end of the container
	void RemoveObjectAtIndex(int Index);
	std::weak_ptr<Object> GetObjectByIndex(int Index);
	
	void WrapEntity(GameEntity* Entity) const;

	bool CanCreateObject() const;
	
	template<typename T>	std::weak_ptr<T> CreateComponent(GameEntity* OwningEntity);
	
public:
private:
	// Array of pointers to all Objects
	std::vector<std::shared_ptr<Object>> Objects;
	
	//  Priority Array of pointers to all Entities
	std::multiset<std::weak_ptr<GameEntity>, UpdatePriorityComparator> UpdateEntities;
	std::multiset<std::weak_ptr<GameEntity>, RenderPriorityComparator> RenderEntities;
		
	int maxObjects_ = 10000;
	
	float windowWidth_ = 0;
	float windowHeight_ = 0;

	bool bResolveRenderDirty_ = false;
	bool bResolveUpdateDirty_ = false;
	
	uint32_t eventHandle_LevelEnd_ = 0;
};

#include "ObjectManager.inl"
