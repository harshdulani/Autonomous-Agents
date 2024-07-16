#pragma once

#include "Base_WaitState.h"

class AutoShoot_WaitAndShootState : public Base_WaitState
{
public:
	void OnEnterState() override;
	void UpdateState(const float deltaTime) override;
	void OnExitState() override;

	void OnTimerEnd() override;
	void InitialiseState(std::weak_ptr<GameEntity> target, const float raycastDistance);
	void EndState();

private:
	/// Returns if successfully took the shot
	bool TryTakeShot();

	float raycastDistance_ = 0.f;
	bool bAiming_ = false;
	std::weak_ptr<GameEntity> target_;
};