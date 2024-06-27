#include "TimerManager.h"
#include "Timer.h"
#include "System.h"

TimerManager::TimerManager() = default;

TimerManager::~TimerManager()
{
	TimerList.clear();
}

int TimerManager::LastTimerHandle = 0;

void TimerManager::Kill(int TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->Kill();
		TimerList.remove(*timer);
	}
}

void TimerManager::Play(int TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->Play();
	}
}

void TimerManager::Pause(int TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->Pause();
	}
}

void TimerManager::Stop(int TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->Stop();
	}
}

void TimerManager::SetTimerEnd(int TimerHandle, float Duration)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->SetTimerEnd(Duration);
	}
}

void TimerManager::ResetTimer(int TimerHandle, float NewDuration /*= -1.0f*/)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->ResetTimer(NewDuration);
	}
}

void TimerManager::SetOnStart(int TimerHandle, const std::function<void()>& call)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->SetOnStart(call);
	}
}

void TimerManager::SetOnComplete(int TimerHandle, const std::function<void()>& call)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->SetOnComplete(call);
	}
}

void TimerManager::SetOnUpdate(int TimerHandle, const std::function<void()>& call)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->SetOnUpdate(call);
	}
}

void TimerManager::ClearOnStart(int TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->ClearOnStart();
	}
}

void TimerManager::ClearOnComplete(int TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->ClearOnComplete();
	}
}

void TimerManager::ClearOnUpdate(int TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->ClearOnUpdate();
	}
}

void TimerManager::SetKillOnComplete(int TimerHandle, bool ShouldKillOnComplete)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->bKillOnComplete = ShouldKillOnComplete;
	}
}

bool TimerManager::IsTimerRunning(const int TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		return timer->GetIsTimerRunning();
	}
	return false;
}

float TimerManager::GetTimeSinceSpawn(int TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		return timer->TimeSinceSpawn();
	}
	return -1.0f;
}

float TimerManager::GetValueDone(int TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		return timer->GetValueDone();
	}
	return -1.0f;
}


Timer* TimerManager::FindTimer(int TimerHandle)
{
	for (auto& timer : TimerList)
	{
		if (timer.Handle == TimerHandle)
			return &timer;
	}
	return nullptr;
}

int TimerManager::GetNewTimer()
{
	Timer newTimer;
	newTimer.Handle = LastTimerHandle;

	TimerList.push_back(newTimer);

	return LastTimerHandle++;
}

void TimerManager::UpdateTimers(const float DeltaTime)
{
	if (TimerList.empty())
	{
		return;
	}

	//reverse for
	for (auto it = TimerList.rbegin(); it != TimerList.rend(); ++it)
	{
		(it)->UpdateTimer(DeltaTime);
	}
}