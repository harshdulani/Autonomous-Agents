#include "Particle.h"
#include <vector>
#include "System.h"
#include "TimerManager.h"
#include "Maths.h"

Particle::Particle()
{
	timerManager_ = System::GetInstance()->GetTimerManager();
}

Particle::~Particle()
{
	logicalRenderShape_ = nullptr;
	timerManager_->SetKillOnComplete(lifeTimerHandle_, true);
	timerManager_->ClearOnComplete(lifeTimerHandle_);
}

void Particle::Update(float deltaTime)
{
	if (!GetIsInUse())
	{
		return;
	}
	float lifeTimeValue = GetLifeTimeDoneValue();
	if (hasScaleOverLifetime_)
	{
		UpdateScaleOverLifetime(lifeTimeValue);
	}
	if (hasColorOverLifetime_)
	{
		UpdateColorOverLifetime(lifeTimeValue);
		// because you need to bake shape w render color
		BakeRenderingShape();
	}
	sf::Vector2f position = GetPosition() + velocity_ * deltaTime;

	SetPosition(position);
}

void Particle::Render(sf::RenderWindow& Window) const
{
	GameEntity::Render(Window);
	
	if (!GetIsInUse())
	{
		return;
	}

	sf::RenderStates states;
	states.transform = GetTransform();
	if (!staticRenderShape_.empty())
	{
		Window.draw(staticRenderShape_.data(), staticRenderShape_.size(), sf::LineStrip, states);
	}
}

void Particle::SetRenderColor(const sf::Color& color)
{
	renderColor_ = color;
}

sf::Color Particle::GetRenderColor() const { return renderColor_; }

Particle* Particle::SetRenderingShape(AParticleShape* shape)
{
	logicalRenderShape_ = shape;
	BakeRenderingShape();
	return this;
}

Particle* Particle::BakeRenderingShape()
{
	staticRenderShape_ = logicalRenderShape_->GenerateVertexShape(GetRenderColor());
	return this;
}

Particle* Particle::SetVelocity(const sf::Vector2f& velocity)
{
	velocity_ = velocity;
	return this;
}

float Particle::GetLifeTimeDoneValue() const
{
	return timerManager_->GetValueDone(lifeTimerHandle_);
}

void Particle::UpdateScaleOverLifetime(float lifeTime)
{
	SetScale(Maths::LerpClamped(initialScaleValue_,
							   initialScaleValue_,
							   lifeTime));
}

void Particle::UpdateColorOverLifetime(float lifeTime)
{
	renderColor_ = Maths::LerpColor(initialColor_, finalColor_, lifeTime);
}

bool Particle::GetIsLifetimeOver() const
{
	return timerManager_->GetValueDone(lifeTimerHandle_) >= 1.f;
}

Particle* Particle::SetLifeTime(const float lifetime)
{
	lifeTimerHandle_ = timerManager_->GetNewTimer();
	timerManager_->ResetTimer(lifeTimerHandle_, lifetime);
	return this;
}

void Particle::InitPoolable()
{
	SetIsInUse(true);

	if (lifeTimerHandle_ > 0)
	{
		timerManager_->Kill(lifeTimerHandle_);
	}
	lifeTimerHandle_ = -1;
	SetScale(Maths::OneVector());
	
	hasScaleOverLifetime_ = false;
	initialScaleValue_ = 0.f;
	finalScaleValue_ = 0.f;
	hasColorOverLifetime_ = false;
	initialColor_ = sf::Color::Black;
	finalColor_ = sf::Color::Black;
	lifeTimerHandle_ = 0;
	velocity_ = Maths::ZeroVector();
	logicalRenderShape_ = nullptr;
	staticRenderShape_.clear();
}

void Particle::SetColorOverLifeRange(const sf::Color& startColor, const sf::Color& endColor)
{
	initialColor_ = startColor;
	finalColor_ = endColor;
	hasColorOverLifetime_ = true;
}

void Particle::SetScaleOverLifeRange(float startScale, float endScale)
{
	initialScaleValue_ = startScale;
	finalScaleValue_ = endScale;
	hasScaleOverLifetime_ = true;
}