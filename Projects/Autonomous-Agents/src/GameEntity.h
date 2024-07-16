#pragma once
#include "pch.h"
#include "Object.h"
#include "System.h"
#include "Component.h"

class GameEntity : public Object, private sf::Transformable
{
	friend class ObjectManager;
// Core
public:
	~GameEntity() override;
	virtual void Update(float DeltaTime);
	virtual void Render(sf::RenderWindow& Window) const;
	void Kill() override;

	void SetActive(bool bStatus);
	bool GetActive() const;

protected:
	GameEntity() = default;

private:
	bool bActive_ = true;
// End core

// Components & Drawables
public:
	virtual void OnCollision(std::weak_ptr<GameEntity> other);
	void CreateCollider(float Radius);
	bool HasColliders() const;

	std::vector<std::weak_ptr<Component>>* GetComponentList();
	
	/// <summary>
	/// Creates component of type T and attaches it to entity
	/// </summary>
	/// <typeparam name="T">T must be a Component/ must inherit from Component class</typeparam>
	/// <returns>Weak_ptr to newly created Component</returns>
	template<typename T>	std::weak_ptr<T> AddComponent();
	
	template<typename T>	std::weak_ptr<T> GetComponentOfType();

	template<typename T>	void GetComponentsOfType(std::vector<std::weak_ptr<T>>& outResult);

	template<typename T>	void DestroyComponentOfType();

	template<typename T>	void DestroyAllComponentsOfType();
	
	template<typename T, class... Types>	std::weak_ptr<T> CreateDrawable(Types... args);
	
	template<typename T>	std::weak_ptr<T> GetDrawableOfType();
	
	void DestroyAllDrawables();

	void DestroyDrawable(const std::weak_ptr<sf::Drawable>& Drawable);

	std::weak_ptr<GameEntity> GetWeakSelf();

private:
	template<typename T>	void DestroyComponentsOfTypeInternal_(bool onlyOne);
	
	std::vector<std::weak_ptr<Component>> components_;
	std::vector<std::shared_ptr<sf::Drawable>> drawables_;

	bool bHasColliders_ = false;
// End Components & Drawables

// Render and Update Priority
public:
	// Lower values get updated first, Higher later
	void SetRenderPriority(int p);
	int GetRenderPriority() const;
	// Lower values get updated first, Higher later
	void SetUpdatePriority(int p);
	int GetUpdatePriority() const;

private:
	bool IsRenderDirty() const;
	void SetRenderDirty(bool Cond);
	bool IsUpdateDirty() const;
	void SetUpdateDirty(bool Cond);

	int updatePriority_ = 0;
	int renderPriority_ = 0;
	bool bRenderDirty_ = false;
	bool bUpdateDirty_ = false;

// End Render and Update Priority

// Transform Wrapper
public:
	sf::Transform GetTransform() const;

	sf::Vector2f GetOrigin() const;
	void SetOrigin(float X, float Y);
	
	sf::Vector2f GetPosition() const;
	float GetRotation() const;
	sf::Vector2f GetScale() const;

	void SetPosition(const sf::Vector2f& NewPos);
	void SetRotation(float NewAngle);
	void SetScale(const sf::Vector2f& NewScale);
	void SetScale(float NewScale);
	
	sf::Vector2f GetForwardVector() const;
	sf::Vector2f GetRightVector() const;
	
private:
	bool GetAndClearDirtyPosition();
	bool GetAndClearDirtyAngle();

private:
	bool bPositionDirty_ = true;
	bool bRotationDirty_ = true;
// End Transform Wrapper
};

// Component Template definitions
template<typename T>
std::weak_ptr<T> GameEntity::AddComponent()
{
	std::weak_ptr<T> WeakCmp = System::GetInstance()->GetObjectMgr()->CreateComponent<T>(this);

	components_.push_back(WeakCmp);
	return WeakCmp;
}

template<typename T> std::weak_ptr<T> GameEntity::GetComponentOfType()
{
	static_assert((std::is_base_of<Component, T>::value && "Type is not a component type"));

	for (auto& Component : components_)
	{
		if (auto shared = Component.lock())
		{
			if (std::shared_ptr<T> CompPtr = std::dynamic_pointer_cast<T>(shared))
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
	static_assert((std::is_base_of<Component, T>::value && "Type is not a component type"));

	for (auto& comp : components_)
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
	if (std::is_same<T, class Collider>::value)
	{
		bHasColliders_ = false;
	}
	DestroyComponentsOfTypeInternal_<T>(false);
}

template<typename T>
inline void GameEntity::DestroyComponentsOfTypeInternal_(const bool onlyOne)
{
	static_assert((std::is_base_of<Component, T>::value && "Type mismatch while destroying object"));

	int i = 0;
	for (auto& comp : components_)
	{
		auto component = comp.lock();
		if (std::dynamic_pointer_cast<T>(component))
		{
			System::GetInstance()->GetObjectMgr()->DestroyObject(component.get());
			components_.erase(components_.begin() + i);
			if (onlyOne)
			{
				if (static_cast<int>(components_.size()) == 1 && std::is_same<T, class Collider>::value)
				{
					bHasColliders_ = false;
				}
				return;
			}
		}
		i++;
	}
}

template<typename T, typename ... Types> std::weak_ptr<T> GameEntity::CreateDrawable(Types... args)
{
	static_assert((std::is_base_of<sf::Drawable, T>::value && "Type is not a drawable type"));
	std::shared_ptr<T> Drawable = std::make_shared<T>(args...);
	drawables_.push_back(Drawable);
	return Drawable;
}

template<typename T>
std::weak_ptr<T> GameEntity::GetDrawableOfType()
{
	static_assert((std::is_base_of<sf::Drawable, T>::value && "Type is not a drawable type"));

	for (auto& Drawable : drawables_)
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
//End Component Template definitions