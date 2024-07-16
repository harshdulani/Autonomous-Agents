#include "pch.h"
#include "PrimitiveComponent.h"

void PrimitiveComponent::Render(sf::RenderWindow& Window, sf::RenderStates States) { }

bool PrimitiveComponent::GetDenyRender() const { return bDenyRender_; }

void PrimitiveComponent::SetDenyRender(bool bStatus)
{
	bDenyRender_ = bStatus;
}
