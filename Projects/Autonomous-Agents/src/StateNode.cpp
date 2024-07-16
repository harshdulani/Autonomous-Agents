#include "pch.h"
#include "StateNode.h"
#include "TransitionEdge.h"

void StateNode::SetGameEntity(std::weak_ptr<GameEntity> owningEntity)
{
	entity = std::move(owningEntity);
}

void StateNode::OnEnterState()
{
}

void StateNode::OnExitState()
{
}