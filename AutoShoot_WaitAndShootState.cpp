#include "AutoShoot_WaitAndShootState.h"
#include <utility>
#include "../EnemyShip.h"
#include "Math.h"
#include "../Physics.h"
#include "../ShootingComponent.h"
#include "../TimerManager.h"

void AutoShoot_WaitAndShootState::OnEnterState()
{
	Base_WaitState::OnEnterState();
	bAiming_ = false;
}

void AutoShoot_WaitAndShootState::UpdateState(const float deltaTime)
{
	if (!bAiming_)
	{
		return;
	}
	if (TryTakeShot())
	{
		EndState();
		bAiming_ = false;
		return;
	}
}

void AutoShoot_WaitAndShootState::OnExitState()
{
	timerManager_->Stop(waitDurationTimerHandle_);
}

void AutoShoot_WaitAndShootState::InitialiseState(std::weak_ptr<GameEntity> target, const float raycastDistance)
{
	target_ = std::move(target);
	raycastDistance_ = raycastDistance;
}

void AutoShoot_WaitAndShootState::OnTimerEnd()
{
	bAiming_ = true;
}

void AutoShoot_WaitAndShootState::EndState()
{
	auto owner = std::dynamic_pointer_cast<EnemyShip>(entity_.lock());
	if (!owner)
	{
		return;
	}
	owner->SetAttacking(false);
	owner->AttackComplete();
}

bool AutoShoot_WaitAndShootState::TryTakeShot()
{
	auto target = target_.lock();
	auto owner = std::dynamic_pointer_cast<EnemyShip>(entity_.lock());
	if (!owner || !target ||
		!owner->GetActive() || owner->IsPendingKill())
	{
		return false;
	}

	auto pos = owner->GetPosition();
	auto lookVector = target->GetPosition() - pos;
	
	//check Line of Sight
	std::vector<std::weak_ptr<GameEntity>> hits;
	Physics::RaycastAll(pos, Math::LimitVector(lookVector, raycastDistance_), owner.get(), hits);

	bool bHit = false;
	for (auto& trial : hits)
	{
		if (trial.lock() == target)
		{
			bHit = true;
			break;
		}
	}
	
	if (!bHit)
		return false;
	
	//if Line of Sight is clear,
	//and all references are valid, found target
	
	if (auto shooter = owner->GetShootingComponent().lock())
	{
		auto angle = Math::RadsToDegs(atan2f(lookVector.y, lookVector.x)) + 90.f;
		
		shooter->SetWorldRotation(angle);

		shooter->ShootBullet();
		return true;
	}
	return false;
}