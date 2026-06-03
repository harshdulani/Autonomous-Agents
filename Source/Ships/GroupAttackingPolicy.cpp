#include "GroupAttackingPolicy.h"
#include "System.h"
#include "TimerManager.h"
#include "EnemyShip.h"
#include "Debug.h"
#include <algorithm>

GroupAttackingPolicy::GroupAttackingPolicy()
{
	timerMgr_ = System::GetInstance()->GetTimerManager();

	// try shoot interval
	timerHandle_TryShootWait_ = GetTimerMgr()->GetNewTimer();

	GetTimerMgr()->SetTimerEnd(timerHandle_TryShootWait_, shootInterval_);
	GetTimerMgr()->SetKillOnComplete(timerHandle_TryShootWait_, false);
	GetTimerMgr()->SetOnComplete(timerHandle_TryShootWait_, 
								 std::bind(&GroupAttackingPolicy::StartTryAttacking, this));
	GetTimerMgr()->Pause(timerHandle_TryShootWait_);

	queueRandomiser_ = std::default_random_engine { };

	// event listeners
	eventHandle_EnemyShipCreated_ = System::GetInstance()->GetGame().Event_EnemyShipCreated
		.Subscribe(BindSubscriber(&GroupAttackingPolicy::OnEnemyShipCreated, this));

	eventHandle_EnemyShipDestroyed_ = System::GetInstance()->GetGame().Event_EnemyShipDestroyed
		.Subscribe(BindSubscriber(&GroupAttackingPolicy::OnEnemyShipDestroyed, this));

	eventHandle_LevelStart_ = System::GetInstance()->GetGame().Event_LevelStart
		.Subscribe(BindSubscriber(&GroupAttackingPolicy::StartGlobalAttacking, this));

	eventHandle_LevelEnd_ = System::GetInstance()->GetGame().Event_ShutdownGame
		.Subscribe(BindSubscriber(&GroupAttackingPolicy::CleanUpOldBots, this));
}

GroupAttackingPolicy::~GroupAttackingPolicy()
{
	GetTimerMgr()->SetKillOnComplete(timerHandle_TryShootWait_, true);
	GetTimerMgr()->ClearOnComplete(timerHandle_TryShootWait_);

	System::GetInstance()->GetGame().Event_EnemyShipCreated.Unsubscribe(eventHandle_EnemyShipCreated_);
	System::GetInstance()->GetGame().Event_EnemyShipDestroyed.Unsubscribe(eventHandle_EnemyShipDestroyed_);
	System::GetInstance()->GetGame().Event_LevelStart.Unsubscribe(eventHandle_LevelStart_);
	System::GetInstance()->GetGame().Event_ShutdownGame.Unsubscribe(eventHandle_LevelEnd_);
}

void GroupAttackingPolicy::StartTryAttacking()
{
	if (shootQueue_.empty())
	{
		return;
	}
	auto attacker = shootQueue_[0].lock();
	if (!attacker)
	{
		return;
	}
	attacker->SetAttacking(true);
}

void GroupAttackingPolicy::AttackComplete()
{
	if (shootQueue_.empty())
	{
		return;
	}
	shootQueue_.push_back(shootQueue_[0]);
	int size = static_cast<int>(shootQueue_.size()) - 2;
	if (size > 0) // if we have more than one ship in the rest of the queue
	{
		std::shuffle(shootQueue_.begin(), shootQueue_.begin() + size, queueRandomiser_);
	}
	shootQueue_.erase(shootQueue_.begin());
	GetTimerMgr()->ResetTimer(timerHandle_TryShootWait_);
}

void GroupAttackingPolicy::OnEnemyShipCreated(const std::weak_ptr<EnemyShip>& enemyShip)
{
	if (auto ship = enemyShip.lock())
	{
		bool wasEmpty = shootQueue_.empty();
		shootQueue_.push_back(ship);
		if (wasEmpty)
		{
			StartGlobalAttacking();
		}
	}
}

void GroupAttackingPolicy::OnEnemyShipDestroyed(const std::weak_ptr<EnemyShip>& enemyShip)
{
	if (auto ship = enemyShip.lock())
	{
		auto it = std::find_if(shootQueue_.begin(),
							   shootQueue_.end(),
							   [&ship](const std::weak_ptr<EnemyShip>& item) -> bool
							   {
								   auto locked = item.lock();
								   return locked && locked == ship;
							   });
		if (it == shootQueue_.end())
		{
			return;
		}
		shootQueue_.erase(it);
		if (shootQueue_.empty())
		{
			GetTimerMgr()->Pause(timerHandle_TryShootWait_);
		}
	}
}

void GroupAttackingPolicy::CleanUpOldBots()
{
	shootQueue_.clear();
	GetTimerMgr()->Pause(timerHandle_TryShootWait_);
}

void GroupAttackingPolicy::StartGlobalAttacking()
{
	GetTimerMgr()->Play(timerHandle_TryShootWait_);
}