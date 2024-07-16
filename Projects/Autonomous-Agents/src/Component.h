#pragma once
#include "pch.h"
#include "Object.h"

class GameEntity;

class Component : public Object
{
	friend class ObjectManager;
public:
	virtual void Update(float DeltaTime);

	void SetEnabled(bool bStatus);
	bool IsEnabled() const;
	
	std::weak_ptr<GameEntity> GetOwningEntity() const;
	void SetOwningEntity(std::weak_ptr<GameEntity> InEntity);

protected:
	Component() = default;
	
private:
	std::weak_ptr<GameEntity> owningEntity_;
	bool bEnabled_ = true;
};
