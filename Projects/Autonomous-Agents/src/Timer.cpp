#include "pch.h"
#include "Timer.h"
#include "System.h"
#include "TimerManager.h"

bool Timer::GetIsTimerRunning() const
{
	return bTimerRunning_;
}

void Timer::Play()
{
	bTimerRunning_ = true;
}

void Timer::Stop()
{
	timeElapsed_ = 0.f;
	bTimerRunning_ = false;
}

void Timer::Pause()
{
	bTimerRunning_ = false;
}

void Timer::UpdateTimer(const float deltaTime)
{
	if (!bTimerRunning_)
	{
		return;
	}
	if (timeElapsed_ == 0.f)
	{
		if (onStart_)
		{
			onStart_();
		}
	}

	timeElapsed_ += deltaTime;
	if (onUpdate_)
	{
		onUpdate_();
	}

	if (!IsTimerComplete())
	{
		return;
	}
	// pause timer at the end so no more playback here. will still call the OnComplete
	Pause();
	if (onComplete_)
	{
		onComplete_();
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
	return timeElapsed_ > lifeTime_;
}

void Timer::InitPoolable()
{
	SetIsInUse(true);
	lifeTime_ = 0;
	timeElapsed_ = 0;
	handle_ = -1;
	bKillOnComplete = true;
	bTimerRunning_ = true;
	onComplete_ = nullptr;
}

void Timer::ResetTimer(float newDuration)
{
	bTimerRunning_ = true;
	if (newDuration < 0)
	{
		SetTimerEnd(lifeTime_);
		return;
	}

	SetTimerEnd(newDuration);
}

void Timer::Kill()
{
	bTimerRunning_ = false;
}

void Timer::SetOnStart(const std::function<void()>& Call)
{
	onStart_ = Call;
}

void Timer::SetOnComplete(const std::function<void()>& Call)
{
	onComplete_ = Call;
}

void Timer::SetOnUpdate(const std::function<void()>& Call)
{
	onUpdate_ = Call;
}

void Timer::ClearOnStart()
{
	onStart_ = { };
}

void Timer::ClearOnUpdate()
{
	onUpdate_ = { };
}

void Timer::ClearOnComplete()
{
	onComplete_ = { };
}

void Timer::SetTimerEnd(float duration)
{
	lifeTime_ = duration;
	timeElapsed_ = 0.f;
}
