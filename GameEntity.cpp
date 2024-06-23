#include "GameEntity.h"
#include "Component.h"
#include "Math.h"
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

void GameEntity::SetActive(bool bStatus)
{
	bActive = bStatus;
}

bool GameEntity::GetActive() const { return bActive; }

void GameEntity::SetRenderPriority(int p)
{
	RenderPriority = p;
}

int GameEntity::GetRenderPriority() const { return RenderPriority; }

bool GameEntity::IsRenderDirty() const { return bRenderDirty; }

void GameEntity::SetRenderDirty(bool Cond)
{
	bRenderDirty = Cond;
}

bool GameEntity::IsUpdateDirty() const { return bUpdateDirty; }

void GameEntity::SetUpdateDirty(bool Cond)
{
	bUpdateDirty = Cond;
}

void GameEntity::SetUpdatePriority(int p)
{
	UpdatePriority = p;
}

int GameEntity::GetUpdatePriority() const { return UpdatePriority; }

std::weak_ptr<GameEntity> GameEntity::GetWeakSelf()
{
	return System::GetInstance()->GetObjectMgr()->GetWeakPtr(this);
}

std::vector<std::weak_ptr<Component>>* GameEntity::GetComponentList()
{
	return &Components;
}

sf::Vector2f GameEntity::GetForwardVector() const
{
	return Math::GetForwardVector(Math::DegToRads(getRotation()));
}

sf::Vector2f GameEntity::GetRightVector() const
{
	return Math::GetRightVector(Math::DegToRads(getRotation()));
}