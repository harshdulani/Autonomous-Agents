#pragma once

#include "Graph/StateNode.h"
#include <SFML/Graphics/Color.hpp>

class Portal_ReadyState : public StateNode
{
public:
	void OnEnterState() override;
	void UpdateState(const float deltaTime) override;

private:
	sf::Color targetColor_;
	float lerpSpeed_ = 5.f;
};
