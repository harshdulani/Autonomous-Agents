#include "TimerManager.h"
#include "Timer.h"
#include "System.h"

TimerManager::TimerManager() = default;

TimerManager::~TimerManager()
{
	timerList_.clear();
}

uint32_t TimerManager::lastTimerHandle_ = 1;

void TimerManager::Kill(uint32_t TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->Kill();
		timerList_.remove(*timer);
	}
}

void TimerManager::Play(uint32_t TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->Play();
	}
}

void TimerManager::Pause(uint32_t TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->Pause();
	}
}

void TimerManager::Stop(uint32_t TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->Stop();
	}
}

void TimerManager::SetTimerEnd(uint32_t TimerHandle, float Duration)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->SetTimerEnd(Duration);
	}
}

void TimerManager::ResetTimer(uint32_t TimerHandle, float NewDuration /*= -1.0f*/)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->ResetTimer(NewDuration);
	}
}

void TimerManager::SetOnStart(uint32_t TimerHandle, const std::function<void()>& call)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->SetOnStart(call);
	}
}

void TimerManager::SetOnComplete(uint32_t TimerHandle, const std::function<void()>& call)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->SetOnComplete(call);
	}
}

void TimerManager::SetOnUpdate(uint32_t TimerHandle, const std::function<void()>& call)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->SetOnUpdate(call);
	}
}

void TimerManager::ClearOnStart(uint32_t TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->ClearOnStart();
	}
}

void TimerManager::ClearOnComplete(uint32_t TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->ClearOnComplete();
	}
}

void TimerManager::ClearOnUpdate(uint32_t TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->ClearOnUpdate();
	}
}

void TimerManager::SetKillOnComplete(uint32_t TimerHandle, bool ShouldKillOnComplete)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		timer->bKillOnComplete = ShouldKillOnComplete;
	}
}

bool TimerManager::IsTimerRunning(const uint32_t TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		return timer->GetIsTimerRunning();
	}
	return false;
}

float TimerManager::GetTimeSinceSpawn(uint32_t TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		return timer->TimeSinceSpawn();
	}
	return -1.0f;
}

float TimerManager::GetValueDone(uint32_t TimerHandle)
{
	if (auto timer = FindTimer(TimerHandle))
	{
		return timer->GetValueDone();
	}
	return -1.0f;
}


Timer* TimerManager::FindTimer(uint32_t TimerHandle)
{
	for (auto& timer : timerList_)
	{
		if (timer.handle_ == TimerHandle)
			return &timer;
	}
	return nullptr;
}

uint32_t TimerManager::GetNewTimer()
{
	Timer newTimer;
	newTimer.handle_ = lastTimerHandle_;

	timerList_.push_back(newTimer);

	return lastTimerHandle_++;
}

void TimerManager::UpdateTimers(const float DeltaTime)
{
	if (timerList_.empty())
	{
		return;
	}

	//reverse for
	for (auto it = timerList_.rbegin(); it != timerList_.rend(); ++it)
	{
		(it)->UpdateTimer(DeltaTime);
	}
}