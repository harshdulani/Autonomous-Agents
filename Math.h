#pragma once
#include <random>
#include <SFML/System/Vector2.hpp>

class Math
{
public:
	static const float PI;
	static const float TWO_PI;
	static float WrapModulo(float value, float max);
	static float WrapModulo(float value, float min, float max);

	static float DegToRads(float degs);
	static float RadsToDegs(float rads);

	// Vector2f helpers
	static sf::Vector2f Normalize(const sf::Vector2f& in);
	static float DotProduct(const sf::Vector2f& a, const sf::Vector2f& b);
	static sf::Vector2f GetForwardVector(float angle_rads);
	static sf::Vector2f GetRightVector(float angle_rads);
	
	static sf::Vector2f ZeroVector();
	static sf::Vector2f UpVector();    
	static sf::Vector2f RightVector();
	
	static int GetRandInt(int Min, int Max);	
	static float GetRandFloat(float Min, float Max);
	
private:
	static std::random_device Rnd;
};
