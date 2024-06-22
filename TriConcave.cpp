#include "TriConcave.h"
#include <SFML/Graphics/RenderTarget.hpp>

void TriConcave::SetTris(const std::vector<sf::Vector2f>& InTris, const sf::Color Color)
{
	Tris.clear();
	for (int i = 0; i < InTris.size(); ++i)
	{
		if (i + 2 < InTris.size())
		{
			auto va = sf::VertexArray(sf::Triangles, 3);
			va[0] = sf::Vertex(InTris[i], Color);
			va[1] = sf::Vertex(InTris[i + 1], Color);
			va[2] = sf::Vertex(InTris[i + 2], Color);
			Tris.push_back(va);
			i += 2;
		}
	}
}

void TriConcave::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (const auto& Tri : Tris)
		target.draw(Tri, states);
}