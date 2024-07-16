#pragma once
#include "pch.h"

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
	sf::VertexArray circle_;
	
	bool bVisualizeCollider_ = false;
	float radius_ = -1.f;
};
