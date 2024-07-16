#include "pch.h"
#include "Enemy_ChaseState.h"
#include "GameEntity.h"
#include "EnemyShip.h"
#include "Maths.h"
#include "Debug.h"
#include "FSM.h"

void Enemy_ChaseState::UpdateState(const float deltaTime)
{
	auto ownerShip = owner_.lock();
	if (!ownerShip || ownerShip->GetTarget().expired())
	{
		return;
	}
	auto targ = ownerShip->GetTarget().lock();

	auto targetPos = targ->GetPosition();
	auto diff = ownerShip->GetPosition() - targetPos;
	auto offset = Math::Normalize(diff) * minDist_;

	ownerShip->SetSeekLocation(targetPos + offset);
	
	if (Math::GetVectorMagnitude(diff) < minDist_ + 2.5f)
	{
		if (auto fsm = ownerShip->GetChaseFSM().lock())
		{
			fsm->SetParameter("shouldRevolve", true);
		}
	}
}

void Enemy_ChaseState::InitialiseState(const float minDistance)
{
	owner_ = std::dynamic_pointer_cast<EnemyShip>(entity.lock());
	minDist_ = minDistance;
}
