#pragma once

#include <memory>
#include <SFML/System/Vector2.hpp>

class SceneComponent;
class Bullet;

class AShootingStrategy
{
public:
	AShootingStrategy() = default;
	virtual ~AShootingStrategy() = default;

	virtual bool TryShoot(SceneComponent& Muzzle, bool bPlayer) = 0;
	static std::weak_ptr<Bullet> SpawnBullet(sf::Vector2f Position);

	virtual float GetCooldownTime() const = 0;
};

class StandardShootingStrategy : public AShootingStrategy
{
public:
	bool TryShoot(SceneComponent& Muzzle, bool bPlayer) override;

	float GetCooldownTime() const override { return 0.15f; }
};

class TripleShootingStrategy : public AShootingStrategy
{
public:
	bool TryShoot(SceneComponent& Muzzle, bool bPlayer) override;

	float GetCooldownTime() const override { return 0.35f; }
};

class SineShootingStrategy : public AShootingStrategy
{
public:
	SineShootingStrategy();
	bool TryShoot(SceneComponent& Muzzle, bool bPlayer) override;
	void SetSineWaveProperties(float Amplitude, float Frequency);

	float GetCooldownTime() const override { return 0.25f; }

private:
	float waveAmplitude_;
	float waveFrequency_;
};