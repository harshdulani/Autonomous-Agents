#pragma once

#include "BaseVehicle.h"
#include <memory>

class InputComponent;
class Graphics;
class Keyboard;
class Game;
class Timer;
class ShootingComponent;

class PlayerShip : public BaseVehicle
{
public:
	~PlayerShip() override;

	void Update(float DeltaTime) override;

	void InitialiseComponents();

	// Collisions
	void OnCollision(std::weak_ptr<GameEntity> WeakOther) override;
	bool IsCollisionAllowed() const;

	//For maintaining lives across levels
	void SetLives(int Lives);

protected:
	void UpdateTransform(float DeltaTime);
	bool LoseALife() override;
	
private:
	void SetupControlMappings();
	
private:
	std::weak_ptr<InputComponent> inputComp_;
	
	float accelerationControl_ = 0.f;
	float rotationControl_ = 0.f;
	bool bNos_ = false;
	
	float inactiveNos_ = 1.f;
	float activeNos_ = 2.f;
};
