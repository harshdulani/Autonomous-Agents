#include "PhysicsComponent.h"

#include "GameEntity.h"

sf::Vector2f PhysicsComponent::GetVelocity() const { return Velocity; }

float PhysicsComponent::GetAngularVelocity() const { return AngularVelocity; }

void PhysicsComponent::SetVelocity(sf::Vector2f InVel)
{
	Velocity = InVel;
}

void PhysicsComponent::SetVelocity(float X, float Y)
{
	Velocity.x = X;
	Velocity.y = Y;
}

void PhysicsComponent::SetAngularVelocity(float InVel)
{
	AngularVelocity = InVel;
}

void PhysicsComponent::Update(float DeltaTime)
{
	auto Entity = GetOwningEntity().lock();
	if (!Entity)
		return;
	if (Entity->IsPendingKill())
		return;

	Entity->setPosition(Entity->getPosition() + Velocity * DeltaTime);
	Entity->setRotation(Entity->getRotation() + AngularVelocity * DeltaTime);
}
