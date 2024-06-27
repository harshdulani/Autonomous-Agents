#include "Collider.h"
#include <SFML/Graphics/RenderWindow.hpp>

#include "Math.h"

void Collider::Render(sf::RenderWindow& Window, sf::RenderStates States)
{
	PrimitiveComponent::Render(Window, States);
	if (!bVisualizeCollider)
		return;

	States.transform = GetTransform().translate(getWorldPosition());
	Window.draw(Circle, States);
}

bool Collider::IsValidCollider() const
{
	return IsEnabled() && !IsPendingKill() && Radius > 0.0f;
}

float Collider::GetRadius() const { return Radius; }
void Collider::SetRadius(float r)
{
	bool bRegenCircle = false;
	if (abs(Radius - r) > 0.1f)
	{
		bRegenCircle = true;
	}
	Radius = r;

	if(bRegenCircle)
		RegenCircle();
}

bool Collider::GetShowColliderVisible() const { return  bVisualizeCollider; }

void Collider::SetColliderVisible(bool bStatus)
{
	bVisualizeCollider = bStatus;
}

void Collider::RegenCircle()
{
	int VertCount = 36;
	Circle = sf::VertexArray(sf::LineStrip, VertCount);
	sf::Color Color {0, 204, 153, 155};
	for (int i = 0; i < 35; ++i)
	{
		float Xx = Math::TWO_PI * i / 36;
		Circle[i] = sf::Vertex({cos(Xx) * Radius, sin(Xx) * Radius}, Color);
	}
	Circle[35] = Circle[0];
}
