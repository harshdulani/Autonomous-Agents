#include "Component.h"
#include <utility>

void Component::Update(float DeltaTime) { }

void Component::SetEnabled(bool bStatus)
{
	bEnabled = bStatus;
}

bool Component::IsEnabled() const { return bEnabled; }

std::weak_ptr<GameEntity> Component::GetOwningEntity() const { return OwningEntity; }

void Component::SetOwningEntity(std::weak_ptr<GameEntity> InEntity)
{
	OwningEntity = std::move(InEntity);
}
