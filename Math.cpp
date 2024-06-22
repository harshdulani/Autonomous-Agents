#include "Math.h"
#include <cmath>
#include <SFML/System/Vector2.hpp>

std::random_device Math::Rnd;
const float Math::PI = 3.141592654f;
const float Math::TWO_PI = PI * 2.0f;

int Math::GetRandInt(int Min, int Max)
{
	std::uniform_int_distribution<> Dist(Min, Max);
	return Dist(Rnd);
}

float Math::GetRandFloat(float Min, float Max)
{
	std::uniform_real_distribution<float> Dist(Min, Max);
	return Dist(Rnd);
}

float Math::WrapModulo(float value, float max)
{
	while (value > max)
		value -= max;
	while (value < 0.0f)
		value += max;
	return value;
}

float Math::WrapModulo(float value, float min, float max)
{
	float diff = max - min;
	while (value > max)
		value -= diff;
	while (value < min)
		value += diff;
	return value;
}

float Math::DegToRads(const float degs)
{
	// Convert angle to radians
	return degs * (PI / 180.0f);
}

float Math::RadsToDegs(const float rads)
{
	// Convert angle to degrees
	return rads * (180.0f / PI);
}

sf::Vector2f Math::ZeroVector() { return {};}
sf::Vector2f Math::UpVector() { return {0.f, 1.f};}
sf::Vector2f Math::RightVector() { return {1.f, 0.f};}

sf::Vector2f Math::Normalize(const sf::Vector2f& in)
{
	float magnitude = std::sqrt(in.x * in.x + in.y * in.y);
	
	if (magnitude == 0) {
		// Handle the zero vector case
		return {0.0f, 0.0f};
	}

	return {in.x / magnitude, in.y / magnitude};
}

float Math::DotProduct(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return (a.x * b.x) + (a.y * b.y);
}

sf::Vector2f Math::GetForwardVector(const float angle_rads)
{
	sf::Vector2f forward_vector;
	forward_vector.x = std::cos(angle_rads);
	forward_vector.y = std::sin(angle_rads);

	return forward_vector;
}

sf::Vector2f Math::GetRightVector(float angle_rads)
{
	sf::Vector2f right_vector;
	right_vector.x = -std::sin(angle_rads);
	right_vector.y = std::cos(angle_rads);

	return right_vector;
}
