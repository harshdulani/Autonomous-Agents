#include "GameEntity.h"
#include "Component.h"
#include "Math.h"
#include "Collider.h"
#include "System.h"

// Core
GameEntity::~GameEntity()
{
	DestroyAllDrawables();
}

void GameEntity::Update(float DeltaTime)
{
}

void GameEntity::Render(sf::RenderWindow& Window) const
{
}

void GameEntity::OnCollision(std::weak_ptr<GameEntity> other)
{
}

void GameEntity::Kill()
{
	Object::Kill();

	// mark all components as kill
	for (const auto& WeakComp : Components)
	{
		if (const auto Comp = WeakComp.lock())
			Comp->Kill();
	}
}

void GameEntity::SetActive(bool bStatus)
{
	bActive = bStatus;
}

bool GameEntity::GetActive() const { return bActive; }
// End Core

// Components
void GameEntity::CreateCollider(float Radius)
{
	auto WeakCollider = AddComponent<Collider>();
	WeakCollider.lock()->SetRadius(Radius);
	bHasColliders = true;
}

std::vector<std::weak_ptr<Component>>* GameEntity::GetComponentList()
{
	return &Components;
}

void GameEntity::DestroyAllDrawables()
{
	int i = 0;
	for (auto& Drawable : Drawables)
	{
		if (Drawable.get())
		{
			Drawables.erase(Drawables.begin() + i);
		}
		i++;
	}
}

void GameEntity::DestroyDrawable(const std::weak_ptr<sf::Drawable>& Drawable)
{
	int i = 0;
	const auto SharedDrawable = Drawable.lock();
	if (!SharedDrawable)
		return;

	for (auto& DrawableItem : Drawables)
	{
		if (DrawableItem == SharedDrawable)
		{
			Drawables.erase(Drawables.begin() + i);
			return;
		}
		i++;
	}
}

std::weak_ptr<GameEntity> GameEntity::GetWeakSelf()
{
	return System::GetInstance()->GetObjectMgr()->GetWeakPtr(this);
}

// End Components

// Render and Update Priority
void GameEntity::SetRenderPriority(int p)
{
	RenderPriority = p;
}

int GameEntity::GetRenderPriority() const { return RenderPriority; }

void GameEntity::SetUpdatePriority(int p)
{
	UpdatePriority = p;
}

int GameEntity::GetUpdatePriority() const { return UpdatePriority; }

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
// End Render and Update Priority


// Transform Wrapper

sf::Transform GameEntity::GetTransform() const { return getTransform(); }
sf::Vector2f GameEntity::GetOrigin() const { return getOrigin(); }

void GameEntity::SetOrigin(float X, float Y)
{
	setOrigin(X, Y);
}

sf::Vector2f GameEntity::GetPosition() const { return getPosition(); }

float GameEntity::GetRotation() const { return getRotation(); }

sf::Vector2f GameEntity::GetScale() const { return getScale(); }

void GameEntity::SetPosition(const sf::Vector2f& NewPos)
{
	setPosition(NewPos);
	bPositionDirty = true;
}

void GameEntity::SetRotation(float NewAngle)
{
	setRotation(NewAngle);
	bRotationDirty = true;
}

void GameEntity::SetScale(const sf::Vector2f& NewScale)
{
	setScale(NewScale);
}

bool GameEntity::GetAndClearDirtyPosition()
{
	if (!bPositionDirty)
		return false;

	bPositionDirty = false;
	return true;
}

bool GameEntity::GetAndClearDirtyAngle()
{
	if (!bRotationDirty)
		return false;

	bRotationDirty = false;
	return true;
}

sf::Vector2f GameEntity::GetForwardVector() const
{
	return Math::GetForwardVector(Math::DegToRads(getRotation()));
}

sf::Vector2f GameEntity::GetRightVector() const
{
	return Math::GetRightVector(Math::DegToRads(getRotation()));
}
// End Transform Wrapper