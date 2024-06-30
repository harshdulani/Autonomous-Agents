#pragma once

#include <memory>
#include "GameEntity.h"
#include "Game.h"
#include "PhysicsComponent.h"
#include "MultiTriShape.h"

class Bullet : public GameEntity
{
public:
	Bullet();

	~Bullet() override;

	// Constructor to create new bullet that fires straight in direction
	void InitialiseStraightBullet(sf::Vector2f direction, bool bSingleShot);

	// Constructor to create new bullet that moves in a sine (actually cos) wave
	void InitialiseSineWaveBullet(sf::Vector2f Direction,
								  sf::Vector2f Perpend,
								  float WaveAmpl,
								  float WaveFreq);

	void Update(float DeltaTime) override;

	void OnCollision(std::weak_ptr<GameEntity> other) override;

	bool GetIsPlayerOwned() const { return bPlayerOwned; }
	void SetIsPlayerOwned(bool val) { bPlayerOwned = val; }

private:
	sf::Vector2f Velocity;

	enum class BulletFiringMode
	{
		Standard, Triple, Sine
	};
	BulletFiringMode BulletFiringMode = BulletFiringMode::Standard;
	sf::Vector2f Perpendicular;

	bool bPlayerOwned = true;

	//lifetime
	TimerManager* TimerManager;
	uint32_t LifeTimerHandle = 0;

	//sine wave attack
	float WaveAmplitude = 0.f;
	float WaveFrequency = 0.f;

	// appearance
	std::weak_ptr<sf::RectangleShape> InitSquareShape(float Size);
	std::weak_ptr<MultiTriShape> InitialiseSpikeShape(sf::Color RenderColor);
	std::weak_ptr<sf::CircleShape> InitialiseCircleShape(float Radius);
};
