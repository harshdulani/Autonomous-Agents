#pragma once

#include "AParticleShape.h"

class BakedParticleShape : public AParticleShape
{
public:
	void SetStaticShape(const std::vector<sf::Vector2f>& shape);
	
protected:
	sf::Vector2f SamplingFunction(float t) const override;

private:
	std::vector<sf::Vertex> bakedShape_;
};