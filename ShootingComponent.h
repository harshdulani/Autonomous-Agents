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

	AShootingStrategy* CurrentShootingStrategy = nullptr;

	StandardShootingStrategy StandardShooting;
	TripleShootingStrategy TripleShooting;
	SineShootingStrategy SineShooting;

	bool bPlayer = false;

	TimerManager* TimerMgr = nullptr;

	int ShootTimerHandle = -1;
	bool bShootCooldown = false;
};

