#include "BaseVehicle.h"
#include "System.h"
#include "TimerManager.h"
#include <functional>
#include "Physics/Collider.h"
#include <memory>
#include <vector>
#include "Maths.h"
#include "Physics/PhysicsComponent.h"

BaseVehicle::BaseVehicle()
{
	livesRemaining_ = totalLives_;

	timerManager_ = System::GetInstance()->GetTimerManager();

	collisionTimerHandle_ = GetTimerManager()->GetNewTimer();
	GetTimerManager()->SetTimerEnd(collisionTimerHandle_, 1);
	GetTimerManager()->SetOnComplete(collisionTimerHandle_, [this] { ResetCollisionCooldown(); });
	GetTimerManager()->SetKillOnComplete(collisionTimerHandle_, false);
}

BaseVehicle::~BaseVehicle()
{
	GetTimerManager()->SetKillOnComplete(collisionTimerHandle_, true);
	GetTimerManager()->ClearOnComplete(collisionTimerHandle_);
}

void BaseVehicle::InitVehicle()
{
	//Physics Component
	physicsC_ = AddComponent<PhysicsComponent>();
}

void BaseVehicle::SetAllCollidersStatus(const bool bEnabled)
{
	std::vector<std::weak_ptr<Collider>> colliderList;
	GetComponentsOfType<Collider>(colliderList);
	for (auto& collider : colliderList)
	{
		if (std::shared_ptr<Collider> item = collider.lock())
		{
			item->SetEnabled(bEnabled);
		}
	}
}

void BaseVehicle::ResetCollisionCooldown()
{
	bCollisionCooldown_ = false;
	SetAllCollidersStatus(true);
}

bool BaseVehicle::LoseALife()
{
	if (livesRemaining_ == 0) return false;

	--livesRemaining_;

	return livesRemaining_ > 0;
}

TimerManager* BaseVehicle::GetTimerManager() const { return timerManager_; }

PhysicsComponent* BaseVehicle::GetPhysicsComponent() const { return physicsC_.lock().get(); }

void BaseVehicle::SetTotalLives(int count)
{
	totalLives_ = count;
}

void BaseVehicle::Reset()
{
	if(auto Physics = physicsC_.lock())
	{
		Physics->SetVelocity(Maths::ZeroVector());
	}

	SetPosition(Maths::ZeroVector());
	SetRotation(0.f);
	// bullet mode set to standard
}

void BaseVehicle::ResetLives()
{
	livesRemaining_ = totalLives_;
}

int BaseVehicle::GetLivesLeft() const
{
	return livesRemaining_;
}