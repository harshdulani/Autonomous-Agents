#include "SceneComponent.h"
#include "GameEntity.h"
#include "Maths.h"

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

sf::Vector2f SceneComponent::GetWorldPosition() const { return AbsolutePosition; }

void SceneComponent::SetWorldPosition(const sf::Vector2f& position)
{
	if (auto parent = GetOwningEntity().lock())
	{
		//recalculate my local position to adjust for any changes
		//to position from parent
		auto offset = position - parent->GetPosition();
		setPosition(Maths::DotProduct(offset, parent->GetForwardVector()),
					Maths::DotProduct(offset, parent->GetRightVector()));
	}
	AbsolutePosition = position;
}

sf::Vector2f SceneComponent::GetLocalPosition() const { return getPosition(); }

void SceneComponent::SetLocalPosition(const sf::Vector2f& position)
{
	setPosition(position);
	RecalculatePosition();
}

float SceneComponent::GetWorldRotation() const
{
	return GetOwningEntity().lock()->GetRotation() + getRotation();
}

void SceneComponent::SetWorldRotation(float angle)
{
	setRotation(angle - GetOwningEntity().lock()->GetRotation());
}

float SceneComponent::GetLocalRotation() const { return getRotation(); }

void SceneComponent::SetLocalRotation(float angle)
{
	setRotation(angle);
}

sf::Vector2f SceneComponent::GetLocalScale() const { return getScale(); }

void SceneComponent::SetLocalScale(sf::Vector2f value)
{
	setScale(value);
}

sf::Vector2f SceneComponent::GetWorldScale() const
{
	auto GameEntity = GetOwningEntity().lock();
	if (!GameEntity)
		return {};
	return {getScale().x * GameEntity->GetScale().x, getScale().y * GameEntity->GetScale().y};
}

sf::Vector2f SceneComponent::GetForwardVector() const
{
	return Maths::GetForwardVector(Maths::DegToRads(GetWorldRotation()));
}

sf::Vector2f SceneComponent::GetRightVector() const
{
	return Maths::GetRightVector(Maths::DegToRads(GetWorldRotation()));
}

bool SceneComponent::GetDenyUpdate() const { return bDenyUpdate_; }

void SceneComponent::SetDenyUpdate(bool bStatus)
{
	bDenyUpdate_ = bStatus;
}
