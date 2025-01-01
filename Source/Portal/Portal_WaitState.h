#pragma once

#include "Ships/Base_WaitState.h"
#include <SFML/Graphics/Color.hpp>

class TimerManager;

class Portal_WaitState : public Base_WaitState
{
public:
	void OnEnterState() override;
	void UpdateState(const float deltaTime) override;

protected:
	void OnTimerEnd() override;

private:
	void LerpUpdateColor(const sf::Color& color, const float deltaTime);
	float lerpSpeed_ = 5.f;
};