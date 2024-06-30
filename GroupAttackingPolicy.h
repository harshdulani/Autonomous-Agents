#pragma once

#include "Object.h"
#include <vector>
#include <memory>
#include <random>

class GameEntity;
class EnemyShip;
class TimerManager;

class GroupAttackingPolicy : public Object
{
public:
	GroupAttackingPolicy();
	~GroupAttackingPolicy() override;

	void AttackComplete();

private:
	void StartTryAttacking();

	void OnEntityCreated(std::weak_ptr<GameEntity> entity);
	void OnEntityDestroyed(std::weak_ptr<GameEntity> entity);
	void CleanUpOldBots();
	void StartGlobalAttacking();
	
	TimerManager* GetTimerMgr() { return timerMgr_; }
	TimerManager* timerMgr_ = nullptr;

	float shootInterval_ = 2.f;

	std::vector<std::weak_ptr<EnemyShip>> shootQueue_;

	std::default_random_engine queueRandomiser_;

	uint32_t timerHandle_TryShootWait_ = 0;
	uint32_t eventHandle_EntityCreated_ = 0;
	uint32_t eventHandle_EntityDestroyed_ = 0;
	//uint32_t eventHandle_LevelStart_ = 0;
	uint32_t eventHandle_LevelEnd_ = 0;
};
