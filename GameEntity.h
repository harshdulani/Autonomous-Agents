#pragma once
#include <memory>
#include <vector>
#include <cassert>
#include <SFML/Graphics/Transformable.hpp>
#include "Object.h"
#include "System.h"
#include "ObjectManager.h"
#include "Component.h"

class Collider;

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

	void SetActive(bool bStatus);
	bool GetActive() const;

	void SetRenderPriority(int p);
	int GetRenderPriority() const;

	void SetUpdatePriority(int p);
	int GetUpdatePriority() const;

	sf::Vector2f GetForwardVector() const;
	sf::Vector2f GetRightVector() const;

	/// <summary>
	/// Creates component of type T and attaches it to entity
	/// </summary>
	/// <typeparam name="T">T must be a Component/ must inherit from Component class</typeparam>
	/// <returns>Weak_ptr to newly created Component</returns>
	template<typename T> std::weak_ptr<T> AddComponent();

	template<typename T> std::weak_ptr<T> GetComponentOfType();

	template<typename T> void GetComponentsOfType(std::vector<std::weak_ptr<T>>& outResult);

	template<typename T> void AddToDrawables(std::shared_ptr<T> InDraw);

	template<typename T> std::weak_ptr<T> GetDrawableOfType();

	std::vector<std::weak_ptr<Component>>* GetComponentList();

protected:
	GameEntity() = default;
	
	template<typename T>
	void DestroyComponentOfType();

	template<typename T>
	void DestroyAllComponentsOfType();

	std::weak_ptr<GameEntity> GetWeakSelf();

private:
	template<typename T>
	void DestroyComponentsOfTypeInternal_(const bool onlyOne);

	bool IsRenderDirty() const;
	void SetRenderDirty(bool Cond);
	bool IsUpdateDirty() const;
	void SetUpdateDirty(bool Cond);

	std::vector<std::weak_ptr<Component>> Components;
	std::vector<std::shared_ptr<sf::Drawable>> Drawables;

	bool bHasColliders = false;

	int UpdatePriority = 0;
	int RenderPriority = 0;
	bool bRenderDirty = false;
	bool bUpdateDirty = false;

	bool bActive = true;
};

template<typename T>
std::weak_ptr<T> GameEntity::AddComponent()
{
	std::weak_ptr<T> WeakCmp = System::GetInstance()->GetObjectMgr()->CreateComponent<T>(this);

	Components.push_back(WeakCmp);
	return WeakCmp;
}

template<typename T> std::weak_ptr<T> GameEntity::GetComponentOfType()
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
void GameEntity::GetComponentsOfType(std::vector<std::weak_ptr<T>>& outResult)
{
	assert((std::is_base_of<Component, T>::value && "Type is not a component type"));

	for (auto& comp : Components)
	{
		if (auto shared = comp.lock())
		{
			if (std::shared_ptr<T> compPtr = std::dynamic_pointer_cast<T>(shared))
			{
				outResult.push_back(compPtr);
			}
		}
	}
}

template<typename T>
inline void GameEntity::DestroyComponentOfType()
{
	DestroyComponentsOfTypeInternal_<T>(true);
}

template<typename T>
inline void GameEntity::DestroyAllComponentsOfType()
{
	if (std::is_same<T, Collider>::value)
	{
		bHasColliders = false;
	}
	DestroyComponentsOfTypeInternal_<T>(false);
}

template<typename T>
inline void GameEntity::DestroyComponentsOfTypeInternal_(const bool onlyOne)
{
	assert((std::is_base_of<Component, T>::value && "Type mismatch while destroying object"));

	int i = 0;
	for (auto& comp : Components)
	{
		auto component = comp.lock();
		if (dynamic_cast<T*>(component.get()))
		{
			System::GetInstance()->GetObjectMgr()->DestroyObject(component.get());
			Components.erase(Components.begin() + i);
			if (onlyOne)
			{
				if (static_cast<int>(Components.size()) == 1 && std::is_same<T, Collider>::value)
				{
					bHasColliders = false;
				}
				return;
			}
		}
		i++;
	}
}

template<typename T>
void GameEntity::AddToDrawables(std::shared_ptr<T> InDraw)
{
	assert((std::is_base_of<sf::Drawable, T>::value && "Type is not a drawable type"));
	for (auto& Drawable : Drawables)
	{
		if (InDraw.get() == Drawable.get())
		{
			return;
		}
	}
	Drawables.push_back(InDraw);
}

template<typename T>
std::weak_ptr<T> GameEntity::GetDrawableOfType()
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
