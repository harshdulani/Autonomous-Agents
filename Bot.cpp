#include "Bot.h"
#include <vector>
#include "Math.h"
#include "PlayerShip.h"
#include "Asteroid.h"
#include "Collider.h"

Bot::Bot()
{
	fov_ = sinf(0.45f);
}

void Bot::InitialisePerception(float perceptionRadius)
{
	perceptionRadius_ = perceptionRadius;
	separationRadius_ = perceptionRadius_ * 0.5f;

	CreateCollider(perceptionRadius);
	auto weakCollider = GetComponentOfType<Collider>();
	if (auto coll = weakCollider.lock())
	{
		coll->SetColliderVisible(false);
	}
}

void Bot::Update(const float deltaTime)
{
	AddFlockingForces();
	
	//Position
	SetPosition(GetPosition() + velocity);
	
	//Rotation	
	sf::Vector2f lookVector = velocity;
	auto angle = atan2f(lookVector.y, lookVector.x) - Math::PI * 0.5f;
	angle = Math::WrapModulo(angle, Math::TWO_PI);
	SetRotation(Math::LerpUnclamped(GetRotation(), angle, 3.5f * deltaTime));
}

void Bot::OnCollision(std::weak_ptr<GameEntity> other)
{
	if (other.expired())
	{
		return;
	}
	auto that = other.lock();
	if (std::shared_ptr<Bot> boid = std::dynamic_pointer_cast<Bot>(that))
	{
		if (boid && Math::DotProduct(that->GetForwardVector(), GetForwardVector()) > fov_)
		{
			TryAddToList<Bot>(boid, flock_);
		}

		if (Math::GetVectorMagnitude(GetPosition() - that->GetPosition()) < GetSeparationRadius())
		{
			TryAddToList<Bot>(boid, separaters_);
		}
	}
}

void Bot::AddFlockingForces()
{
/*
	AddForce(Separation() * 1.25f);
	AddForce(Alignment() * 0.3f);
	AddForce(Cohesion() * 0.45f);
*/
	AddForce(Separation() * 0.8f);
	AddForce(Alignment() * 0.3f);
	AddForce(Cohesion() * 0.65f);
}

void Bot::UpdateVelocity(const float deltaTime)
{
	// Use and clear acceleration
	sf::Vector2f newVelocity = velocity + acceleration;
	newVelocity = Math::LimitVector(newVelocity, maxSpeed * deltaTime);
	velocity = newVelocity;
	acceleration *= 0.f;
}

sf::Vector2f Bot::SeekLocation(const sf::Vector2f& location) const
{
	// Velocity
	auto direction = location - this->GetPosition();
	sf::Vector2f desired = Math::Normalize(direction) * maxSpeed;

	// Steering force
	sf::Vector2f steerForce = desired - velocity;
	steerForce = Math::LimitVector(steerForce, maxSteerForce);

	return steerForce;
}

sf::Vector2f Bot::Separation()
{
	int count = 0;
	sf::Vector2f steerForce { };
	sf::Vector2f sum { };

	for (int i = 0; i < separaters_.size(); i++)
	{
		if (separaters_[i].expired())
		{
			//remove expired ref
			RemoveFromList(i, separaters_);
			continue;
		}
		if (auto other = separaters_[i].lock())
		{
			auto direction = GetPosition() - other->GetPosition();
			float objectRadius = other->GetSeparationRadius();

			float distance = Math::GetVectorMagnitude(direction);
			if (other->IsPendingKill() ||
				!other->GetActive() ||
				distance > separationRadius_)
			{
				// remove refs to dead or faraway friends
				RemoveFromList(i, separaters_);
				continue;
			}
			
			// the farther away someone is from us, lesser the force to be applied
			float factor = 1.f - (Math::InverseLerpUnclamped(0.f, GetSeparationRadius(), distance));
			
			sum += Math::Normalize(direction) * factor;
			count++;
		}
	}
	if (count > 0)
	{
		sum /= static_cast<float>(count);
		sum = Math::Normalize(sum) * maxSpeed;
		steerForce = sum - velocity;
		steerForce = Math::LimitVector(steerForce, maxSteerForce);
	}
	return steerForce;
}

sf::Vector2f Bot::Alignment()
{
	int count = 0;
	sf::Vector2f steerForce{ };
	sf::Vector2f sum{ };

	for (int i = 0; i < flock_.size(); i++)
	{
		if (flock_[i].expired())
		{
			//remove expired ref
			RemoveFromList(i, flock_);
			continue;
		}
		if (auto other = flock_[i].lock())
		{
			auto direction = GetPosition() - other->GetPosition();
			float distance = Math::GetVectorMagnitude(direction);

			if (other->IsPendingKill() ||
				!other->GetActive() ||
				distance > GetNeighborRadius()
				|| Math::DotProduct(other->GetForwardVector(), GetForwardVector()) < fov_
				)
			{
				// remove refs to dead or faraway friends
				RemoveFromList<Bot>(i, flock_);
				continue;
			}
			// the farther away someone is from us, lesser the force to be applied
			float factor = 1.f - (Math::InverseLerpClamped(0.f, GetSeparationRadius(), distance));
			sum += Math::Normalize(direction) * factor;
			count++;
		}
	}
	if (count > 0)
	{
		sum /= static_cast<float>(count);
		sum = Math::Normalize(sum) * maxSpeed;
		steerForce = sum - velocity;
		steerForce = Math::LimitVector(steerForce, maxSteerForce);
	}
	return steerForce;
}

sf::Vector2f Bot::Cohesion()
{
	int count = 0;
	sf::Vector2f steerForce{ };
	sf::Vector2f sum{ };

	for (int i = 0; i < flock_.size(); i++)
	{
		if (flock_[i].expired())
		{
			//remove expired ref
			RemoveFromList(i, flock_);
			continue;
		}
		if (auto other = flock_[i].lock())
		{
			float distance = Math::GetVectorMagnitude(GetPosition() - other->GetPosition());

			if (other->IsPendingKill() ||
				!other->GetActive() ||
				distance > GetNeighborRadius()) // we already removed boids that don't fit in the fov in alignment
			{
				// remove refs to dead or faraway friends
				RemoveFromList(i, flock_);
				continue;
			}
			sum += other->GetPosition();
			count++;
		}
	}
	if (count > 0)
	{
		sum /= static_cast<float>(count);
		steerForce = SeekLocation(sum);
	}
	return steerForce;
}

void Bot::AddForce(const sf::Vector2f& force)
{
	acceleration += force;
}

float Bot::GetPerceptionRadius() const
{
	return perceptionRadius_;
}
