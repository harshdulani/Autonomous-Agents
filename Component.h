#pragma once
#include <memory>

class GameEntity;

class Component
{
public:
	virtual ~Component() = default;
	virtual void Update(float DeltaTime);

	GameEntity* GetOwningEntity() const;
	void SetOwningEntity(GameEntity* InEntity);
	
private:
	//@todo: get this from GameEntity->GetSelfPtr()
	GameEntity* OwningEntity = nullptr;
};
