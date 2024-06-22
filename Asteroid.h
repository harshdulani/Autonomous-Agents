#pragma once
#include <SFML/Graphics/Color.hpp>

#include "GameEntity.h"

class Asteroid : public GameEntity
{
public:

	void InitAsteroid(float Radius, sf::Color Color);
	
private:
};
