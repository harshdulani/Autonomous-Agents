#include "pch.h"
#include "Base_WaitState.h"
#include "TimerManager.h"
#include "System.h"

Base_WaitState::Base_WaitState()
{
	timerManager_ = System::GetInstance()->GetTimerManager();
}

Base_WaitState::~Base_WaitState()
{
	timerManager_->SetKillOnComplete(waitDurationTimerHandle, true);
	timerManager_->ClearOnComplete(waitDurationTimerHandle);
}

void Base_WaitState::OnEnterState()
{
	if (waitDurationTimerHandle == 0)
	{
		waitDurationTimerHandle = timerManager_->GetNewTimer();

		timerManager_->SetTimerEnd(waitDurationTimerHandle, waitDuration);
		timerManager_->SetKillOnComplete(waitDurationTimerHandle, false);
		timerManager_->SetOnComplete(waitDurationTimerHandle, [this] { OnTimerEnd(); });
		return;
	}

	timerManager_->ResetTimer(waitDurationTimerHandle);
}

void Base_WaitState::UpdateState(const float deltaTime)
{

}

void Base_WaitState::SetStateDuration(const float duration)
{
	waitDuration = duration;
	
	if (waitDurationTimerHandle > 0)
	{
		timerManager_->SetTimerEnd(waitDurationTimerHandle, duration);
	}
}

float Base_WaitState::GetStateDuration() const
{
	return waitDuration;
}
