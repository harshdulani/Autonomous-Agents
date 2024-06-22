#pragma once
#include "Object.h"
#include <memory>

class GameEntity;

class Component : public Object
{
	friend class ObjectManager;
public:
	virtual void Update(float DeltaTime);

	std::weak_ptr<GameEntity> GetOwningEntity() const;
	void SetOwningEntity(std::weak_ptr<GameEntity> InEntity);

protected:
	Component() = default;
	
private:
	std::weak_ptr<GameEntity> OwningEntity;
};
