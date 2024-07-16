#pragma once
#include "pch.h"
#include "Component.h"

class SceneComponent : public Component, private sf::Transformable
{
	friend class ObjectManager;
public:
	void Update(float DeltaTime) override = 0;
	
	sf::Transform GetTransform() const;
	sf::Transform GetTotalTransform() const;
	
	sf::Vector2f GetOrigin() const;
	void SetOrigin(float X, float Y);
	
	sf::Vector2f GetWorldPosition() const;
	void SetWorldPosition(const sf::Vector2f& position);
	sf::Vector2f GetLocalPosition() const;
	void SetLocalPosition(const sf::Vector2f& position);
	
	float GetWorldRotation() const;
	void SetWorldRotation(float angle);
	float GetLocalRotation() const;
	void SetLocalRotation(float angle);
	
	sf::Vector2f GetLocalScale() const;
	void SetLocalScale(sf::Vector2f value);
	sf::Vector2f GetWorldScale() const;

	sf::Vector2f GetForwardVector() const;
	sf::Vector2f GetRightVector() const;

	bool GetDenyUpdate() const;
	void SetDenyUpdate(bool bStatus);
protected:
	SceneComponent() = default;

private:
	void RecalculatePosition();

	// absolute transform
	sf::Vector2f AbsolutePosition;

	bool bDenyUpdate_ = false;
};
