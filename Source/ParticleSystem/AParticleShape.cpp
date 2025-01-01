#include "AParticleShape.h"
#include "Maths.h"

sf::Vector2f AParticleShape::GetRandomPointSampled() const
{
	return GetPointSampled(Maths::GetRandFloat(0.f, 1.f));
}

int AParticleShape::GetShapeResolution() const { return resolution_; }

void AParticleShape::SetShapeScale(float scale)
{
	shapeScale_ = scale;
}

float AParticleShape::GetShapeScale() const { return shapeScale_; }

void AParticleShape::SetShapeResolution(int res)
{
	resolution_ = res;
}

sf::Vector2f AParticleShape::GetPointSampled(float t) const
{
	auto position = SamplingFunction(t);
	ScaleInVector(position);
	return position;
}

void AParticleShape::ScaleInVector(sf::Vector2f& inVector) const
{
	// interesting: scaling a vector with another vector instead of a scalar is called the Hadamard product
	inVector *= GetShapeScale();
}

std::vector<sf::Vertex> AParticleShape::GenerateVertexShape(sf::Color color) const
{
	std::vector<sf::Vertex> shape;
	for (int i = 0; i < resolution_ + 1; i++)
	{
		auto pos = GetPointSampled(i / static_cast<float>(resolution_));
		shape.push_back({{ pos.x, pos.y}, color });
	}
	return shape;
}
