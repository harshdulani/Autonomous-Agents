#pragma once
#include <memory>
#include <vector>
#include <cassert>
#include <SFML/Graphics/Transformable.hpp>

namespace sf
{
	class Drawable;
}

class Component;

namespace sf
{
	class RenderWindow;
}

class GameEntity : public sf::Transformable
{
	//@todo: friend class ObjectMgr
	friend class Game;
public:
	~GameEntity() override = default;
	virtual void Update(float DeltaTime);
	virtual void Render(const sf::RenderWindow* Window) const;

	virtual void Kill();

	bool IsDeleteDeferred() const;

	template<typename T>
	std::weak_ptr<T> AddComponent();

	template<typename T>
	std::weak_ptr<T> GetComponent();
	
	template<typename T>
	void AddToDrawables(std::shared_ptr<T> InDraw);

	template<typename T>
	std::weak_ptr<T> GetDrawable();

private:
	//@todo: ownership moved to ObjectMgr
	std::vector<std::shared_ptr<Component>> Components;
	std::vector<std::shared_ptr<sf::Drawable>> Drawables;
	
	//@todo: move up to Object
	bool bDeleteDeferred = false;
};

template<typename T>
std::weak_ptr<T> GameEntity::AddComponent()
{
	assert((std::is_base_of<Component, T>::value && "Type mismatch while creating component"));

	std::shared_ptr<T> Component = std::make_shared<T>();

	Component->SetOwningEntity(this);
	Components.push_back(Component);
	return Component;
}

template<typename T>
std::weak_ptr<T> GameEntity::GetComponent()
{
	assert((std::is_base_of<Component, T>::value && "Type is not a component type"));

	for (auto& Component : Components)
	{
		//if (auto shared = Component)
		{
			if (std::shared_ptr<T> CompPtr = std::dynamic_pointer_cast<T>(Component))
			{
				return std::weak_ptr<T>(CompPtr);
			}
		}
	}
	return std::weak_ptr<T>();
}

template<typename T>
void GameEntity::AddToDrawables(std::shared_ptr<T> InDraw)
{
	assert((std::is_base_of<sf::Drawable, T>::value && "Type is not a drawable type"));
	for (auto& Drawable : Drawables)
	{
		if(InDraw.get() == Drawable.get())
		{
			return;
		}
	}
	Drawables.push_back(InDraw);
}

template<typename T>
std::weak_ptr<T> GameEntity::GetDrawable()
{
	assert((std::is_base_of<sf::Drawable, T>::value && "Type is not a drawable type"));

	for (auto& Drawable : Drawables)
	{
		//if (auto shared = Component)
		{
			if (std::shared_ptr<T> DrawPtr = std::dynamic_pointer_cast<T>(Drawable))
			{
				return std::weak_ptr<T>(DrawPtr);
			}
		}
	}
	return std::weak_ptr<T>();
}
