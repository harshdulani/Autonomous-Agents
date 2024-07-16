#pragma once

#include "pch.h"


class TimerManager;

class ScreenShaker
{
public:
	ScreenShaker();
	void SetViewCam(const std::weak_ptr<sf::View>& cam);

	void UpdateScreenShakes(float deltaTime);

	void CreateImpulse(float duration, 
					   float amplitude,
					   float frequency);

private:
	static float SampleEaseOutCubic(float t);

public:
	
private:
	std::weak_ptr<sf::View> cam_;
	TimerManager* timerManager_ = nullptr;

	struct ScreenShakeImpulse
	{
	public:
		ScreenShakeImpulse(const float impulseDuration,
						   const float impulseAmplitude,
						   const float impulseFrequency,
						   const uint32_t impulseTimerHandle) : 
			duration(impulseDuration),
			amplitude(impulseAmplitude),
			frequency(impulseFrequency),
			timerHandle(impulseTimerHandle)
		{
		}

		float duration;
		float amplitude;
		float frequency;
		uint32_t timerHandle;
	};

	std::vector<ScreenShakeImpulse> impulses_;
};
