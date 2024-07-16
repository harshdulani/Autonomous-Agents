#pragma once

#include "pch.h"
#include "../SceneComponent.h"
#include "Particle.h"
#include "../Maths.h"
#include "BakedParticleShape.h"
#include "LogicalParticleShape.h"

class AParticleShape;
class ParticleSystemManager;

struct EmitterBurst
{
public:
	EmitterBurst() : count(1), countMin(-1), durationValue(0.f), didBurst(false)
	{ 
	}
	/// <param name="count">- No of particles to emit during burst</param>
	/// <param name="countMin">- If countMin > 0, burst spawn particles in a range from c to cmin</param>
	/// <param name="durationValue">- Ranges between 0 and 1 to signify 0 to totalDuration of particle sys</param>
	EmitterBurst(int count, int countMin, float durationValue) : didBurst(false)
	{
		this->count = count;
		this->countMin = countMin;
		this->durationValue = durationValue;
	}

	int count;
	int countMin;
	float durationValue;
	bool didBurst;
};

enum class EmissionMode
{
	LinearLooping,
	Random
};

enum class TransformSpace
{
	World,
	Local
};

enum class EmissionVelocityMode
{
	Outward,
	RightTangent,
	LeftTangent,
	Normal
};

class ParticleSystem : public SceneComponent
{
	friend class ParticleSystemManager;
public:
	/// <summary>
	/// IMPORTANT, we delay the death of the particle system, 
	/// so that all the active particles it is managing can use its shapes till they die
	/// </summary>
	void Kill() override;

	ParticleSystem();

	void Update(float deltaTime) override;

	void Play();
	void Stop();

	ParticleSystem& SetAutoPlay(bool val);
	ParticleSystem& SetOnStart(const std::function<void()>& onStart);
	ParticleSystem& SetOnComplete(const std::function<void()>& onComplete);

	ParticleSystem& SetRenderPriority(int priority);
	ParticleSystem& SetDuration(float seconds);
	ParticleSystem& SetLooping(const bool status);
	ParticleSystem& SetMaxParticles(int count);

	// initialise particle system to have shape as a collection of defined vertices for the emitter
	ParticleSystem& SetEmissionShape(const std::vector<sf::Vector2f>& vertices,
									 int resolution);
	// initialise particle system to have shape as a function for the emitter
	ParticleSystem& SetEmissionShape(const std::function<sf::Vector2f(float)>& shapeFunction,
									 int resolution);
	ParticleSystem& SetEmitterScaling(float scalingValue);
	ParticleSystem& SetEmissionMode(EmissionMode mode);
	ParticleSystem& SetEmitterRate(float emitterRate, float emitterRateMin = -1.f);
	ParticleSystem& AddEmitterBurst(float durationValue, int count, int countMin = -1);
	ParticleSystem& SetEmissionSpeed(float particleSpeed, float particleSpeedMin = -1);
	ParticleSystem& SetEmitterRotationMode(TransformSpace rotation);

	ParticleSystem& SetEmissionVelocityMode(EmissionVelocityMode val);


	// initialise particle system to have shape as a collection of defined vertices for the emitter
	ParticleSystem& SetParticleShape(const std::vector<sf::Vector2f>& vertices,
									 int resolution);
	// initialise particle system to have shape as a function for the emitter
	ParticleSystem& SetParticleShape(const std::function<sf::Vector2f(float)>& shapeFunction,
									 int resolution);

	ParticleSystem& SetParticleLifeTime(float seconds, float secondsMax = -1.f);

	// This will be applied on top of any scaling that the emitter has
	ParticleSystem& SetParticleScale(float scale, float scaleMin = -1.f);

	/// <summary>If set, OverLifetime values will override initial values</summary>
	ParticleSystem& SetScaleOverLifetime(float initialScale,
										 float finalScale);

	ParticleSystem& SetParticleColor(const sf::Color& color);
	ParticleSystem& SetParticleColor(const sf::Color& randomBetweenColorA,
									 const sf::Color& randomBetweenColorB);

	/// <summary>If set, OverLifetime values will override initial values</summary>
	ParticleSystem& SetColorOverLifeTime(const sf::Color& startColor,
										 const sf::Color& endColor);

	bool GetShouldAutoPlay() const;
	bool GetIsPlaying() const { return isPlaying_; }
	void SetIsPlaying(bool val) { isPlaying_ = val; }

private:
	bool ShouldEmit() const;

	void EmitterUpdate(float deltaTime);
	void BurstsUpdate();
	void CheckForExpiredLifetimes();

	std::vector<std::weak_ptr<Particle>>& GetActiveParticles();
	void SpawnParticle();
	void InitialiseParticle(const std::weak_ptr<Particle>& particle);
	void UpdateSpawnInterval(float rate);

	void RemoveParticleAtIndex(int index);

private:
	// IMPORTANT, related to life &cycle
	bool READY_TO_DIE = false;

	//variables
	int particleRenderPriority_ = 0;
	bool isPlaying_ = false;
	bool shouldAutoPlay_ = true;
	bool isStopped_ = false;
	std::function<void()> onStart_;
	std::function<void()> onComplete_;
	bool hasCalledOnStart_ = false;
	bool hasCalledOnComplete_ = false;

	float totalDuration_ = 2.f;
	float timeElapsed_ = 0.f;

	float currentSpawnInterval_ = 0.f;
	float spawnElapsed_ = 0.f;

	int maxParticles_= 100;

	float particleLifetime_= 1.f;
	float particleLifetimeMax_ = -1.f;

	bool shouldLoop_ = false;

	bool shouldUseStaticEmitterShip_ = false;
	BakedParticleShape staticEmitterShape_;
	LogicalParticleShape logicalEmitterShape_;

	EmissionMode emissionMode_ = EmissionMode::Random;
	TransformSpace emitterRotationSpace_ = TransformSpace::World;
	EmissionVelocityMode emissionVelocityMode_ = EmissionVelocityMode::Outward;
	float emitterRate_ = 0.f;
	float emitterRateMin_ = -1.f;

	bool shouldUseStaticParticleShip_ = false;
	BakedParticleShape staticParticleShape_;
	LogicalParticleShape logicalParticleShape_;

	bool shouldScaleOverLifetime_ = false;
	float particleScalingValue_ = 1.f;
	float particleScalingValueMin_ = -1.f;
	float scaleOverLifetimeStart_ = -1.f;
	float scaleOverLifetimeEnd_ = -1.f;

	float particleSpeed_ = 0.f;
	float particleSpeedMin_ = -1.f;

	sf::Color particleColor_;
	sf::Color particleRandomColorRangeFrom_;
	sf::Color particleRandomColorRangeTo_;
	bool shouldChangeColorOverLifetime_ = false;
	sf::Color colorOverLifetimeFrom_;
	sf::Color colorOverLifetimeTo_;

	std::vector<EmitterBurst> bursts_;

	ParticleSystemManager* particleSysMgr_ = nullptr;
	std::vector<std::weak_ptr<Particle>> activeParticles_;
};
