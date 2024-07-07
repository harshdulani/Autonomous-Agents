#pragma once

#include "AShootingStrategy.h"
#include "PrimitiveComponent.h"

class TimerManager;
class Keyboard;

class ShootingComponent : public PrimitiveComponent
{
public:
	ShootingComponent();
	~ShootingComponent() override;
	
	void Update(float DeltaTime) override;
	void Render(sf::RenderWindow& Window, sf::RenderStates States) override;

	void SetIsPlayer(bool status);
	void ShootBullet();
	void SwitchShootStrategy();
	bool CanShoot() const;

private:
	void ResetShootCooldown();

	AShootingStrategy* currentShootingStrategy_ = nullptr;

	StandardShootingStrategy standardShooting_;
	TripleShootingStrategy tripleShooting_;
	SineShootingStrategy sineShooting_;

	bool bPlayer_ = false;

	TimerManager* timerMgr_ = nullptr;

	uint32_t shootTimerHandle_ = 0;
	bool bShootCooldown_ = false;
};

