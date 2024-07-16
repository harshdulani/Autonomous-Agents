#include "pch.h"
#include "LogicalParticleShape.h"
#include "../Maths.h"

void LogicalParticleShape::SetSamplingFunction(const std::function<sf::Vector2f(float)>& func)
{
	samplingFunction_ = func;
}

sf::Vector2f LogicalParticleShape::SamplingFunction(float t) const
{
	return samplingFunction_(t);
}

// static function definitions
std::function<sf::Vector2f(float)> LogicalParticleShape::CircleSamplingFunction =
	[](float t) -> sf::Vector2f
	{
		auto val = Math::LerpUnclamped(0.f, Math::TWO_PI, t);
		return {cosf(val), sinf(val)};
	};

std::function<sf::Vector2f(float)> LogicalParticleShape::HLineSamplingFunction =
	[](float t) -> sf::Vector2f
	{
		return {t, 0.f};
	};

std::function<sf::Vector2f(float)> LogicalParticleShape::VLineSamplingFunction =
	[](float t) -> sf::Vector2f
	{
		return {0.f, t};
	};
