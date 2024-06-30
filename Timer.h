#pragma once

#include "APoolable.h"
#include <functional>

class Timer : public APoolable
{
	friend class TimerManager;
public:
	bool operator==(const Timer& rhs) const;
	Timer() = default;

private:
	bool GetIsTimerRunning() const;

	// Starts timer playback. Does nothing if timer already running.
	void Play();

	// Stops timer. Will start from beginning if Play() called after this. 
	// Does nothing if timer already stopped.
	void Stop();

	// Pauses timer. Will start from same place if Play() called after this. 
	// Does nothing if timer already paused.
	void Pause();

	void UpdateTimer(const float deltaTime);

	void SetTimerEnd(float duration);
	void ResetTimer(float newDuration = -1.0f);

	// Kill the timer without letting it complete and calling onComplete_
	void Kill();

	// If set to true (default), timer will be deleted after running once.
	bool bKillOnComplete = true;

	/// <summary>
	/// Use to Assign function to be called at the start of the timer, loops also.
	/// Use lambda function if you want a parameterized callback.
	/// </summary>
	/// <param name="callback">must be of "void Foo()" signature</param>
	void SetOnStart(const std::function<void()>& Call);

	/// <summary>
	/// Use to Assign function to be called after timer lifetime.
	/// Use lambda function if you want a parameterized callback.
	/// </summary>
	/// <param name="callback">must be of "void Foo()" signature</param>
	void SetOnComplete(const std::function<void()>& Call);

	void SetOnUpdate(const std::function<void()>& Call);
	
	void ClearOnStart();
	void ClearOnUpdate();
	void ClearOnComplete();

	float TimeSinceSpawn() const { return TimeElapsed; }
	float GetValueDone() const { return TimeSinceSpawn() / LifeTime; }
	bool IsTimerComplete() const;

	/// <summary>
	/// Use to call the function to be called at the start of the timer
	/// </summary>
	std::function<void()> onStart_;
	/// <summary>
	/// Use to call the function to be called at the end of the timer
	/// </summary>
	std::function<void()> OnComplete = nullptr;
	std::function<void()> OnUpdate = nullptr;

	uint32_t Handle = 0;

	//calculated in milliseconds
	float LifeTime = 0;
	float TimeElapsed = 0;

	bool bTimerRunning = true;
protected:
	void InitPoolable() override;

};
