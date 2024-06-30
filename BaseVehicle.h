#pragma once

#include "GameEntity.h"
#include <memory>
#include <vector>

class PhysicsComponent;
class ShootingComponent;
class TimerManager;

class BaseVehicle : public GameEntity
{
public:
	BaseVehicle();
	~BaseVehicle() override;

	void InitVehicle();
	
	// reset ship lives
	void ResetLives();

	// Returns number of lives left
	int GetLivesLeft() const;
	
	void SetTotalLives(int count);
	int GetTotalLives() const { return TotalLives; }
	
	PhysicsComponent* GetPhysicsComponent() const;
	
protected:
	TimerManager* GetTimerManager() const;
	
	//Reset all ship values
	void Reset();

	// Returns if ship survived after losing a life
	virtual bool LoseALife();
	
	bool GetIsInCollisionCooldown() const { return bCollisionCooldown; }
	
	void SetAllCollidersStatus(bool bEnabled);
	void ResetCollisionCooldown();

	std::weak_ptr<ShootingComponent> ShootingComp;

	// lives
	int LivesRemaining;
	int TotalLives = 1;

	uint32_t CollisionTimerHandle = 0;
	bool bCollisionCooldown = false;

private:
	std::weak_ptr<PhysicsComponent> PhysicsC;
	
	TimerManager* TimerManager;	
};