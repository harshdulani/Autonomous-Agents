#pragma once
#include "pch.h"

#include "SceneComponent.h"

class PrimitiveComponent : public SceneComponent
{
public:
	virtual void Render(sf::RenderWindow& Window, sf::RenderStates States);
	bool GetDenyRender() const;
	void SetDenyRender(bool bStatus);

private:
	bool bDenyRender_ = false;
};
