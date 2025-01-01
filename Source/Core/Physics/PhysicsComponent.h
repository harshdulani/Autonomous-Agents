#pragma once
#include <SFML/System/Vector2.hpp>
#include "Component.h"

class PhysicsComponent final : public Component
{
public:
	void SetVelocity(const sf::Vector2f& InVel);
	void SetVelocity(float X, float Y);
	sf::Vector2f GetVelocity() const;
	sf::Vector2f GetAcceleration() const;
	
	void SetAngularVelocity(float InVel);
	float GetAngularVelocity() const;

	void AddForce(const sf::Vector2f& Force);
	
	//@todo: FixedUpdate(fixed delta time)
	void Update(float DeltaTime) override;
	
private:	
	sf::Vector2f velocity_;
	sf::Vector2f acceleration_;
	
	float angularVelocity_ = 0.f;
};
