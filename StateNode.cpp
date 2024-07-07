#include "StateNode.h"

#include <utility>
#include "TransitionEdge.h"

void StateNode::SetGameEntity(std::weak_ptr<GameEntity> owningEntity)
{
	entity_ = std::move(owningEntity);
}

void StateNode::OnEnterState()
{
}

void StateNode::OnExitState()
{
}