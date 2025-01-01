#include "Maths.h"
#include <cmath>
#include <SFML/System/Vector2.hpp>

std::random_device Maths::Rnd;

int Maths::GetRandInt(int Min, int Max)
{
	std::uniform_int_distribution<> Dist(Min, Max);
	return Dist(Rnd);
}

float Maths::GetRandFloat(float Min, float Max)
{
	std::uniform_real_distribution<float> Dist(Min, Max);
	return Dist(Rnd);
}

float Maths::RemapUnclamped(float iMin, float iMax, float oMin, float oMax, float t)
{
	float v = InverseLerpUnclamped(iMin, iMax, t);
	return LerpUnclamped(oMin, oMax, v);
}

float Maths::RemapClamped(float iMin, float iMax, float oMin, float oMax, float t)
{
	float v = InverseLerpClamped(iMin, iMax, t);
	return LerpClamped(oMin, oMax, v);
}

float Maths::WrapModulo(float value, float max)
{
	while (value > max)
		value -= max;
	while (value < 0.0f)
		value += max;
	return value;
}

float Maths::WrapModulo(float value, float min, float max)
{
	float diff = max - min;
	while (value > max)
		value -= diff;
	while (value < min)
		value += diff;
	return value;
}

float Maths::DegToRads(const float degs)
{
	// Convert angle to radians
	return degs * (PI / 180.0f);
}

float Maths::RadsToDegs(const float rads)
{
	// Convert angle to degrees
	return rads * (180.0f / PI);
}

bool Maths::IsAlmostZero(float val)
{
	// change to epsi
	return val < 0.001f;
}

bool Maths::IsAlmostEqualTo(float a, float b)
{
	return IsAlmostZero(abs(a - b));
}

bool Maths::IsAlmostEqualTo(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return IsAlmostZero(abs(a.x - b.x)) && IsAlmostZero(abs(a.y - b.y));
}

sf::Vector2f Maths::ZeroVector() { return {}; }
sf::Vector2f Maths::OneVector() { return {1.f, 1.f}; }
sf::Vector2f Maths::UpVector() { return {0.f, 1.f}; }
sf::Vector2f Maths::RightVector() { return {1.f, 0.f}; }

sf::Vector2f Maths::Normalize(const sf::Vector2f& in)
{
	float magnitude = std::sqrt(in.x * in.x + in.y * in.y);

	if (magnitude == 0)
	{
		// Handle the zero vector case
		return {0.0f, 0.0f};
	}

	return {in.x / magnitude, in.y / magnitude};
}

float Maths::DotProduct(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return (a.x * b.x) + (a.y * b.y);
}

sf::Vector2f Maths::GetForwardVector(const float angle_rads)
{
	return {std::sin(angle_rads), -std::cos(angle_rads)};	
}

sf::Vector2f Maths::GetRightVector(float angle_rads)
{
	return {std::cos(angle_rads), std::sin(angle_rads)};
}

sf::Vector2f Maths::LerpVector(sf::Vector2f From, sf::Vector2f To, float Alpha)
{
	return {LerpUnclamped(From.x,
						  To.x,
						  Alpha),
			LerpUnclamped(From.y,
						  To.y,
						  Alpha)};
}

float Maths::GetVectorMagnitude(sf::Vector2f Vec)
{
	return sqrt(Vec.x * Vec.x + Vec.y * Vec.y); 
}

float Maths::GetVectorSqrMagnitude(sf::Vector2f Vec)
{
	return Vec.x * Vec.x + Vec.y * Vec.y; 
}

sf::Vector2f Maths::LimitVector(sf::Vector2f vec, float limit)
{
	if (GetVectorMagnitude(vec) > limit)
	{
		vec = Normalize(vec);
		vec = vec * limit;
	}
	return vec;
}

sf::Color Maths::LerpColor(const sf::Color& from, const sf::Color& to, float t)
{
	if (from == to)
		return from;
	if (t == 0.f)
		return from;
	if (t == 1.f)
		return to;

	return {
		LerpClamped(from.r, to.r, t),
		LerpClamped(from.g, to.g, t),
		LerpClamped(from.b, to.b, t),
		LerpClamped(from.a, to.a, t),
	};
}
