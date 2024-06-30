#pragma once

#include <list>
#include <functional>

class Timer;

class TimerManager
{
public:
	TimerManager();

	~TimerManager();

	void Kill(uint32_t TimerHandle);

	void Play(uint32_t TimerHandle);
	void Pause(uint32_t TimerHandle);
	void Stop(uint32_t TimerHandle);

	void SetTimerEnd(uint32_t TimerHandle, float Duration);
	void ResetTimer(uint32_t TimerHandle, float NewDuration = -1.0f);

	void SetOnStart(uint32_t TimerHandle, const std::function<void()>& call);
	void SetOnComplete(uint32_t TimerHandle, const std::function<void()>& call);
	void SetOnUpdate(uint32_t TimerHandle, const std::function<void()>& call);

	void ClearOnStart(uint32_t TimerHandle);
	void ClearOnUpdate(uint32_t TimerHandle);
	void ClearOnComplete(uint32_t TimerHandle);
	void SetKillOnComplete(uint32_t TimerHandle, bool ShouldKillOnComplete);

	bool IsTimerRunning(uint32_t TimerHandle);
	float GetTimeSinceSpawn(uint32_t TimerHandle);
	float GetValueDone(uint32_t TimerHandle);

private:
	Timer* FindTimer(uint32_t TimerHandle);

	static uint32_t LastTimerHandle;

public:
	uint32_t GetNewTimer();
	void UpdateTimers(float DeltaTime);

private:
	std::list<Timer> TimerList;
};