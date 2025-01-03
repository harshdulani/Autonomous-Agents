﻿#pragma once
#include <random>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

class Maths
{
public:
	static constexpr float PI = 3.141592654f;
	static constexpr float TWO_PI = PI * 2.f;
	
	static float WrapModulo(float value, float max);
	static float WrapModulo(float value, float min, float max);

	static float DegToRads(float degs);
	static float RadsToDegs(float rads);

	static bool IsAlmostZero(float val);
	static bool IsAlmostEqualTo(float a, float b);
	
	// Vector2f helpers
	static sf::Vector2f Normalize(const sf::Vector2f& in);
	static float DotProduct(const sf::Vector2f& a, const sf::Vector2f& b);
	static sf::Vector2f GetForwardVector(float angle_rads);
	static sf::Vector2f GetRightVector(float angle_rads);
	
	static sf::Vector2f LerpVector(sf::Vector2f From, sf::Vector2f To, float Alpha);
	static float GetVectorMagnitude(sf::Vector2f Vec);
	static float GetVectorSqrMagnitude(sf::Vector2f Vec);
	static sf::Vector2f LimitVector(sf::Vector2f vec, float limit);

	static bool IsAlmostEqualTo(const sf::Vector2f& a, const sf::Vector2f& b);
	
	static sf::Vector2f ZeroVector();
	static sf::Vector2f OneVector();
	static sf::Vector2f UpVector();    
	static sf::Vector2f RightVector();
	// End Vector2f helpers

	// Max-Inclusive
	static int GetRandInt(int Min, int Max);	
	// Max-Inclusive
	static float GetRandFloat(float Min, float Max);

	template<typename T> static T LerpUnclamped(T from, T to, float t);
	template<typename T> static T LerpClamped(T from, T to, float t);

	template<typename T> static T InverseLerpUnclamped(T from, T to, float value);
	template<typename T> static T InverseLerpClamped(T from, T to, float value);

	static float RemapUnclamped(float iMin, float iMax, float oMin, float oMax, float t);
	static float RemapClamped(float iMin, float iMax, float oMin, float oMax, float t);

	template<typename T> static T ClampValue(T value, T min, T max);
	
	static sf::Color LerpColor(const sf::Color& from, const sf::Color& to, float t);
	
	template <typename T> static int Sign(T val) { return (T(0) < val) - (val < T(0)); }

private:
	static std::random_device Rnd;
};

template<typename T>  T Maths::LerpUnclamped(T a, T b, float t)
{
	return (T)((1.0f - t) * a + b * t);
}

template<typename T>  T Maths::LerpClamped(T a, T b, float t)
{
	return (T)(LerpUnclamped(a, b, ClampValue(t, 0.0f, 1.0f)));
}

template<typename T>
T Maths::InverseLerpUnclamped(T a, T b, float v)
{
	return (T)(v - a) / (b - a);
}

template<typename T>
inline T Maths::InverseLerpClamped(T from, T to, float value)
{
	return ClampValue(0.f, 1.f, InverseLerpUnclamped(from, to, value));
}

template<typename T>
inline T Maths::ClampValue(const T value, const T min, const T max)
{
	if (value >= max)
		return max;
	if (value <= min)
		return min;

	return value;
}
