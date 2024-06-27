#pragma once
#include <SFML/Graphics/VertexArray.hpp>

#include "PrimitiveComponent.h"

class Collider : public PrimitiveComponent
{
public:
	void Render(sf::RenderWindow& Window, sf::RenderStates States) override;
	bool IsValidCollider() const;

	float GetRadius() const;
	void SetRadius(float r);

	bool GetShowColliderVisible() const;
	void SetColliderVisible(bool bStatus);
	
private:
	void RegenCircle();

	sf::VertexArray Circle;
	
	bool bVisualizeCollider = false;
	float Radius = -1.f;
};
