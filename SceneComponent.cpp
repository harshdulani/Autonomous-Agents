#include "SceneComponent.h"
#include "GameEntity.h"
#include "Math.h"

void SceneComponent::RecalculatePosition()
{
	if (auto parent = GetOwningEntity().lock())
	{
		AbsolutePosition = (parent->GetTransform() * getTransform()).transformPoint(parent->GetOrigin() + GetOrigin());
	}
}

sf::Transform SceneComponent::GetTransform() const { return getTransform(); }

sf::Transform SceneComponent::GetTotalTransform() const
{
	return GetOwningEntity().lock()->GetTransform() * getTransform();
}

sf::Vector2f SceneComponent::GetOrigin() const { return getOrigin(); }

void SceneComponent::SetOrigin(float X, float Y) 
{
	setOrigin(X, Y);
}

sf::Vector2f SceneComponent::getWorldPosition() const { return AbsolutePosition; }

void SceneComponent::setWorldPosition(const sf::Vector2f& position)
{
	if (auto parent = GetOwningEntity().lock())
	{
		//recalculate my local position to adjust for any changes
		//to position from parent
		auto offset = position - parent->GetPosition();
		setPosition(Math::DotProduct(offset, parent->GetForwardVector()),
					Math::DotProduct(offset, parent->GetRightVector()));
	}
	AbsolutePosition = position;
}

sf::Vector2f SceneComponent::getLocalPosition() const { return getPosition(); }

void SceneComponent::setLocalPosition(const sf::Vector2f& position)
{
	setPosition(position);
	RecalculatePosition();
}

float SceneComponent::getWorldRotation() const
{
	return GetOwningEntity().lock()->GetRotation() + getRotation();
}

void SceneComponent::setWorldRotation(float angle)
{
	setRotation(angle - GetOwningEntity().lock()->GetRotation());
}

float SceneComponent::getLocalRotation() const { return getRotation(); }

void SceneComponent::setLocalRotation(float angle)
{
	setRotation(angle);
}

sf::Vector2f SceneComponent::getLocalScale() const { return getScale(); }

void SceneComponent::setLocalScale(sf::Vector2f value)
{
	setScale(value);
}

sf::Vector2f SceneComponent::getWorldScale() const
{
	auto GameEntity = GetOwningEntity().lock();
	if (!GameEntity)
		return {};
	return {getScale().x * GameEntity->GetScale().x, getScale().y * GameEntity->GetScale().y};
}

sf::Vector2f SceneComponent::GetForwardVector() const
{
	return Math::GetForwardVector(Math::DegToRads(getWorldRotation()));
}

sf::Vector2f SceneComponent::GetRightVector() const
{
	return Math::GetRightVector(Math::DegToRads(getWorldRotation()));
}
