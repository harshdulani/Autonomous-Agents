#pragma once
#include "pch.h"

class GameEntity;

class Physics
{
public:
	static std::weak_ptr<GameEntity> Raycast(const sf::Vector2f& start,
											 const sf::Vector2f& end,
											 const GameEntity* self);

	static void RaycastAll(const sf::Vector2f& start,
					const sf::Vector2f& end,
					const GameEntity* self,
					std::vector<std::weak_ptr<GameEntity>>& inVector);

	static void CircleCast(const sf::Vector2f& position,
					float radius,
					std::vector<std::weak_ptr<GameEntity>>& inVector);
};
