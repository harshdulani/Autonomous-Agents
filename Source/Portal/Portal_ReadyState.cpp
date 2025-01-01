#include "Portal_ReadyState.h"
#include "Portal.h"
#include "Maths.h"

void Portal_ReadyState::OnEnterState()
{
	if (auto portal = std::dynamic_pointer_cast<Portal>(entity.lock()))
	{
		targetColor_ = portal->GetReadyColor();
	}
}

void Portal_ReadyState::UpdateState(const float deltaTime)
{
	if (auto portalEntity = entity.lock())
	{
		if (auto portal = std::dynamic_pointer_cast<Portal>(portalEntity))
		{
			sf::Color lerpedVal = Maths::LerpColor(portal->GetRenderColor(),
												   targetColor_,
												   deltaTime * lerpSpeed_);
			portal->SetRenderColor(lerpedVal);
		}
	}
}