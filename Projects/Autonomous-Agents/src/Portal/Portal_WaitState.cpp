#include "pch.h"
#include "Portal_WaitState.h"
#include "Portal.h"
#include "../TimerManager.h"
#include "../Maths.h"


void Portal_WaitState::OnEnterState()
{
	Base_WaitState::OnEnterState();
}

void Portal_WaitState::UpdateState(const float deltaTime)
{
	Base_WaitState::UpdateState(deltaTime);

	float timeDone = timerManager_->GetTimeSinceSpawn(waitDurationTimerHandle);

	if (auto portal = std::dynamic_pointer_cast<Portal>(entity.lock()))
	{
		if (timeDone < 0.5f)
		{
			LerpUpdateColor(portal->GetWaitColor(), deltaTime);
			return;
		}

		if (timeDone > 0.5f && timeDone < waitDuration - 0.5f)
		{
			portal->SetRenderColor(portal->GetWaitColor());
			return;
		}

		if (timeDone > waitDuration - 0.5f)
		{
			LerpUpdateColor(portal->GetReadyColor(), deltaTime);
			return;
		}
	}
}

void Portal_WaitState::OnTimerEnd()
{
	if (auto portal = std::dynamic_pointer_cast<Portal>(entity.lock()))
	{
		portal->EndFSMCooldown();
	}
}

void Portal_WaitState::LerpUpdateColor(const sf::Color& color, const float deltaTime)
{
	if (auto portal = std::dynamic_pointer_cast<Portal>(entity.lock()))
	{
		auto lerpedCol = Math::LerpColor(portal->GetRenderColor(),
												color,
												deltaTime * lerpSpeed_);
		portal->SetRenderColor(lerpedCol);
	}
}
