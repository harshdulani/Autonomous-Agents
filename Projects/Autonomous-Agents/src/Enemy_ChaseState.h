#pragma once

#include "pch.h"
#include "StateNode.h"

class EnemyShip;
class Enemy_ChaseState : public StateNode
{
public:
	void UpdateState(float deltaTime) override;

	void InitialiseState(float minDistance);

private:
	float minDist_ = 0.f;

	std::weak_ptr<EnemyShip> owner_;
};