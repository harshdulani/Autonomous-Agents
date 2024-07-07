#include "Enemy_RevolveState.h"
#include "EnemyShip.h"
#include "Math.h"
#include "FSM.h"
#include "Debug.h"

void Enemy_RevolveState::UpdateState(const float deltaTime)
{
	auto ownerShip = owner_.lock();
	if (!ownerShip || ownerShip->GetTarget().expired())
	{
		return;
	}
	auto targ = ownerShip->GetTarget().lock();

	auto targetPos = targ->GetPosition();
	auto diff = ownerShip->GetPosition() - targetPos;
	auto normDiff = Math::Normalize(diff);

	float angle = atan2f(normDiff.y, normDiff.x) - Math::PI * 0.5f;
	angle -= deltaAngle_ * deltaTime;
	sf::Vector2f offset = sf::Vector2f(cosf(angle), sinf(angle)) * pathRadius_;

	ownerShip->SetSeekLocation(targetPos + offset);

	if (Math::GetVectorMagnitude(diff) > minDist_)
	{
		if (auto fsm = ownerShip->GetChaseFSM().lock())
		{
			fsm->SetParameter("shouldRevolve", false);
		}
	}
}

void Enemy_RevolveState::InitialiseState(const float minDistance, const float pathRadius, const float deltaAngle)
{
	owner_ = std::dynamic_pointer_cast<EnemyShip>(entity_.lock());
	minDist_ = minDistance;
	pathRadius_ = pathRadius;
	deltaAngle_ = deltaAngle;
}
