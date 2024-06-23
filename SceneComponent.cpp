#include "SceneComponent.h"
#include "GameEntity.h"
#include "Math.h"

void SceneComponent::RecalculatePosition()
{
	if (auto parent = GetOwningEntity().lock())
	{
		//recalculate my world position to adjust for any changes
		//to position from parent or local position of self
		sf::Vector2f localT = parent->GetForwardVector() * getPosition().y +
			parent->GetRightVector() * getPosition().x;

		AbsolutePosition = parent->getPosition() + localT;
	}
}

sf::Vector2f SceneComponent::GetAbsolutePosition() const { return AbsolutePosition; }

void SceneComponent::SetAbsolutePosition(const sf::Vector2f& position)
{
	if (auto parent = GetOwningEntity().lock())
	{
		//recalculate my local position to adjust for any changes
		//to position from parent
		auto offset = position - parent->getPosition();
		setPosition(Math::DotProduct(offset, parent->GetForwardVector()),
					Math::DotProduct(offset, parent->GetRightVector()));
	}
	AbsolutePosition = position;
}

sf::Vector2f SceneComponent::GetLocalPosition() const { return getPosition(); }

void SceneComponent::SetLocalPosition(const sf::Vector2f& position)
{
	if (auto parent = GetOwningEntity().lock())
	{
		//recalculate my world position to adjust for any changes
		//to position from parent or local position of self
		auto localT = parent->GetForwardVector() * position.y +
			parent->GetRightVector() * position.x;

		AbsolutePosition = parent->getPosition() + localT;
	}
	setPosition(position);
}

float SceneComponent::GetAbsoluteAngle() const
{
	return GetOwningEntity().lock()->getRotation() + getRotation();
}

void SceneComponent::SetAbsoluteAngle(float angle)
{
	setRotation(angle - GetOwningEntity().lock()->getRotation());
}

float SceneComponent::GetLocalAngle() const { return getRotation(); }

void SceneComponent::SetLocalAngle(float angle)
{
	setRotation(angle);
}

sf::Vector2f SceneComponent::GetLocalScale() const { return getScale(); }

void SceneComponent::SetLocalScale(sf::Vector2f value)
{
	setScale(value);
}

sf::Vector2f SceneComponent::GetAbsoluteScale() const
{
	auto GameEntity = GetOwningEntity().lock();
	if (!GameEntity)
		return {};
	return {getScale().x * GameEntity->getScale().x, getScale().y * GameEntity->getScale().y};
}

sf::Vector2f SceneComponent::GetForwardVector() const
{
	return Math::GetForwardVector(Math::DegToRads(GetAbsoluteAngle()));
}

sf::Vector2f SceneComponent::GetRightVector() const
{
	return Math::GetRightVector(Math::DegToRads(GetAbsoluteAngle()));
}
