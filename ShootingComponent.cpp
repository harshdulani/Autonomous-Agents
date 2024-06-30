#include "ShootingComponent.h"

#include "GameEntity.h"
#include "System.h"
#include "TimerManager.h"

ShootingComponent::ShootingComponent()
{
	CurrentShootingStrategy = &StandardShooting;
	SineShooting.SetSineWaveProperties(180.f, 7.f);

	TimerMgr = System::GetInstance()->GetTimerManager();

	//when this timer dies, disable shoot cooldown
	ShootTimerHandle = TimerMgr->GetNewTimer();
	TimerMgr->SetTimerEnd(ShootTimerHandle, CurrentShootingStrategy->GetCooldownTime());
	TimerMgr->SetOnComplete(ShootTimerHandle, std::bind(&ShootingComponent::ResetShootCooldown, this));
	TimerMgr->SetKillOnComplete(ShootTimerHandle, false);
}

ShootingComponent::~ShootingComponent()
{
	TimerMgr->SetKillOnComplete(ShootTimerHandle, true);
	TimerMgr->ClearOnComplete(ShootTimerHandle);
}

void ShootingComponent::Update(const float DeltaTime)
{
	SwitchShootStrategy();
	if (!bPlayer)
		return;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (CanShoot())
		{
			ShootBullet();
		}
	}
}

void ShootingComponent::Render(sf::RenderWindow& Window, sf::RenderStates States)
{
	PrimitiveComponent::Render(Window, States);

	if (!bPlayer)
		return;
	
	auto DirVector = GetForwardVector();
	auto Pos = getWorldPosition();

	auto va = sf::VertexArray(sf::Lines, 4);
	auto Color = sf::Color(255, 255, 255, 105);
	va[0] = sf::Vertex(Pos + DirVector * 35.f, Color);
	va[1] = sf::Vertex(Pos + DirVector * 50.f, Color);

	Pos += DirVector * 37.5f;
	DirVector = {-DirVector.y, DirVector.x};

	va[2] = sf::Vertex(Pos + DirVector * 5.f, Color);
	va[3] = sf::Vertex(Pos - DirVector * 5.f, Color);
	
	Window.draw(va);
}

void ShootingComponent::ShootBullet()
{
	if (CurrentShootingStrategy)
	{
		if (CurrentShootingStrategy->TryShoot(*this, bPlayer))
		{
			//if bullet fired successfully
			bShootCooldown = true;
			TimerMgr->ResetTimer(ShootTimerHandle);
		}
	}
}

void ShootingComponent::SwitchShootStrategy()
{
	if (!bPlayer)
	{
		return;
	}	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) // 1 key
	{
		CurrentShootingStrategy = &StandardShooting;
		TimerMgr->ResetTimer(ShootTimerHandle, CurrentShootingStrategy->GetCooldownTime());
		return;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) // 2
	{
		CurrentShootingStrategy = &TripleShooting;
		TimerMgr->ResetTimer(ShootTimerHandle, CurrentShootingStrategy->GetCooldownTime());
		return;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) // 3
	{
		CurrentShootingStrategy = &SineShooting;
		TimerMgr->ResetTimer(ShootTimerHandle, CurrentShootingStrategy->GetCooldownTime());
		return;
	}
}

bool ShootingComponent::CanShoot() const
{
	//bullet shoot cooldown/ interval
	return !bShootCooldown;
}

void ShootingComponent::ResetShootCooldown()
{
	bShootCooldown = false;
}

void ShootingComponent::SetIsPlayer(bool status)
{
	bPlayer = status;
}