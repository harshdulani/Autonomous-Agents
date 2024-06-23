#pragma once
#include <SFML/Graphics/Transformable.hpp>
#include "Component.h"

class SceneComponent : public Component, public sf::Transformable
{
public:	
	void RecalculatePosition();

	sf::Vector2f GetLocalPosition() const;
	float GetLocalAngle() const;
	sf::Vector2f GetLocalScale() const;

	sf::Vector2f GetAbsolutePosition() const;
	sf::Vector2f GetForwardVector() const;
	sf::Vector2f GetRightVector() const;
	float GetAbsoluteAngle() const;
	sf::Vector2f GetAbsoluteScale() const;

	void SetLocalPosition(const sf::Vector2f& position);
	void SetLocalAngle(float angle);
	void SetLocalScale(sf::Vector2f value);
	
	void SetAbsolutePosition(const sf::Vector2f& position);
	void SetAbsoluteAngle(float angle);
	
protected:
	SceneComponent() = default;

private:
	// absolute transform
	sf::Vector2f AbsolutePosition;
};
