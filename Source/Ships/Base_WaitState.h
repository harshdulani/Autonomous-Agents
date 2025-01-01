#pragma once

#include "Graph/StateNode.h"

class TimerManager;

class Base_WaitState : public StateNode
{
public:
	Base_WaitState();
	~Base_WaitState() override;

	void OnEnterState() override;
	void UpdateState(float deltaTime) override;

	void SetStateDuration(float duration);
	float GetStateDuration() const;

	virtual void OnTimerEnd() = 0;

protected:
	TimerManager* timerManager_ = nullptr;

	float waitDuration = -1.f;
	uint32_t waitDurationTimerHandle = 0;
};
