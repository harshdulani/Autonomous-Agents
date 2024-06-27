#pragma once
#include "GameEntity.h"

class GridBackground : public GameEntity
{
public:
	void Render(sf::RenderWindow& Window) const override;
};
