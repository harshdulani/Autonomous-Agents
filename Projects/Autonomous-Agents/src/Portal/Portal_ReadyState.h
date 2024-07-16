#pragma once

#include "pch.h"
#include "../StateNode.h"

class Portal_ReadyState : public StateNode
{
public:
	void OnEnterState() override;
	void UpdateState(const float deltaTime) override;

private:
	sf::Color targetColor_;
	float lerpSpeed_ = 5.f;
};
