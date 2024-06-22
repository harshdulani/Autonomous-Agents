#pragma once
#include <SFML/System/Vector2.hpp>
#include "Component.h"

class PhysicsComponent final : public Component
{
public:
	void SetVelocity(sf::Vector2f InVel);
	void SetVelocity(float X, float Y);
	sf::Vector2f GetVelocity() const;
	
	void SetAngularVelocity(float InVel);
	float GetAngularVelocity() const;

	//@todo: FixedUpdate(fixed delta time)
	void Update(float DeltaTime) override;
	
private:	
	sf::Vector2f Velocity;
	float AngularVelocity = 0.f;
};
