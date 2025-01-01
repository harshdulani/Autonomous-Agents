#include "Collider.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "Maths.h"

void Collider::Update(float DeltaTime)
{
}

void Collider::Render(sf::RenderWindow& Window, sf::RenderStates States)
{
	PrimitiveComponent::Render(Window, States);
	if (!bVisualizeCollider_)
		return;

	States.transform = GetTransform().translate(GetWorldPosition());
	Window.draw(circle_, States);
}

bool Collider::IsValidCollider() const
{
	return IsEnabled() && !IsPendingKill() && radius_ > 0.0f;
}

float Collider::GetRadius() const { return radius_; }
void Collider::SetRadius(float r)
{
	bool bRegenCircle = false;
	if (abs(radius_ - r) > 0.1f)
	{
		bRegenCircle = true;
	}
	radius_ = r;

	if(bRegenCircle)
		RegenCircle();
}

bool Collider::GetShowColliderVisible() const { return  bVisualizeCollider_; }

void Collider::SetColliderVisible(bool bStatus)
{
	bVisualizeCollider_ = bStatus;
}

void Collider::RegenCircle()
{
	int VertCount = 36;
	circle_ = sf::VertexArray(sf::LineStrip, VertCount);
	sf::Color Color {0, 204, 153, 155};
	for (int i = 0; i < 35; ++i)
	{
		float Xx = Maths::TWO_PI * i / 36;
		circle_[i] = sf::Vertex({cos(Xx) * radius_, sin(Xx) * radius_}, Color);
	}
	circle_[35] = circle_[0];
}
