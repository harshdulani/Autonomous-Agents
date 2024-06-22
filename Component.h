#pragma once
#include "Object.h"

class GameEntity;

class Component : public Object
{
public:
	~Component() override = default;
	virtual void Update(float DeltaTime);

	GameEntity* GetOwningEntity() const;
	void SetOwningEntity(GameEntity* InEntity);
	
private:
	//@todo: get this from GameEntity->GetSelfPtr()
	GameEntity* OwningEntity = nullptr;
};
