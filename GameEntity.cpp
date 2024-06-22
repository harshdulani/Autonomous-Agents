#include "GameEntity.h"

void GameEntity::Update(float DeltaTime)
{
}

void GameEntity::Render(const sf::RenderWindow* Window) const
{
}

void GameEntity::Kill()
{
	bDeleteDeferred = true;
}

bool GameEntity::IsDeleteDeferred() const { return bDeleteDeferred; }
