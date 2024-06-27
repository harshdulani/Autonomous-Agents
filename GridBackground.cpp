#include "GridBackground.h"

void GridBackground::Render(sf::RenderWindow& Window) const
{
	GameEntity::Render(Window);

	auto va = sf::VertexArray(sf::Lines, 4);
	auto Color = sf::Color(255, 255, 255, 25);
	va[0] = sf::Vertex({400.f, 0.f}, Color);
	va[1] = sf::Vertex({400.f, 600.f}, Color);

	va[2] = sf::Vertex({0.f, 300.f}, Color);
	va[3] = sf::Vertex({800.f, 300.f}, Color);
	
	Window.draw(va);
}
