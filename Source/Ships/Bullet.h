#pragma once

#include <memory>
#include "GameEntity.h"
#include "Game.h"
#include "Physics/PhysicsComponent.h"
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

	bool GetIsPlayerOwned() const { return bPlayerOwned_; }
	void SetIsPlayerOwned(bool val) { bPlayerOwned_ = val; }

private:
	// appearance
	std::weak_ptr<sf::RectangleShape> InitSquareShape(float Size);
	std::weak_ptr<MultiTriShape> InitialiseSpikeShape(sf::Color RenderColor);
	std::weak_ptr<sf::CircleShape> InitialiseCircleShape(float Radius);

public:
private:
	sf::Vector2f velocity_;

	enum class BulletFiringMode
	{
		Standard, Triple, Sine
	};
	BulletFiringMode bulletFiringMode_ = BulletFiringMode::Standard;
	sf::Vector2f perpendicular_;

	bool bPlayerOwned_ = true;

	//lifetime
	TimerManager* timerManager_;
	uint32_t lifeTimerHandle_ = 0;

	//sine wave attack
	float waveAmplitude_ = 0.f;
	float waveFrequency_ = 0.f;
};
