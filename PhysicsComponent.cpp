#include "PhysicsComponent.h"

#include "GameEntity.h"
#include "Math.h"

sf::Vector2f PhysicsComponent::GetVelocity() const { return velocity_; }

sf::Vector2f PhysicsComponent::GetAcceleration() const { return acceleration_; }

float PhysicsComponent::GetAngularVelocity() const { return angularVelocity_; }

void PhysicsComponent::AddForce(const sf::Vector2f& Force)
{
	acceleration_ += Force;
}

void PhysicsComponent::SetVelocity(const sf::Vector2f& InVel)
{
	velocity_ = InVel;
}

void PhysicsComponent::SetVelocity(float X, float Y)
{
	velocity_.x = X;
	velocity_.y = Y;
}

void PhysicsComponent::SetAngularVelocity(float InVel)
{
	angularVelocity_ = InVel;
}

void PhysicsComponent::Update(float DeltaTime)
{
	acceleration_ += velocity_;
	acceleration_ *= 0.f;
	
	auto Entity = GetOwningEntity().lock();
	if (!Entity)
		return;
	if (Entity->IsPendingKill())
		return;

	if(Math::GetVectorSqrMagnitude(velocity_) > 0)
		Entity->SetPosition(Entity->GetPosition() + velocity_ * DeltaTime);
	if(angularVelocity_ > 0 || angularVelocity_ < 0)
		Entity->SetRotation(Entity->GetRotation() + angularVelocity_ * DeltaTime);
}
