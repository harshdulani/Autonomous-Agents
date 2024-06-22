#include "Component.h"

void Component::Update(float DeltaTime) { }

GameEntity* Component::GetOwningEntity() const { return OwningEntity; }

void Component::SetOwningEntity(GameEntity* InEntity)
{
	OwningEntity = InEntity;
}
