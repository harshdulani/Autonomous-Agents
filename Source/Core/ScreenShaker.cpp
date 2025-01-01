#include "ScreenShaker.h"
#include "System.h"
#include "Game.h"
#include <cmath>
#include "TimerManager.h"

ScreenShaker::ScreenShaker()
{
	timerManager_ = System::GetInstance()->GetTimerManager();
}

void ScreenShaker::SetViewCam(const std::weak_ptr<sf::View>& cam)
{
	cam_ = cam;
}

void ScreenShaker::UpdateScreenShakes(float deltaTime)
{
	float maxAngle = 0.0f;

	auto timerMgr = timerManager_;
	auto itr = std::find_if(impulses_.begin(),
							impulses_.end(),
							[timerMgr](const ScreenShakeImpulse& impulse) -> bool
							{
								return !timerMgr->IsTimerRunning(impulse.timerHandle);
							});
	if (itr != impulses_.end())
	{
		timerMgr->Kill((*itr).timerHandle);
		impulses_.erase(itr);
	}

	for (auto & impulse : impulses_)
	{
		if (!timerManager_->IsTimerRunning(impulse.timerHandle))
		{
			continue;
		}

		const float t = 1.0f - timerManager_->GetValueDone(impulse.timerHandle);
		const float angle = sin(SampleEaseOutCubic(t) * impulse.frequency) * impulse.amplitude;

		if (abs(angle) > abs(maxAngle))
		{
			maxAngle = angle;
		}
	}

	if (auto cam = cam_.lock())
	{
		cam->setRotation(maxAngle);
	}
}

void ScreenShaker::CreateImpulse(float duration,
								float amplitude,
								float frequency)
{
	ScreenShakeImpulse impulse = ScreenShakeImpulse(duration,
													amplitude,
													frequency,
													timerManager_->GetNewTimer());
	timerManager_->SetTimerEnd(impulse.timerHandle, duration);

	impulses_.push_back(impulse);
}

float ScreenShaker::SampleEaseOutCubic(float t)
{
	return 1 - pow(1 - t, 3);
}
