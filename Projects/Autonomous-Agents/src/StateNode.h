#pragma once

#include "pch.h"
#include "Node.h"

class GameEntity;

class StateNode : public Node
{
public:
	void SetGameEntity(std::weak_ptr<GameEntity> owningEntity);

	virtual void OnEnterState();
	virtual void UpdateState(const float deltaTime) = 0;
	virtual void OnExitState();

protected:
	std::weak_ptr<GameEntity> entity;
};
