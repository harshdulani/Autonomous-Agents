#include "Asteroid.h"
#include <SFML/Graphics/ConvexShape.hpp>
#include "Ships/AShootingStrategy.h"
#include "Physics/PhysicsComponent.h"
#include "Maths.h"
#include "Ships/PlayerShip.h"
#include "Ships/Bullet.h"
#include "Physics/Collider.h"
#include "ScreenShaker.h"
#include "ParticleSystem/ParticleSystem.h"
#include "ParticleSystem/ParticleSystemManager.h"

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
		Physics->SetVelocity(Maths::GetRandFloat(-SpeedRand, SpeedRand),
							 Maths::GetRandFloat(-SpeedRand, SpeedRand));
		Physics->SetAngularVelocity(angularMomentum);
	}
	size_ = size;
	constexpr int MinPoly = 6;
	constexpr int MaxPoly = 8;

	float polyCount = static_cast<float>(Maths::GetRandInt(MinPoly, MaxPoly));

	auto weakShape = CreateDrawable<sf::ConvexShape>(static_cast<size_t>(polyCount));

	if (auto Shape = weakShape.lock())
	{
		for (int i = 0; i < static_cast<int>(polyCount); ++i)
		{
			float angle = static_cast<float>(i) / polyCount * 2 * Maths::PI;
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
	System::GetInstance()->GetScreenShaker()->CreateImpulse(0.25f, oldSize * 1.0f, 100.0f);

	auto particleSys = System::GetInstance()->GetParticleSystemManager()->CreateNewParticleSystem(this);
	std::shared_ptr<ParticleSystem> p = particleSys.lock();

	p->SetWorldPosition(GetPosition());

	std::function<void()> onStart = []() -> void
		{
			System::GetInstance()->GetGame().ExplosionCount++;
		};

	std::function<void()> onEnd = [&]() -> void
		{
			// For level complete checking
			System::GetInstance()->GetGame().SetAsteroidsDestroyed(System::GetInstance()->GetGame().GetAsteroidsDestroyed() + 1);
			System::GetInstance()->GetGame().AsteroidCount--;
			System::GetInstance()->GetGame().ExplosionCount--;

			if (!IsPendingKill())
			{
				// object is here to be marked as pending kill to be destroyed in the next update loop
				Kill();
				return;

				// it wont come here twice in the same frame 
				// bc if it is already marked for death, then it wont be checked for collisions
			}
		};

	float oldSizeF = static_cast<float>(oldSize);
	
	p->SetOnStart(onStart)
		.SetOnComplete(onEnd);
	
	p->SetDuration(0.125f)
		.SetParticleLifeTime(0.25f * oldSizeF, 0.5f * oldSizeF)

		.SetEmissionShape(LogicalParticleShape::CircleSamplingFunction, 32)
		.SetEmissionMode(EmissionMode::Random)
		.SetEmitterScaling(3.f * oldSizeF)
		.SetEmitterRate(0.f)
		.AddEmitterBurst(0.f, 16)
		.SetEmissionSpeed(50.f)

		.SetParticleShape(LogicalParticleShape::CircleSamplingFunction, 32)
		.SetScaleOverLifetime(1.5f * oldSizeF, 0.f)
		.SetColorOverLifeTime(sf::Color(255, 255, 0), sf::Color::Black);
}

int Asteroid::GetSize() const { return size_; }