#pragma once
#include <memory>
#include <vector>
#include <SFML/System/Vector2.hpp>

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
	std::weak_ptr<T> CreateGameEntity(const sf::Vector2f& pos);
	
private:
	void WrapEntity(GameEntity* Entity) const;
	
public:
private:
	std::vector<std::shared_ptr<GameEntity>> Entities;
	float WindowWidth = 0;
	float WindowHeight = 0;
};

template<typename T> std::weak_ptr<T> ObjectManager::CreateGameEntity(const sf::Vector2f& pos)
{
	assert((std::is_base_of<GameEntity, T>::value && "Type mismatch while creating GameEntity"));

	//std::weak_ptr<T> newEntity = CreateObject<T>();
	//std::shared_ptr<GameEntity> casted = std::static_pointer_cast<GameEntity>(newEntity.lock());

	std::shared_ptr<T> casted = std::make_shared<T>();
	casted->setPosition(pos);

	//@todo: replace with below
	Entities.push_back(casted);
	//add to update & render priority lists
	//updateEntities_.insert(newEntity);
	//renderEntities_.insert(newEntity);

	//System::GetInstance()->Event_EntityCreated.Invoke(newEntity);

	// return newEntity
	return std::weak_ptr<T>(casted);
}
