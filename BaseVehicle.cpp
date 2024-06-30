#include "BaseVehicle.h"
#include "System.h"
#include "TimerManager.h"
#include <functional>
#include "Collider.h"
#include <memory>
#include <vector>
#include "PhysicsComponent.h"

BaseVehicle::BaseVehicle()
{
	LivesRemaining = TotalLives;

	TimerManager = System::GetInstance()->GetTimerManager();

	CollisionTimerHandle = TimerManager->GetNewTimer();
	GetTimerManager()->SetTimerEnd(CollisionTimerHandle, 1);
	GetTimerManager()->SetOnComplete(CollisionTimerHandle, [this] { ResetCollisionCooldown(); });
	GetTimerManager()->SetKillOnComplete(CollisionTimerHandle, false);
}

BaseVehicle::~BaseVehicle()
{
	GetTimerManager()->SetKillOnComplete(CollisionTimerHandle, true);
	GetTimerManager()->ClearOnComplete(CollisionTimerHandle);
}

void BaseVehicle::InitVehicle()
{
	//Physics Component
	PhysicsC = AddComponent<PhysicsComponent>();
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
	bCollisionCooldown = false;
	SetAllCollidersStatus(true);
}

bool BaseVehicle::LoseALife()
{
	if (LivesRemaining == 0) return false;

	--LivesRemaining;

	return LivesRemaining > 0;
}

TimerManager* BaseVehicle::GetTimerManager() const { return TimerManager; }

PhysicsComponent* BaseVehicle::GetPhysicsComponent() const { return PhysicsC.lock().get(); }

void BaseVehicle::SetTotalLives(int count)
{
	TotalLives = count;
}

void BaseVehicle::Reset()
{
	if(auto Physics = PhysicsC.lock())
	{
		Physics->SetVelocity({});
	}

	SetPosition({});
	SetRotation(0.f);
	// bullet mode set to standard
}

void BaseVehicle::ResetLives()
{
	LivesRemaining = TotalLives;
}

int BaseVehicle::GetLivesLeft() const
{
	return LivesRemaining;
}