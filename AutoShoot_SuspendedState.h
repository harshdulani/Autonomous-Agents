#pragma once

#include "StateNode.h"
class AutoShoot_SuspendedState : public StateNode
{
public:
	void UpdateState(float deltaTime) override;

private:

};