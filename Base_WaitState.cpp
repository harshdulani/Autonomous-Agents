#include "Base_WaitState.h"
#include "TimerManager.h"
#include "System.h"

Base_WaitState::Base_WaitState()
{
	timerManager = System::GetInstance()->GetTimerManager();
}

Base_WaitState::~Base_WaitState()
{
	timerManager->SetKillOnComplete(waitDurationTimerHandle, true);
	timerManager->ClearOnComplete(waitDurationTimerHandle);
}

void Base_WaitState::OnEnterState()
{
	if (waitDurationTimerHandle == 0)
	{
		waitDurationTimerHandle = timerManager->GetNewTimer();

		timerManager->SetTimerEnd(waitDurationTimerHandle, waitDuration);
		timerManager->SetKillOnComplete(waitDurationTimerHandle, false);
		timerManager->SetOnComplete(waitDurationTimerHandle, [this] { OnTimerEnd(); });
		return;
	}

	timerManager->ResetTimer(waitDurationTimerHandle);
}

void Base_WaitState::UpdateState(const float deltaTime)
{

}

void Base_WaitState::SetStateDuration(const float duration)
{
	waitDuration = duration;
	
	if (waitDurationTimerHandle != -1)
	{
		timerManager->SetTimerEnd(waitDurationTimerHandle, duration);
	}
}

float Base_WaitState::GetStateDuration() const
{
	return waitDuration;
}
