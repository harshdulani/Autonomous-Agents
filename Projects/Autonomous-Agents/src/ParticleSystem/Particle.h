#pragma once

#include "../APoolable.h"
#include "../GameEntity.h"
#include "AParticleShape.h"

class TimerManager;

class Particle : public GameEntity, public APoolable
{
	friend class ParticleSystem;
public:
	Particle();
	~Particle() override;

	void Update(float deltaTime) override;
	void Render(sf::RenderWindow& Window) const override;

	void SetRenderColor(const sf::Color& color);
	sf::Color GetRenderColor() const;
	void SetColorOverLifeRange(const sf::Color& startColor, const sf::Color& endColor);
	void SetScaleOverLifeRange(float startScale, float endScale);

	float GetLifeTimeDoneValue() const;
	void UpdateScaleOverLifetime(float lifeTime);
	void UpdateColorOverLifetime(float lifeTime);

private:
	//Getters
	bool GetIsLifetimeOver() const;

	//Setters
	// Will reset timer (if present) and continue till new lifetime
	Particle* SetLifeTime(float lifetime);
	Particle* SetRenderingShape(AParticleShape* shape);	
	Particle* BakeRenderingShape();
	Particle* SetVelocity(const sf::Vector2f& velocity);

protected:
	// APoolable
	void InitPoolable() override;

private:
	//variables
	TimerManager* timerManager_ = nullptr;

	uint32_t lifeTimerHandle_ = 0;
	sf::Vector2f velocity_;

	//shape
	AParticleShape* logicalRenderShape_ = nullptr;
	std::vector<sf::Vertex> staticRenderShape_;
	
	bool hasScaleOverLifetime_ = false;
	float initialScaleValue_ = 0.f;
	float finalScaleValue_= 0.f;

	bool hasColorOverLifetime_ = false;
	sf::Color renderColor_;
	sf::Color initialColor_;
	sf::Color finalColor_;
};
