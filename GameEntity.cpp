#include "GameEntity.h"
#include "Component.h"
#include "System.h"

void GameEntity::Update(float DeltaTime) { }

void GameEntity::Render(const sf::RenderWindow* Window) const { }

void GameEntity::Kill()
{
	Object::Kill();

	// mark all components as kill
	for (const auto& WeakComp : Components)
	{
		if(const auto Comp = WeakComp.lock())
			Comp->Kill();
	}
}

std::weak_ptr<GameEntity> GameEntity::GetWeakSelf()
{
	return System::GetInstance()->GetObjectMgr()->GetWeakPtr(this);
}

std::vector<std::weak_ptr<Component>>* GameEntity::GetComponentList()
{
	return &Components;
}
