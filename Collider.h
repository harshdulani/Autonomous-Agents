#pragma once
#include <SFML/Graphics/VertexArray.hpp>

#include "PrimitiveComponent.h"

class Collider : public PrimitiveComponent
{
public:
	void Update(float DeltaTime) override;
	void Render(sf::RenderWindow& Window, sf::RenderStates States) override;
	bool IsValidCollider() const;

	float GetRadius() const;
	void SetRadius(float r);

	bool GetShowColliderVisible() const;
	void SetColliderVisible(bool bStatus);
	
private:
	void RegenCircle();

private:
	sf::VertexArray Circle;
	
	bool bVisualizeCollider = false;
	float Radius = -1.f;
};
