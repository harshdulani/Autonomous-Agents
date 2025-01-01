#pragma once

#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Vector2.hpp>

class AParticleShape
{
public:
	virtual ~AParticleShape() = default;
	
	virtual sf::Vector2f GetPointSampled(float t) const;
	virtual sf::Vector2f GetRandomPointSampled() const;
	
	std::vector<sf::Vertex> GenerateVertexShape(sf::Color color) const;
	
	void SetShapeResolution(int res);
	int GetShapeResolution() const;
	void SetShapeScale(float scale);
	float GetShapeScale() const;
	
protected:	
	void ScaleInVector(sf::Vector2f& inVector) const;

	// this class remains abstract
	
	// Take in a float between 0.f-1.f to get a position interpolated between shape indices
	virtual sf::Vector2f SamplingFunction(float t) const = 0;
	
private:
	float shapeScale_ = 1.f;
	int resolution_ = -1;
};
