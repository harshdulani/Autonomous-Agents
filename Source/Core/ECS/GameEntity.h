#pragma once
#include <memory>
#include <vector>
#include "Object.h"
#include "System.h"
#include "Component.h"
#include <SFML/Graphics/Transformable.hpp>

class Collider;
class ObjectManager;

namespace sf
{
	class Drawable;
}

namespace sf
{
	class RenderWindow;
}

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

#include "GameEntity.inl"
