#include "Bullet.h"
#include "Math.h"
#include "PhysicsComponent.h"
#include "TimerManager.h"
#include "System.h"
#include "MultiTriShape.h"

Bullet::Bullet()
{
	timerManager_ = System::GetInstance()->GetTimerManager();
	lifeTimerHandle_ = timerManager_->GetNewTimer();
}

Bullet::~Bullet()
{
	timerManager_->Kill(lifeTimerHandle_);
	timerManager_ = nullptr;
}

void Bullet::InitialiseStraightBullet(sf::Vector2f Direction, bool bSingleShot)
{
	bulletFiringMode_ = bSingleShot ? BulletFiringMode::Standard : BulletFiringMode::Triple;
	const float BULLET_SPEED = 200.0f;

	auto PhysicsComp = AddComponent<PhysicsComponent>();
	auto Physics = PhysicsComp.lock();
	if (Physics)
	{
		Physics->SetVelocity(Direction * BULLET_SPEED);
	}

	SetRotation(atan2f(velocity_.y, velocity_.x));

	if (bulletFiringMode_ == BulletFiringMode::Standard)
	{
		auto Square = InitSquareShape(5.f);
		Square.lock()->setFillColor(sf::Color(240, 200, 10)); // yellow
	}
	else// if (myMode_ == BulletFiringMode::Triple)
	{
		auto Spike = InitialiseSpikeShape(sf::Color(240, 90, 175));
		SetScale(0.25f);
	}

	//Lifetime
	timerManager_->SetTimerEnd(lifeTimerHandle_,
							  bulletFiringMode_ == BulletFiringMode::Standard ? 1.0f : 0.65f);
	// mark bullet to be killed after duration
	timerManager_->SetOnComplete(lifeTimerHandle_, [&]() { Kill(); });
}

void Bullet::InitialiseSineWaveBullet(sf::Vector2f Direction, sf::Vector2f Perpend, float WaveAmpl, float WaveFreq)
{
	bulletFiringMode_ = BulletFiringMode::Sine;
	const float BULLET_SPEED = 400.0f;
	
	auto PhysicsComp = AddComponent<PhysicsComponent>();
	auto Physics = PhysicsComp.lock();
	if (Physics)
	{
		Physics->SetVelocity(Direction * BULLET_SPEED);
	}

	SetRotation(atan2f(velocity_.y, velocity_.x));
	waveAmplitude_ = WaveAmpl;
	waveFrequency_ = WaveFreq;
	perpendicular_ = Math::Normalize(Perpend);

	//light pink
	auto Circle = InitialiseCircleShape(3.f);
	Circle.lock()->setFillColor(sf::Color(111, 255, 220));

	timerManager_->SetTimerEnd(lifeTimerHandle_, 0.925f);
	// mark bullet to be killed after duration
	timerManager_->SetOnComplete(lifeTimerHandle_, [&]() { Kill(); });
}

void Bullet::Update(const float DeltaTime)
{
	if (bulletFiringMode_ == BulletFiringMode::Sine)
	{
		//add Sine wave motion on bullet's right axis, along with forward
		auto Position = GetPosition();
		Position += perpendicular_ * cosf(timerManager_->GetTimeSinceSpawn(lifeTimerHandle_) * waveFrequency_) * waveAmplitude_ * DeltaTime;
		SetPosition(Position);
	}
}

void Bullet::OnCollision(std::weak_ptr<GameEntity> other)
{
	if (other.expired())
	{
		return;
	}
	if (auto bullet = std::dynamic_pointer_cast<Bullet>(other.lock()))
	{
		if (bullet->GetIsPlayerOwned() == GetIsPlayerOwned())
		{
			return;
		}
		Kill();
		bullet->Kill();
	}
}

std::weak_ptr<sf::RectangleShape> Bullet::InitSquareShape(float Size)
{
	sf::Vector2f Size2f(Size, Size);
	auto Square = CreateDrawable<sf::RectangleShape>(Size2f);
	Size2f = Size2f * 0.5f;
	SetOrigin(Size * 0.5f, Size * 0.5f);
	return Square;
}

std::weak_ptr<MultiTriShape> Bullet::InitialiseSpikeShape(sf::Color RenderColor)
{
	auto weakSpike = CreateDrawable<MultiTriShape>();
	SetOrigin(15.f, 15.f);
	std::vector<sf::Vector2f> Tris = {{15.f, 0.f},
							{30.f, 30.f},
							{0.f, 30.f},
							{22.5f, 10.f},
							{15.f, 30.f},
							{7.5f, 10.f},
	};
	if (auto Spike = weakSpike.lock())
	{
		Spike->SetTris(Tris, RenderColor);
	}
	return weakSpike;
}

std::weak_ptr<sf::CircleShape> Bullet::InitialiseCircleShape(float Radius)
{
	auto weakCircle = CreateDrawable<sf::CircleShape>(Radius, 24);
	if (auto Circle = weakCircle.lock())
	{
		Circle->setOrigin(Radius * 0.5f, Radius * 0.5f);
	}
	return weakCircle;
}
