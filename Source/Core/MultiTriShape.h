#pragma once
#include <SFML/Graphics/Shape.hpp>

// made from multiple triangles
class MultiTriShape final : public sf::Drawable, public sf::Transformable
{
public:
	// InTris picks up tris as {0,1,2}, {3,4,5}
	void SetTris(const std::vector<sf::Vector2f>& InTris, sf::Color Color);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::vector<sf::VertexArray> tris_;
};
