#pragma once

#include "Graph/StateNode.h"

class AutoShoot_SuspendedState : public StateNode
{
public:
	void UpdateState(float deltaTime) override;

private:

};
