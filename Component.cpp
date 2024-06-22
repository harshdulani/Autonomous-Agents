#include "Component.h"
#include <utility>

void Component::Update(float DeltaTime) { }

std::weak_ptr<GameEntity> Component::GetOwningEntity() const { return OwningEntity; }

void Component::SetOwningEntity(std::weak_ptr<GameEntity> InEntity)
{
	OwningEntity = std::move(InEntity);
}
