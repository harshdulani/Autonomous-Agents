#pragma once
#include "SceneComponent.h"

class Collider : public SceneComponent
{
public:
	bool IsValidCollider() const;

	float Radius = -1.f;
};
