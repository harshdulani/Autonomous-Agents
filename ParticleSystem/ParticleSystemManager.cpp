#include "ParticleSystemManager.h"
#include <memory>
#include "ParticleSystem.h"
#include "../GameEntity.h"
#include "../WeakPointerObjectPool.h"
#include "../System.h"
#include "../ObjectManager.h"

ParticleSystemManager::ParticleSystemManager()
{
	std::function<std::weak_ptr<Particle>()> creation = []() -> std::weak_ptr<Particle>
		{
			return System::GetInstance()->GetObjectMgr()->CreateGameEntity<Particle>(Math::ZeroVector());
		};
	particlePool_.SetObjectCreationFunction(creation);

	eventHandle_levelStart_ = System::GetInstance()->
		Event_LevelStart.Subscribe(BindSubscriber(&ParticleSystemManager::RefreshParticlePool, this));

	eventHandle_levelEnd_ = System::GetInstance()->
		Event_LevelEnd.Subscribe(BindSubscriber(&ParticleSystemManager::CleanUpOldParticleSys, this));
}

ParticleSystemManager::~ParticleSystemManager()
{
	System::GetInstance()->Event_LevelStart.Unsubscribe(eventHandle_levelStart_);
	System::GetInstance()->Event_LevelEnd.Unsubscribe(eventHandle_levelEnd_);
}

std::weak_ptr<ParticleSystem> ParticleSystemManager::CreateNewParticleSystem(GameEntity* owningEntity)
{
	auto pSys = owningEntity->AddComponent<ParticleSystem>();
	
	activeSystems_.push_back(pSys);
	return pSys;
}

void ParticleSystemManager::UpdateAllParticleSystems(const float deltaTime)
{
	if (activeSystems_.empty())
	{
		return;
	}
	for (auto& item : activeSystems_)
	{
		if (auto pSys = item.lock())
		{
			if (!pSys->GetIsPlaying() && !pSys->GetShouldAutoPlay() && !pSys->IsEnabled())
			{
				return;
			}
			pSys->Update(deltaTime);
		}
	}
}

std::weak_ptr<Particle> ParticleSystemManager::GetParticleFromPool()
{
	return particlePool_.GetFromPool();
}

void ParticleSystemManager::ReturnParticleToPool(std::weak_ptr<Particle> particle)
{
	particlePool_.ReturnToPool(particle);
}

void ParticleSystemManager::CleanUpOldParticleSys()
{
	activeSystems_.clear();
}

void ParticleSystemManager::RefreshParticlePool()
{
	particlePool_.AddElements(100);
}
