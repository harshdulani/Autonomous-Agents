#pragma once
#include <cassert>
#include <memory>
#include <vector>
#include <SFML/System/Vector2.hpp>

#include "GameEntity.h"

class Object;
class Game;
class GameEntity;

namespace sf
{
	class RenderWindow;
}

class ObjectManager
{
public:
	void UpdateAllObjects(float DeltaTime);
	void RenderAllObjects(sf::RenderWindow* Window) const;

	void SetWindowVals(float X, float Y);

	template<typename T>
	std::weak_ptr<T> CreateObject();
	
	template<typename T>
	std::weak_ptr<T> CreateGameEntity(const sf::Vector2f& Pos);
	
private:
	void AddToObjectList(std::shared_ptr<Object> NewObject);

	static void SetObjectIndex(const std::shared_ptr<Object>& Shared, int Index);
	/// Removes object from the index and fills the gap by bringing an object from the end of the container
	void RemoveObjectAtIndex(int Index);
	
	std::weak_ptr<Object> GetObjectByIndex(int Index);

	// Array of pointers to all Objects
	std::vector<std::shared_ptr<Object>> Objects;

	/* Priority Queues
	// Array of pointers to all Entities
	std::multiset<std::weak_ptr<GameEntity>, update_priority_comparator> updateEntities_;
	std::multiset<std::weak_ptr<GameEntity>, render_priority_comparator> renderEntities_;
	*/
	void WrapEntity(GameEntity* Entity) const;

	bool CanCreateObject() const;
	
public:
private:
	std::vector<std::shared_ptr<GameEntity>> Entities;

	int MaxEntities = 10000;
	
	float WindowWidth = 0;
	float WindowHeight = 0;
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
	std::shared_ptr<GameEntity> casted = std::static_pointer_cast<GameEntity>(NewEntity.lock());
	
	casted->setPosition(Pos);

	//@todo: replace with below
	Entities.push_back(casted);
	//add to update & render priority lists
	//UpdateEntities.insert(newEntity);
	//RenderEntities.insert(newEntity);

	//System::GetInstance()->Event_EntityCreated.Invoke(newEntity);
	
	return std::weak_ptr<T>(NewEntity);
}
