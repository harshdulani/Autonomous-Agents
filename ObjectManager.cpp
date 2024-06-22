#include "ObjectManager.h"
#include "Component.h"
#include "Game.h"
#include "GameEntity.h"
#include "Math.h"

void ObjectManager::UpdateAllObjects(float DeltaTime)
{
	// update Entities, Components & Drawables (separately bc drawables not [yet] integrated as objects)
	for (auto& Entity : Entities)
	{
		Entity->Update(DeltaTime);
		WrapEntity(Entity.get());
		
		for (auto& Component : Entity->Components)
		{
			if (Component.get())
			{
				Component->Update(DeltaTime);
			}
		}
	}
}

void ObjectManager::RenderAllObjects(sf::RenderWindow* Window) const
{
	// draw entities
	for (const auto& Entity : Entities)
	{
		Entity->Render(Window);
		for (auto& Drawer : Entity->Drawables)
		{
			auto entityTransformed = sf::Transform::Identity * Entity->getTransform();
			if (Drawer.get())
			{
				sf::RenderStates states;
				states.transform = entityTransformed;
				if (auto Transformer = std::dynamic_pointer_cast<sf::Transformable>(Drawer))
				{
					states.transform *= Transformer->getTransform();
				}
				Window->draw(*Drawer, states);
			}
		}
	}
}

void ObjectManager::SetWindowVals(float X, float Y)
{
	WindowWidth = X;
	WindowHeight = Y;
}

void ObjectManager::WrapEntity(GameEntity* Entity) const
{
	auto EntityPosition = Entity->getPosition();
	if (EntityPosition.x > WindowWidth || EntityPosition.x < 0 ||
		EntityPosition.y > WindowHeight || EntityPosition.y < 0)
	{
		EntityPosition.x = Math::WrapModulo(EntityPosition.x, 0, WindowWidth);
		EntityPosition.y = Math::WrapModulo(EntityPosition.y, 0, WindowHeight);
		Entity->setPosition(EntityPosition);
	}
}
