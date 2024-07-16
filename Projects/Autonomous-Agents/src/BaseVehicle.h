#pragma once

#include "pch.h"
#include "GameEntity.h"

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
	int GetTotalLives() const { return totalLives_; }
	
	class PhysicsComponent* GetPhysicsComponent() const;
	
protected:
	TimerManager* GetTimerManager() const;
	
	//Reset all ship values
	void Reset();

	// Returns if ship survived after losing a life
	virtual bool LoseALife();
	
	bool GetIsInCollisionCooldown() const { return bCollisionCooldown_; }
	
	void SetAllCollidersStatus(bool bEnabled);
	void ResetCollisionCooldown();

	std::weak_ptr<class ShootingComponent> ShootingComp;

	// lives
	int livesRemaining_;
	int totalLives_ = 1;

	uint32_t collisionTimerHandle_ = 0;
	bool bCollisionCooldown_ = false;

private:
	std::weak_ptr<class PhysicsComponent> physicsC_;
	
	TimerManager* timerManager_;	
};