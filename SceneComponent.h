#pragma once
#include <SFML/Graphics/Transformable.hpp>
#include "Component.h"

class SceneComponent : public Component, private sf::Transformable
{
	friend class ObjectManager;
public:
	sf::Transform GetTransform() const;
	sf::Transform GetTotalTransform() const;
	
	sf::Vector2f GetOrigin() const;
	void SetOrigin(float X, float Y);
	
	sf::Vector2f getWorldPosition() const;
	void setWorldPosition(const sf::Vector2f& position);
	sf::Vector2f getLocalPosition() const;
	void setLocalPosition(const sf::Vector2f& position);
	
	float getWorldRotation() const;
	void setWorldRotation(float angle);
	float getLocalRotation() const;
	void setLocalRotation(float angle);
	
	sf::Vector2f getLocalScale() const;
	void setLocalScale(sf::Vector2f value);
	sf::Vector2f getWorldScale() const;

	sf::Vector2f GetForwardVector() const;
	sf::Vector2f GetRightVector() const;

protected:
	SceneComponent() = default;

private:
	void RecalculatePosition();

	// absolute transform
	sf::Vector2f AbsolutePosition;
};
