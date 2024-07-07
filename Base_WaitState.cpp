#include "Base_WaitState.h"
#include "TimerManager.h"
#include "System.h"

Base_WaitState::Base_WaitState()
{
	timerManager_ = System::GetInstance()->GetTimerManager();
}

Base_WaitState::~Base_WaitState()
{
	timerManager_->SetKillOnComplete(waitDurationTimerHandle_, true);
	timerManager_->ClearOnComplete(waitDurationTimerHandle_);
}

void Base_WaitState::OnEnterState()
{
	if (waitDurationTimerHandle_ == 0)
	{
		waitDurationTimerHandle_ = timerManager_->GetNewTimer();

		timerManager_->SetTimerEnd(waitDurationTimerHandle_, waitDuration_);
		timerManager_->SetKillOnComplete(waitDurationTimerHandle_, false);
		timerManager_->SetOnComplete(waitDurationTimerHandle_, [this] { OnTimerEnd(); });
		return;
	}

	timerManager_->ResetTimer(waitDurationTimerHandle_);
}

void Base_WaitState::UpdateState(const float deltaTime)
{

}

void Base_WaitState::SetStateDuration(const float duration)
{
	waitDuration_ = duration;
	
	if (waitDurationTimerHandle_ != -1)
	{
		timerManager_->SetTimerEnd(waitDurationTimerHandle_, duration);
	}
}

float Base_WaitState::GetStateDuration() const
{
	return waitDuration_;
}
