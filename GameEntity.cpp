#include "GameEntity.h"
#include "Component.h"
#include "Math.h"
#include "Collider.h"
#include "System.h"
#include "ParticleSystem/ParticleSystem.h"

void GameEntity::Update(float DeltaTime)
{
}

void GameEntity::Render(sf::RenderWindow& Window) const
{
	if(components_.empty())
		return;
	
	sf::VertexArray lines;
	lines.setPrimitiveType(sf::Lines);
	auto myPos = GetPosition();
	auto Col = sf::Color(255, 255, 255, 55);
	auto myVert = sf::Vertex(myPos, Col);
	for (const auto& weakCmp : components_)
	{
		if (auto cmp = weakCmp.lock())
		{
			if(const auto scene = std::dynamic_pointer_cast<sf::Transformable>(cmp))
			{
				lines.append(myVert);
				lines.append({{(scene->getTransform() * GetTransform()).transformPoint(scene->getPosition())}, Col});
			}
		}
	}
	
	Window.draw(lines);
}

// Core
GameEntity::~GameEntity()
{
	DestroyAllDrawables();
}

void GameEntity::OnCollision(std::weak_ptr<GameEntity> other)
{
}

void GameEntity::Kill()
{
	Object::Kill();

	// mark all components as kill
	for (const auto& WeakComp : components_)
	{
		if (const auto Comp = WeakComp.lock())
			Comp->Kill();
	}
}

void GameEntity::SetActive(bool bStatus)
{
	bActive_ = bStatus;
}

bool GameEntity::GetActive() const { return bActive_; }

bool GameEntity::HasColliders() const { return bHasColliders_; }

// End Core

// Components
void GameEntity::CreateCollider(float Radius)
{
	auto WeakCollider = AddComponent<Collider>();
	WeakCollider.lock()->SetRadius(Radius);
	bHasColliders_ = true;
}

std::vector<std::weak_ptr<Component>>* GameEntity::GetComponentList()
{
	return &components_;
}

void GameEntity::DestroyAllDrawables()
{
	int i = 0;
	for (auto& Drawable : drawables_)
	{
		if (Drawable.get())
		{
			drawables_.erase(drawables_.begin() + i);
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

	for (auto& DrawableItem : drawables_)
	{
		if (DrawableItem == SharedDrawable)
		{
			drawables_.erase(drawables_.begin() + i);
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
	renderPriority_ = p;
}

int GameEntity::GetRenderPriority() const { return renderPriority_; }

void GameEntity::SetUpdatePriority(int p)
{
	updatePriority_ = p;
}

int GameEntity::GetUpdatePriority() const { return updatePriority_; }

bool GameEntity::IsRenderDirty() const { return bRenderDirty_; }

void GameEntity::SetRenderDirty(bool Cond)
{
	bRenderDirty_ = Cond;
}

bool GameEntity::IsUpdateDirty() const { return bUpdateDirty_; }

void GameEntity::SetUpdateDirty(bool Cond)
{
	bUpdateDirty_ = Cond;
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
	bPositionDirty_ = true;
}

void GameEntity::SetRotation(float NewAngle)
{
	setRotation(NewAngle);
	bRotationDirty_ = true;
}

void GameEntity::SetScale(const sf::Vector2f& NewScale)
{
	setScale(NewScale);
}

void GameEntity::SetScale(float NewScale)
{
	setScale(Math::OneVector() * NewScale);
}

bool GameEntity::GetAndClearDirtyPosition()
{
	if (!bPositionDirty_)
		return false;

	bPositionDirty_ = false;
	return true;
}

bool GameEntity::GetAndClearDirtyAngle()
{
	if (!bRotationDirty_)
		return false;

	bRotationDirty_ = false;
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