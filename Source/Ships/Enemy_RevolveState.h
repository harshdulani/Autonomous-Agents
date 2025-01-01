#pragma once
#include "Graph/StateNode.h"

class EnemyShip;

class Enemy_RevolveState : public StateNode
{
public:
	void UpdateState(const float deltaTime) override;

	void InitialiseState(const float maxDistance, 
						 const float pathRadius, 
						 const float deltaAngle);

private:
	float minDist_ = 0.f;
	float pathRadius_ = 0.f;
	float deltaAngle_ = 0.f;

	std::weak_ptr<EnemyShip> owner_;
};