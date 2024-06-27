#pragma once
#include <SFML/Graphics/RenderStates.hpp>

#include "SceneComponent.h"

namespace sf
{
	class RenderWindow;
}

class PrimitiveComponent : public SceneComponent
{
public:
	virtual void Render(sf::RenderWindow& Window, sf::RenderStates States);
};
