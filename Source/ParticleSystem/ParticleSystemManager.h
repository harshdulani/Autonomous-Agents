#pragma once

#include "Pooling/WeakPointerObjectPool.h"
#include "Particle.h"

class ParticleSystem;

class ParticleSystemManager
{
public:
	ParticleSystemManager();
	~ParticleSystemManager();

	std::weak_ptr<ParticleSystem> CreateNewParticleSystem(GameEntity* owningEntity);

	void UpdateAllParticleSystems(const float deltaTime);

	std::weak_ptr<Particle> GetParticleFromPool();
	void ReturnParticleToPool(std::weak_ptr<Particle> particle);

private:
	void CleanUpOldParticleSys();

	void RefreshParticlePool();

	uint32_t eventHandle_levelStart_ = 0;
	uint32_t eventHandle_levelEnd_ = 0;

	WeakPointerObjectPool<Particle> particlePool_;
	std::vector<std::weak_ptr<ParticleSystem>> activeSystems_;
};
 