#include "ShootingComponent.h"

#include "GameEntity.h"
#include "System.h"
#include "TimerManager.h"

ShootingComponent::ShootingComponent()
{
	currentShootingStrategy_ = &standardShooting_;
	sineShooting_.SetSineWaveProperties(180.f, 7.f);

	timerMgr_ = System::GetInstance()->GetTimerManager();

	//when this timer dies, disable shoot cooldown
	shootTimerHandle_ = timerMgr_->GetNewTimer();
	timerMgr_->SetTimerEnd(shootTimerHandle_, currentShootingStrategy_->GetCooldownTime());
	timerMgr_->SetOnComplete(shootTimerHandle_, std::bind(&ShootingComponent::ResetShootCooldown, this));
	timerMgr_->SetKillOnComplete(shootTimerHandle_, false);
}

ShootingComponent::~ShootingComponent()
{
	timerMgr_->SetKillOnComplete(shootTimerHandle_, true);
	timerMgr_->ClearOnComplete(shootTimerHandle_);
}

void ShootingComponent::Update(const float DeltaTime)
{
	SwitchShootStrategy();
	if (!bPlayer_)
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

	if (!bPlayer_)
		return;
	
	auto DirVector = GetForwardVector();
	auto Pos = GetWorldPosition();

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
	if (currentShootingStrategy_)
	{
		if (currentShootingStrategy_->TryShoot(*this, bPlayer_))
		{
			//if bullet fired successfully
			bShootCooldown_ = true;
			timerMgr_->ResetTimer(shootTimerHandle_);
		}
	}
}

void ShootingComponent::SwitchShootStrategy()
{
	if (!bPlayer_)
	{
		return;
	}	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) // 1 key
	{
		currentShootingStrategy_ = &standardShooting_;
		timerMgr_->ResetTimer(shootTimerHandle_, currentShootingStrategy_->GetCooldownTime());
		return;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) // 2
	{
		currentShootingStrategy_ = &tripleShooting_;
		timerMgr_->ResetTimer(shootTimerHandle_, currentShootingStrategy_->GetCooldownTime());
		return;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) // 3
	{
		currentShootingStrategy_ = &sineShooting_;
		timerMgr_->ResetTimer(shootTimerHandle_, currentShootingStrategy_->GetCooldownTime());
		return;
	}
}

bool ShootingComponent::CanShoot() const
{
	//bullet shoot cooldown/ interval
	return !bShootCooldown_;
}

void ShootingComponent::ResetShootCooldown()
{
	bShootCooldown_ = false;
}

void ShootingComponent::SetIsPlayer(bool status)
{
	bPlayer_ = status;
}