#include "pch.h"
#include "BakedParticleShape.h"
#include "../Maths.h"

void BakedParticleShape::SetStaticShape(const std::vector<sf::Vector2f>& shape)
{
	if (GetShapeResolution() == -1)
	{
		SetShapeResolution(static_cast<int>(shape.size()));
	}
	auto color = bakedShape_[0].color;
	for (int i  = 0; i < shape.size(); ++i)
	{
		bakedShape_[i].position = shape[i];
		bakedShape_[i].color = color;
	}
}

sf::Vector2f BakedParticleShape::SamplingFunction(float t) const
{
	assert(!bakedShape_.empty());

	int resolution = static_cast<int>(bakedShape_.size()) - 1;

	float slice = 1.f / resolution;
	float val = resolution * t;
	if (val == (int)val)
	{
		//t / res is a whole number and hence can be used as an index
		return bakedShape_[(int)val].position;
	}
	int prev = (int) floorf(val);
	int next = (int) ceilf(val);
	float prevT = prev * slice;
	float nextT = next * slice;

	float v = Math::InverseLerpClamped(prevT, nextT, t);
	return Math::LerpVector(bakedShape_[prev].position, bakedShape_[next].position, v);
}
