#include "PlayerShip.h"
#include "Math.h"
#include "TimerManager.h"
#include "Bullet.h"
//#include "ScreenShaker.h"
#include "Asteroid.h"
//#include "ParticleSystem.h"
//#include "ParticleSystemManager.h"
#include "Debug.h"
#include "ShootingComponent.h"

PlayerShip::~PlayerShip()
{
	GetTimerManager()->SetKillOnComplete(CollisionTimerHandle, true);
	GetTimerManager()->ClearOnComplete(CollisionTimerHandle);
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
	/*
	//Particle system
	auto particleSys = System::GetInstance()->GetParticleSystemManager()->CreateNewParticleSystem(this);
	std::shared_ptr<ParticleSystem> p = particleSys.lock();

	p->SetLocalPosition(XMFLOAT3(0.f, 0.f, 0.f));
	p->SetLocalAngle(XMConvertToRadians(180.f));

	p->SetLooping(true)
		.SetDuration(0.5f)

		.SetEmissionShape(LogicalShape::HLineSamplingFunction, 2)
		.SetEmissionMode(EmissionMode::Random)
		.SetEmitterRotationMode(TransformSpace::Local)
		.SetEmissionVelocityMode(EmissionVelocityMode::Normal)
		.SetEmitterScaling(1.f)
		.SetEmitterRate(32.f)
		.SetEmissionSpeed(150.f)

		.SetParticleShape(LogicalShape::VLineSamplingFunction, 2)
		.SetParticleScale(5.f)
		.SetParticleLifeTime(0.15f, 0.45f)
		.SetColorOverLifeTime(Helpers::GetColorFromRGB(255, 155, 0), 0);
*/
	//Shooting
	ShootingComp = AddComponent<ShootingComponent>();
	auto Shooter = ShootingComp.lock();
	Shooter->SetIsPlayer(true);
	Shooter->setLocalPosition({0.f, -15.f});

	// Appearance
	SetOrigin(15.f, 15.f);	
	auto MultiTris = std::make_shared<MultiTriShape>();
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
	MultiTris->SetTris(Tris, {69, 204, 255});
	MultiTris->setScale(1.f, 1.f);
	AddToDrawables(MultiTris);
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
			bCollisionCooldown = true;
			GetTimerManager()->ResetTimer(CollisionTimerHandle);
			SetAllCollidersStatus(false);

			//System::GetInstance()->GetGame().UpdateLivesRemaining(LivesRemaining);
			//System::GetInstance()->GetScreenShaker()->CreateImpulse(0.35f, 5.0f, 25.0f);
			return;
		}

		// if player doesn't have any lives remaining
		//System::GetInstance()->GetScreenShaker()->CreateImpulse(0.5f, 5.0f, 25.0f);
		Kill();
	}
}

void PlayerShip::SetControlInput()
{
	AccelerationControl = 0.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		AccelerationControl = 1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		AccelerationControl = -1.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		AccelerationControl *= NosMultiplier;
	}

	RotationControl = 0.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		RotationControl = 1.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		RotationControl = -1.0f;
	}
}

void PlayerShip::UpdateTransform(const float DeltaTime)
{
	constexpr float RateOfRotation = 360.f * 0.35f;
	constexpr float MaxSpeed = 150.0f;
	constexpr float TweenVelocity = 0.05f;

	//Rotation
	auto angle = Math::WrapModulo(GetRotation() + 
								   RotationControl * RateOfRotation * DeltaTime,
								   360.f);
	SetRotation(angle);

	sf::Vector2f NewForward = GetForwardVector();

	auto Physics = GetPhysicsComponent();
	if (!Physics)
		return;
	//Movement
	sf::Vector2f idealVelocity = NewForward * (AccelerationControl * MaxSpeed);
	sf::Vector2f newVelocity = Math::LerpVector(Physics->GetVelocity(), idealVelocity, TweenVelocity);
	Physics->SetVelocity(newVelocity);
}

bool PlayerShip::IsCollisionAllowed() const
{
	return !GetIsInCollisionCooldown();
}

void PlayerShip::SetLives(int Lives)
{
	LivesRemaining = Lives;
}
