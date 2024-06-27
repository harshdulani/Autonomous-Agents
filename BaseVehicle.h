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
	
	void SetTotalLives(int count);
	int GetTotalLives() const { return TotalLives; }

	//Reset all ship values
	void Reset();

	// reset ship lives
	void ResetLives();

	// Returns number of lives left
	int GetLivesLeft() const;

	// Returns if ship survived after losing a life
	bool LoseALife();

	bool GetIsInCollisionCooldown() const { return bCollisionCooldown; }
	
protected:
	TimerManager* GetTimerManager() const;

	PhysicsComponent* GetPhysicsComponent() const;

	void SetAllCollidersStatus(bool bEnabled);
	void ResetCollisionCooldown();

	std::weak_ptr<ShootingComponent> ShootingComp;

	// lives
	int LivesRemaining;
	int TotalLives = 1;

	int CollisionTimerHandle = -1;
	bool bCollisionCooldown = false;

private:
	std::weak_ptr<PhysicsComponent> PhysicsC;
	
	TimerManager* TimerManager;	
};