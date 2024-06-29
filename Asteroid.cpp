#include "Asteroid.h"
#include <SFML/Graphics/ConvexShape.hpp>
#include "AShootingStrategy.h"
#include "PhysicsComponent.h"
#include "Math.h"
#include "PlayerShip.h"
#include "Bullet.h"
#include "Collider.h"

void Asteroid::OnCollision(std::weak_ptr<GameEntity> other)
{
	GameEntity::OnCollision(other);
	auto that = other.lock();
	auto bullet = std::dynamic_pointer_cast<Bullet>(that);

	if (bullet || std::dynamic_pointer_cast<PlayerShip>(that))
	{
		if (bullet)
		{
			//destroy bullet
			that->Kill();
			if (!bullet->GetIsPlayerOwned())
			{
				return;
			}
		}

		AsteroidHit();
	}
}

void Asteroid::InitAsteroid(float radius, sf::Color color, float angularMomentum, int size)
{
	CreateCollider(radius);

	auto PhysicComponent = AddComponent<PhysicsComponent>();
	if (auto Physics = PhysicComponent.lock())
	{
		constexpr float SpeedRand = 50.f;
		Physics->SetVelocity(Math::GetRandFloat(-SpeedRand, SpeedRand),
							 Math::GetRandFloat(-SpeedRand, SpeedRand));
		Physics->SetAngularVelocity(angularMomentum);
	}
	size_ = size;
	constexpr int MinPoly = 6;
	constexpr int MaxPoly = 8;

	float polyCount = static_cast<float>(Math::GetRandInt(MinPoly, MaxPoly));

	auto weakShape = CreateDrawable<sf::ConvexShape>(static_cast<size_t>(polyCount));

	if (auto Shape = weakShape.lock())
	{
		for (int i = 0; i < static_cast<int>(polyCount); ++i)
		{
			float angle = static_cast<float>(i) / polyCount * 2 * Math::PI;
			float X = cos(angle);
			float Y = sin(angle);
			Shape->setPoint(i, sf::Vector2f(X, Y) * (radius));
		}
		Shape->setFillColor(color);
		color = sf::Color::White;
		color.a = 55;
		Shape->setOutlineColor(color);
		Shape->setOutlineThickness(2.5f);
	}
}

void Asteroid::AsteroidHit()
{
	int oldSize = GetSize();
	Game& game = System::GetInstance()->GetGame();
	if (oldSize > 1)
	{
		int smallerSize = oldSize - 1;
		sf::Vector2f position = GetPosition();

		sf::Vector2f offset = GetRightVector() * 10.0f;

		game.SpawnAsteroidAt(position + offset, smallerSize);
		game.SpawnAsteroidAt(position - offset, smallerSize);
	}
	SpawnExplosion(oldSize);
	game.UpdateScore(oldSize);

	SetActive(false);
	// we dont need collisions after being hit/ destroyed 
	// but this entity needs to stay alive for particle sys component
	DestroyAllComponentsOfType<Collider>();
}

void Asteroid::SpawnExplosion(int oldSize)
{
}

int Asteroid::GetSize() const { return size_; }