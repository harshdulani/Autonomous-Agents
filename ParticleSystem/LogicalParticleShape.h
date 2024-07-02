#pragma once

#include "AParticleShape.h"
#include <functional>

class LogicalParticleShape : public AParticleShape
{
public:	
	void SetSamplingFunction(const std::function<sf::Vector2f(float)>& func);

	static std::function<sf::Vector2f(float)> HLineSamplingFunction;
	static std::function<sf::Vector2f(float)> VLineSamplingFunction;
	static std::function<sf::Vector2f(float)> CircleSamplingFunction;

protected:
	sf::Vector2f SamplingFunction(float t) const override;

private:
	std::function<sf::Vector2f(float)> samplingFunction_;
};
