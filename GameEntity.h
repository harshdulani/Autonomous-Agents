﻿#pragma once
#include <memory>
#include <vector>
#include <cassert>
#include <SFML/Graphics/Transformable.hpp>
#include "Object.h"
#include "System.h"
#include "ObjectManager.h"
#include "Component.h"

namespace sf
{
	class Drawable;
}

namespace sf
{
	class RenderWindow;
}

class GameEntity : public Object, public sf::Transformable
{
	friend class ObjectManager;
public:
	~GameEntity() override = default;
	virtual void Update(float DeltaTime);
	virtual void Render(const sf::RenderWindow* Window) const;

	void Kill() override;
	
	template<typename T>
	std::weak_ptr<T> AddComponent();

	template<typename T>
	std::weak_ptr<T> GetComponent();
	
	template<typename T>
	void AddToDrawables(std::shared_ptr<T> InDraw);

	template<typename T>
	std::weak_ptr<T> GetDrawable();

	std::weak_ptr<GameEntity> GetWeakSelf();
	std::vector<std::weak_ptr<Component>>* GetComponentList();

private:
	//@todo: ownership moved to ObjectMgr
	std::vector<std::weak_ptr<Component>> Components;
	std::vector<std::shared_ptr<sf::Drawable>> Drawables;
};

template<typename T>
std::weak_ptr<T> GameEntity::AddComponent()
{
	std::weak_ptr<T> WeakCmp = System::GetInstance()->GetObjectMgr()->CreateComponent<T>(this);
	
	Components.push_back(WeakCmp);
	return WeakCmp;
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
