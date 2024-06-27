#include "Timer.h"
#include "System.h"
#include "TimerManager.h"

bool Timer::GetIsTimerRunning() const
{
	return bTimerRunning;
}

void Timer::Play()
{
	bTimerRunning = true;
}

void Timer::Stop()
{
	TimeElapsed = 0.f;
	bTimerRunning = false;
}

void Timer::Pause()
{
	bTimerRunning = false;
}

void Timer::UpdateTimer(const float deltaTime)
{
	if (!bTimerRunning)
	{
		return;
	}
	if (TimeElapsed == 0.f)
	{
		if (onStart_)
		{
			onStart_();
		}
	}

	TimeElapsed += deltaTime;
	if (OnUpdate)
	{
		OnUpdate();
	}

	if (!IsTimerComplete())
	{
		return;
	}
	// pause timer at the end so no more playback here. will still call the OnComplete
	Pause();
	if (OnComplete)
	{
		OnComplete();
	}

	if (!bKillOnComplete)
	{
		return;
	}

	Kill();
}

// check memory addresses to confirm if they are the same instance or not
bool Timer::operator==(const Timer& rhs) const { return this == &rhs; }

bool Timer::IsTimerComplete() const
{
	return TimeElapsed > LifeTime;
}

void Timer::InitPoolable()
{
	SetIsInUse(true);
	LifeTime = 0;
	TimeElapsed = 0;
	Handle = -1;
	bKillOnComplete = true;
	bTimerRunning = true;
	OnComplete = nullptr;
}

void Timer::ResetTimer(float newDuration)
{
	bTimerRunning = true;
	if (newDuration < 0)
	{
		SetTimerEnd(LifeTime);
		return;
	}

	SetTimerEnd(newDuration);
}

void Timer::Kill()
{
	bTimerRunning = false;
}

void Timer::SetOnStart(const std::function<void()>& Call)
{
	onStart_ = Call;
}

void Timer::SetOnComplete(const std::function<void()>& Call)
{
	OnComplete = Call;
}

void Timer::SetOnUpdate(const std::function<void()>& Call)
{
	OnUpdate = Call;
}

void Timer::ClearOnStart()
{
	onStart_ = { };
}

void Timer::ClearOnUpdate()
{
	OnUpdate = { };
}

void Timer::ClearOnComplete()
{
	OnComplete = { };
}

void Timer::SetTimerEnd(float duration)
{
	LifeTime = duration;
	TimeElapsed = 0.f;
}
