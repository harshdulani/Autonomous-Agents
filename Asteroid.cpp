#include "Asteroid.h"
#include <SFML/Graphics/ConvexShape.hpp>
#include "PhysicsComponent.h"
#include "Math.h"

void Asteroid::InitAsteroid(float Radius, sf::Color Color)
{
	CreateCollider(Radius);
	
	auto PhysicComponent = AddComponent<PhysicsComponent>();
	if(auto Physics = PhysicComponent.lock())
	{
		constexpr float SpeedRand = 50.f;
		Physics->SetVelocity(Math::GetRandFloat(-SpeedRand, SpeedRand),
			Math::GetRandFloat(-SpeedRand, SpeedRand));
		Physics->SetAngularVelocity(Math::GetRandFloat(-180.f, 180.f));
	}
	constexpr int MinPoly = 6;
	constexpr int MaxPoly = 8;

	float polyCount = static_cast<float>(Math::GetRandInt(MinPoly, MaxPoly));

	const auto Shape = std::make_shared<sf::ConvexShape>(static_cast<int>(polyCount));

	for (int i = 0; i < static_cast<int>(polyCount); ++i)
	{
		float angle = static_cast<float>(i) / polyCount * 2 * Math::PI;
		float X = cos(angle);
		float Y = sin(angle);
		Shape->setPoint(i, sf::Vector2f(X, Y) * (Radius));
	}
	Shape->setFillColor(Color);
	Color = sf::Color::White;
	Color.a = 55;
	Shape->setOutlineColor(Color);
	Shape->setOutlineThickness(2.5f);
	
	AddToDrawables(Shape);
}
