#pragma once

#include <list>
#include <functional>

class Timer;

class TimerManager
{
public:
	TimerManager();

	~TimerManager();

	void Kill(int TimerHandle);

	void Play(int TimerHandle);
	void Pause(int TimerHandle);
	void Stop(int TimerHandle);

	void SetTimerEnd(int TimerHandle, float Duration);
	void ResetTimer(int TimerHandle, float NewDuration = -1.0f);

	void SetOnStart(int TimerHandle, const std::function<void()>& call);
	void SetOnComplete(int TimerHandle, const std::function<void()>& call);
	void SetOnUpdate(int TimerHandle, const std::function<void()>& call);

	void ClearOnStart(int TimerHandle);
	void ClearOnUpdate(int TimerHandle);
	void ClearOnComplete(int TimerHandle);
	void SetKillOnComplete(int TimerHandle, bool ShouldKillOnComplete);

	bool IsTimerRunning(int TimerHandle);
	float GetTimeSinceSpawn(int TimerHandle);
	float GetValueDone(int TimerHandle);

private:
	Timer* FindTimer(int TimerHandle);

	static int LastTimerHandle;

public:
	int GetNewTimer();
	void UpdateTimers(float DeltaTime);

private:
	std::list<Timer> TimerList;
};