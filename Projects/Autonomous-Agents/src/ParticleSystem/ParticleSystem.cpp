#include "pch.h"
#include "ParticleSystem.h"
#include "../System.h"
#include "../ObjectManager.h"
#include "../Maths.h"
#include "AParticleShape.h"
#include "ParticleSystemManager.h"

ParticleSystem::ParticleSystem()
{
	particleSysMgr_ = System::GetInstance()->GetParticleSystemManager();
	SetEmissionShape(LogicalParticleShape::HLineSamplingFunction, 8);
	SetParticleShape(LogicalParticleShape::CircleSamplingFunction, 8);
}

void ParticleSystem::Update(const float deltaTime)
{
	// Delayed death
	if (READY_TO_DIE)
	{
		CheckForExpiredLifetimes();
		if (activeParticles_.empty())
		{
			Kill();
		}
		return;
	}

	if (isStopped_)
	{
		return;
	}
	// Regular behaviour
	if (!GetIsPlaying())
	{
		if (shouldAutoPlay_ && timeElapsed_ == 0.f)
		{
			// if not playing, start playing
			Play();
		}
		else
		{
			// if not playing, and shouldn't start playing, return
			return;
		}
	}
	timeElapsed_ += deltaTime;

	EmitterUpdate(deltaTime);
	BurstsUpdate();
	CheckForExpiredLifetimes();

	if (!hasCalledOnComplete_ && timeElapsed_ > totalDuration_ && activeParticles_.empty())
	{
		if (onComplete_)
		{
			onComplete_();
		}
		hasCalledOnComplete_ = true;
	}
}

void ParticleSystem::EmitterUpdate(const float deltaTime)
{
	if (ShouldEmit())
	{
		spawnElapsed_ += deltaTime;
		if (spawnElapsed_ > currentSpawnInterval_)
		{
			SpawnParticle();
			if (emitterRateMin_ > 0)
			{
				UpdateSpawnInterval(Math::GetRandFloat(emitterRateMin_, emitterRate_));
			}
			spawnElapsed_ = fmodf(spawnElapsed_, currentSpawnInterval_);
		}
	}
}

void ParticleSystem::BurstsUpdate()
{
	if (bursts_.empty())
	{
		return;
	}

	for (auto& burst : bursts_)
	{
		if (burst.didBurst)
		{
			continue;
		}
		// if didn't burst yet and
		if (timeElapsed_ > totalDuration_ * burst.durationValue)
		{
			int amt = burst.count;
			if (burst.countMin >= 0 && burst.count != burst.countMin)
			{
				amt = (int)Math::GetRandFloat((float)burst.countMin, (float)burst.count);
			}
			for (int i = 0; i < amt; i++)
			{
				SpawnParticle();
			}
			burst.didBurst = true;
			return;
		}
	}
}

void ParticleSystem::CheckForExpiredLifetimes()
{
	for (int i = 0; i < activeParticles_.size(); ++i)
	{
		if (activeParticles_[i].expired())
		{
			RemoveParticleAtIndex(i);
			// the object at the end of the list was brought at this index by destroy()
			// it also needs to be indexed
			--i;
			return;
		}
		auto particle = activeParticles_[i].lock();
		
		if (!particle->GetIsLifetimeOver())
		{
			continue;
		}
		//if waiting to die
		particle->SetPosition(Math::ZeroVector());
		particle->SetScale(Math::ZeroVector());
		particleSysMgr_->ReturnParticleToPool(activeParticles_[i]);
		RemoveParticleAtIndex(i);

		// the object at the end of the list was brought at this index by destroy()
		// it also needs to be indexed
		i--;
	}
}

void ParticleSystem::SpawnParticle()
{
	if (static_cast<int>(activeParticles_.size()) >= maxParticles_)
	{
		return;
	}
	auto particle = particleSysMgr_->GetParticleFromPool();
	activeParticles_.push_back(particle);
	sf::Vector2f pos;
	if (emissionMode_ == EmissionMode::Random)
	{
		if (shouldUseStaticEmitterShip_)
		{
			pos = staticEmitterShape_.GetRandomPointSampled();
		}
		else
		{
			pos = logicalEmitterShape_.GetRandomPointSampled();
		}
	}
	else //if(emissionMode_ == EmissionMode::LinearLooping)
	{
		if (shouldUseStaticEmitterShip_)
		{
			pos = staticEmitterShape_.GetPointSampled(timeElapsed_ - floorf(timeElapsed_));
		}
		else
		{
			pos = logicalEmitterShape_.GetPointSampled(timeElapsed_ - floorf(timeElapsed_));
		}
	}

	auto anchorPosition = GetWorldPosition() + GetOrigin();

	auto emitPos = pos;
	if (emitterRotationSpace_ == TransformSpace::Local)
	{
		emitPos = GetTotalTransform().transformPoint(pos);
		particle.lock()->SetPosition({emitPos.x, emitPos.y });
	}
	else
	{
		particle.lock()->SetPosition({anchorPosition.x + emitPos.x, anchorPosition.y + emitPos.y });		
	}
	InitialiseParticle(particle);
}

void ParticleSystem::InitialiseParticle(const std::weak_ptr<Particle>& particle)
{
	if (std::shared_ptr<Particle> p = particle.lock())
	{
		//assign all relevant values to this particle
		
		p->SetRenderPriority(particleRenderPriority_);
		//Lifetime
		if (particleLifetimeMax_ > 0.f)
		{
			p->SetLifeTime(Math::GetRandFloat(particleLifetime_, particleLifetimeMax_));
		}
		else
		{
			p->SetLifeTime(particleLifetime_);
		}
		if (particleScalingValue_ != 1.f)
		{
			p->SetScale(particleScalingValue_);
		}
		if (shouldScaleOverLifetime_)
		{
			p->SetScaleOverLifeRange(scaleOverLifetimeStart_, scaleOverLifetimeEnd_);
		}

		sf::Vector2f direction;
		switch (emissionVelocityMode_)
		{
			case EmissionVelocityMode::Outward:
				{
					sf::Vector2f pPos = p->GetPosition();
					sf::Vector2f mainPos = GetWorldPosition();
					direction = Math::Normalize(pPos - mainPos);
				}
				break;
			case EmissionVelocityMode::RightTangent:
				direction = GetRightVector();
				break;
			case EmissionVelocityMode::LeftTangent:
				direction = -GetRightVector();
				break;
			case EmissionVelocityMode::Normal:
				direction = GetForwardVector();
				break;
		}		

		float angle = Math::RadsToDegs(atan2f(direction.y, direction.x)) + 90.f;

		//@todo: this was just angle, maybe this should be set at the caller of this function where transform rotation space is selected
		p->SetRotation((emitterRotationSpace_ == TransformSpace::World ? GetWorldRotation() : GetLocalRotation()) + angle);

		sf::Vector2f velocity;
		if (particleSpeedMin_ > 0.f)
		{
			velocity = direction * Math::GetRandFloat(particleSpeedMin_, particleSpeed_);
		}
		else
		{
			velocity =  direction * particleSpeed_;
		}
		p->SetVelocity(velocity);


		if (particleRandomColorRangeTo_ != sf::Color::Black)
		{
			p->SetRenderColor(Math::LerpColor(particleRandomColorRangeFrom_,
												 particleRandomColorRangeTo_,
												 Math::GetRandFloat(0.f, 1.f)));
		}
		else
		{
			p->SetRenderColor(particleColor_);
		}
		if (shouldChangeColorOverLifetime_)
		{
			p->SetColorOverLifeRange(colorOverLifetimeFrom_, colorOverLifetimeTo_);
		}

		// because you need to bake shape w color
		if (shouldUseStaticParticleShip_)
		{
			p->SetRenderingShape(&staticParticleShape_);
		}
		else
		{
			p->SetRenderingShape(&logicalParticleShape_);
		}
	}
}

void ParticleSystem::Play()
{
	SetIsPlaying(true);
	isStopped_ = false;

	if (!hasCalledOnStart_)
	{
		if (onStart_)
		{
			onStart_();
			hasCalledOnStart_ = true;
		}
	}
	hasCalledOnComplete_ = false;
}

void ParticleSystem::Stop()
{
	SetIsPlaying(false);

	isStopped_ = true;
	hasCalledOnComplete_ = false;
	timeElapsed_ = 0.f;

	for (auto& particle : activeParticles_)
	{
		//if waiting to die
		particleSysMgr_->ReturnParticleToPool(particle);
	}
	for (auto& burst : bursts_)
	{
		burst.didBurst = false;
	}

	activeParticles_.clear();
}

bool ParticleSystem::GetShouldAutoPlay() const { return shouldAutoPlay_; }

void ParticleSystem::Kill()
{
	if (READY_TO_DIE)
	{
		SceneComponent::Kill();
		return;
	}

	READY_TO_DIE = true;
}

ParticleSystem& ParticleSystem::SetAutoPlay(bool val)
{
	shouldAutoPlay_ = val;
	return *this;
}

ParticleSystem& ParticleSystem::SetOnStart(const std::function<void()>& onStart)
{
	onStart_ = onStart;
	return *this;
}

ParticleSystem& ParticleSystem::SetOnComplete(const std::function<void()>& onComplete)
{
	onComplete_ = onComplete;
	return *this;
}

ParticleSystem& ParticleSystem::SetRenderPriority(const int priority)
{
	particleRenderPriority_ = priority;
	return *this;
}

bool ParticleSystem::ShouldEmit() const
{
	// if  shouldLoop is true, def emit
	// else only if lifetime is not over
	return shouldLoop_ || timeElapsed_ < totalDuration_;
}

std::vector<std::weak_ptr<Particle>>& ParticleSystem::GetActiveParticles()
{
	return activeParticles_;
}

void ParticleSystem::RemoveParticleAtIndex(int index)
{
	const int ParticleNum = static_cast<int>(activeParticles_.size());
	if (index >= ParticleNum || index < 0)
	{
		return;
	}

	// DEFRAGMENTATION of vector by filling in gaps
	if (index == ParticleNum - 1)
	{
		// there is no need to fill any gap if the object is at the end of the list
		// just make sure that element is not accessed anymore
		activeParticles_.pop_back();
		return;
	}

	// move pointer to last object stored in list to the new emptied index
	std::iter_swap((activeParticles_.begin() + index),
				   (activeParticles_.begin() + ParticleNum - 1));
	activeParticles_.pop_back();
}

void ParticleSystem::UpdateSpawnInterval(float rate)
{
	currentSpawnInterval_ = 1.f / rate;
}

ParticleSystem& ParticleSystem::SetDuration(const float seconds)
{
	totalDuration_ = seconds;
	return *this;
}

ParticleSystem& ParticleSystem::SetParticleLifeTime(const float seconds, const float secondsMax)
{
	particleLifetime_ = seconds;
	particleLifetimeMax_ = secondsMax;
	return *this;
}

ParticleSystem& ParticleSystem::SetLooping(const bool status)
{
	shouldLoop_ = status;
	return *this;
}

ParticleSystem& ParticleSystem::SetMaxParticles(const int count)
{
	maxParticles_ = count;
	return *this;
}

ParticleSystem& ParticleSystem::SetEmitterScaling(const float scalingValue)
{
	staticEmitterShape_.SetShapeScale(scalingValue);
	logicalEmitterShape_.SetShapeScale(scalingValue);
	return *this;
}

ParticleSystem& ParticleSystem::SetEmissionMode(EmissionMode mode)
{
	emissionMode_ = mode;
	return *this;
}

ParticleSystem& ParticleSystem::SetEmitterRate(const float emitterRate, const float emitterRateMin)
{
	emitterRate_ = emitterRate;
	emitterRateMin_ = emitterRateMin;
	if (emitterRate_ > 0.f)
	{
		UpdateSpawnInterval(emitterRate_);
	}
	return *this;
}

ParticleSystem& ParticleSystem::AddEmitterBurst(const float durationValue, const int count, const int countMin /*= -1*/)
{
	bursts_.emplace_back(count, countMin, durationValue);
	return *this;
}

ParticleSystem& ParticleSystem::SetEmissionSpeed(const float particleSpeed, const float particleSpeedMin)
{
	particleSpeed_ = particleSpeed;
	particleSpeedMin_ = particleSpeedMin;
	return *this;
}

ParticleSystem& ParticleSystem::SetEmitterRotationMode(TransformSpace rotation)
{
	emitterRotationSpace_ = rotation;
	return *this;
}

ParticleSystem& ParticleSystem::SetEmissionVelocityMode(EmissionVelocityMode val)
{
	emissionVelocityMode_ = val;
	return *this;
}

ParticleSystem& ParticleSystem::SetParticleShape(const std::vector<sf::Vector2f>& vertices,
												 int resolution)
{
	shouldUseStaticParticleShip_ = true;
	staticParticleShape_.SetStaticShape(vertices);
	staticParticleShape_.SetShapeResolution(resolution);
	//logicalParticleShape_.SetShapeResolution(resolution);
	return *this;
}

ParticleSystem& ParticleSystem::SetParticleShape(const std::function<sf::Vector2f(float)>& shapeFunction,
												 int resolution)
{
	shouldUseStaticParticleShip_ = false;
	logicalParticleShape_.SetSamplingFunction(shapeFunction);
	//staticParticleShape_.SetShapeResolution(resolution);
	logicalParticleShape_.SetShapeResolution(resolution);
	return *this;
}

ParticleSystem& ParticleSystem::SetEmissionShape(const std::vector<sf::Vector2f>& vertices,
												 int resolution)
{
	shouldUseStaticEmitterShip_ = true;
	staticEmitterShape_.SetStaticShape(vertices);
	staticEmitterShape_.SetShapeResolution(resolution);
	//logicalEmitterShape_.SetShapeResolution(resolution);
	return *this;
}

ParticleSystem& ParticleSystem::SetEmissionShape(const std::function<sf::Vector2f(float)>& shapeFunction,
												 int resolution)
{
	shouldUseStaticEmitterShip_ = false;
	logicalEmitterShape_.SetSamplingFunction(shapeFunction);
	//staticEmitterShape_.SetShapeResolution(resolution);
	logicalEmitterShape_.SetShapeResolution(resolution);
	return *this;
}

ParticleSystem& ParticleSystem::SetParticleScale(const float scale, const float scaleMin)
{
	particleScalingValue_ = scale;
	particleScalingValueMin_ = scaleMin;
	return *this;
}

ParticleSystem& ParticleSystem::SetScaleOverLifetime(const float initialScale,
													 const float finalScale)
{
	shouldScaleOverLifetime_ = true;
	scaleOverLifetimeStart_ = initialScale;
	scaleOverLifetimeEnd_ = finalScale;
	return *this;
}

ParticleSystem& ParticleSystem::SetParticleColor(const sf::Color& color)
{
	particleColor_ = color;
	return *this;
}

ParticleSystem& ParticleSystem::SetParticleColor(const sf::Color& randomBetweenColorA,
												 const sf::Color& randomBetweenColorB)
{
	particleRandomColorRangeFrom_ = randomBetweenColorA;
	particleRandomColorRangeTo_ = randomBetweenColorB;
	return *this;
}

ParticleSystem& ParticleSystem::SetColorOverLifeTime(const sf::Color& startColor,
													 const sf::Color& endColor)
{
	shouldChangeColorOverLifetime_ = true;
	colorOverLifetimeFrom_ = startColor;
	colorOverLifetimeTo_ = endColor;
	return *this;
}