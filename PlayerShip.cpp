#include "PlayerShip.h"
#include "Math.h"
#include "TimerManager.h"
#include "Bullet.h"
#include "ScreenShaker.h"
#include "Asteroid.h"
#include "Debug.h"
#include "ShootingComponent.h"
#include "Collider.h"
#include "ParticleSystem/ParticleSystem.h"
#include "ParticleSystem/ParticleSystemManager.h"

PlayerShip::~PlayerShip()
{
	GetTimerManager()->SetKillOnComplete(collisionTimerHandle_, true);
	GetTimerManager()->ClearOnComplete(collisionTimerHandle_);
}

void PlayerShip::Update(float DeltaTime)
{
	BaseVehicle::Update(DeltaTime);
	UpdateTransform(DeltaTime);
	SetControlInput();
}

void PlayerShip::InitialiseComponents()
{
	InitVehicle();

	System::GetInstance()->GetGame().Event_LivesUpdate.Invoke(GetLivesLeft());
	
	//Shooting
	ShootingComp = AddComponent<ShootingComponent>();
	auto Shooter = ShootingComp.lock();
	Shooter->SetIsPlayer(true);
	Shooter->SetLocalPosition({0.f, -15.f});

	CreateCollider(15.0f);
	auto weakCollider = GetComponentOfType<Collider>();
	if(auto collider = weakCollider.lock())
	{
		collider->SetColliderVisible(false);
	}

	// Appearance
	auto MultiTris = CreateDrawable<MultiTriShape>();
	if (auto shape = MultiTris.lock())
	{
		std::vector<sf::Vector2f> Tris = {
			{0.f, 20.f},
			{12.5f, 0.f},
			{10.f, 20.f},
			{17.5f, 0.f},
			{30.f, 20.f},
			{20.f, 20.f},
			{0.f, 20.f},
			{30.f, 20.f},
			{15.f, 30.f}
		};
		shape->SetTris(Tris, {69, 204, 255});
		shape->setScale(1.f, 1.f);
		shape->setPosition(-7.5f, -7.5f);
	}
	
	//Particle system
	auto particleSys = System::GetInstance()->GetParticleSystemManager()->CreateNewParticleSystem(this);
	std::shared_ptr<ParticleSystem> p = particleSys.lock();

	p->SetLocalPosition({0.f, 15.f});
	p->SetLocalRotation(180.f);

	p->SetLooping(true)
		.SetDuration(0.25f)

		.SetEmissionShape(LogicalParticleShape::HLineSamplingFunction, 2)
		.SetEmissionMode(EmissionMode::Random)
		.SetEmitterRotationMode(TransformSpace::Local)
		.SetEmissionVelocityMode(EmissionVelocityMode::Normal)
		.SetEmitterScaling(1.f)
		.SetEmitterRate(32.f)
		.SetEmissionSpeed(150.f)

		.SetParticleShape(LogicalParticleShape::VLineSamplingFunction, 2)
		.SetParticleScale(5.f)
		.SetParticleLifeTime(0.15f, 0.45f)
		.SetColorOverLifeTime(sf::Color(255, 155, 0), sf::Color::Black);
	
}

void PlayerShip::OnCollision(std::weak_ptr<GameEntity> WeakOther)
{
	if (!IsCollisionAllowed())
	{
		return;
	}
	auto Other = WeakOther.lock();
	auto OtherBullet = std::dynamic_pointer_cast<Bullet>(Other);
	
	if(OtherBullet || std::dynamic_pointer_cast<Asteroid>(Other))
	{
		if (OtherBullet)
		{
			if (OtherBullet->GetIsPlayerOwned())
			{
				return;
			}
			OtherBullet->Kill();
		}

		if (LoseALife())
		{
			// if player survives collision/ losing a life
			bCollisionCooldown_ = true;
			GetTimerManager()->ResetTimer(collisionTimerHandle_);
			SetAllCollidersStatus(false);

			System::GetInstance()->GetGame().Event_LivesUpdate.Invoke(livesRemaining_);
			System::GetInstance()->GetScreenShaker()->CreateImpulse(0.35f, 5.0f, 25.0f);
			return;
		}

		// if player doesn't have any lives remaining
		System::GetInstance()->GetScreenShaker()->CreateImpulse(0.5f, 5.0f, 25.0f);
		Kill();
	}
}

void PlayerShip::SetControlInput()
{
	accelerationControl_ = 0.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		accelerationControl_ = 1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		accelerationControl_ = -1.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		accelerationControl_ *= nosMultiplier_;
	}

	rotationControl_ = 0.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		rotationControl_ = 1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		rotationControl_ = -1.0f;
	}
}

void PlayerShip::UpdateTransform(const float DeltaTime)
{
	constexpr float RateOfRotation = 360.f * 0.35f;
	constexpr float MaxSpeed = 150.0f;
	constexpr float TweenVelocity = 0.05f;

	//Rotation
	auto angle = Math::WrapModulo(GetRotation() + 
								   rotationControl_ * RateOfRotation * DeltaTime,
								   360.f);
	SetRotation(angle);

	sf::Vector2f NewForward = GetForwardVector();

	auto Physics = GetPhysicsComponent();
	if (!Physics)
		return;
	//Movement
	sf::Vector2f idealVelocity = NewForward * (accelerationControl_ * MaxSpeed);
	sf::Vector2f newVelocity = Math::LerpVector(Physics->GetVelocity(), idealVelocity, TweenVelocity);
	Physics->SetVelocity(newVelocity);
}

bool PlayerShip::LoseALife()
{
	System::GetInstance()->GetGame().Event_LivesUpdate.Invoke(GetLivesLeft());
	return BaseVehicle::LoseALife();
}

bool PlayerShip::IsCollisionAllowed() const
{
	return !GetIsInCollisionCooldown();
}

void PlayerShip::SetLives(int Lives)
{
	livesRemaining_ = Lives;
}
