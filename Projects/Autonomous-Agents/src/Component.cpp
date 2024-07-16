#include "pch.h"
#include "Component.h"

void Component::Update(float DeltaTime) { }

void Component::SetEnabled(bool bStatus)
{
	bEnabled_ = bStatus;
}

bool Component::IsEnabled() const { return bEnabled_; }

std::weak_ptr<GameEntity> Component::GetOwningEntity() const { return owningEntity_; }

void Component::SetOwningEntity(std::weak_ptr<GameEntity> InEntity)
{
	owningEntity_ = std::move(InEntity);
}
