#include "pch.h"
#include "PlayerShip.h"
#include "Maths.h"
#include "TimerManager.h"
#include "Bullet.h"
#include "ScreenShaker.h"
#include "Asteroid.h"
#include "Debug.h"
#include "ShootingComponent.h"
#include "Collider.h"
#include "Input/InputComponent.h"
#include "ParticleSystem/ParticleSystem.h"
#include "ParticleSystem/ParticleSystemManager.h"
#include "GameEntity.h"
#include "ObjectManager.h"

PlayerShip::~PlayerShip()
{
	GetTimerManager()->SetKillOnComplete(collisionTimerHandle_, true);
	GetTimerManager()->ClearOnComplete(collisionTimerHandle_);
}

void PlayerShip::Update(float DeltaTime)
{
	BaseVehicle::Update(DeltaTime);
	UpdateTransform(DeltaTime);

	// reset these values to have fresh ones assigned in the next frame
	accelerationControl_ = 0.f;
	rotationControl_ = 0.f;
	bNos_ = false;
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

	//controls
	inputComp_ = AddComponent<InputComponent>();
	auto Input = inputComp_.lock();
	SetupControlMappings();

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
	sf::Vector2f idealVelocity = NewForward * (accelerationControl_ * (bNos_ ? activeNos_ : inactiveNos_) * MaxSpeed);
	sf::Vector2f newVelocity = Math::LerpVector(Physics->GetVelocity(), idealVelocity, TweenVelocity);
	Physics->SetVelocity(newVelocity);
}

bool PlayerShip::LoseALife()
{
	System::GetInstance()->GetGame().Event_LivesUpdate.Invoke(GetLivesLeft());
	return BaseVehicle::LoseALife();
}

void PlayerShip::SetupControlMappings()
{
	auto InputC = inputComp_.lock();
	//acceleration
	InputC->MapControlBinding(sf::Keyboard::Scancode::W,
					  InputHandler::EButtonState::EBS_Held,
					  [&]() -> void { accelerationControl_ = 1.f; });
	InputC->MapControlBinding(sf::Keyboard::Scancode::Up,
					  InputHandler::EButtonState::EBS_Held,
					  [&]() -> void { accelerationControl_ = 1.f; });
	InputC->MapControlBinding(sf::Keyboard::Scancode::S,
					  InputHandler::EButtonState::EBS_Held,
					  [&]() -> void { accelerationControl_ = -1.f; });
	InputC->MapControlBinding(sf::Keyboard::Scancode::Down,
					  InputHandler::EButtonState::EBS_Held,
					  [&]() -> void { accelerationControl_ = -1.f; });
	//rotation
	InputC->MapControlBinding(sf::Keyboard::Scancode::A,
					  InputHandler::EButtonState::EBS_Held,
					  [&]() -> void { rotationControl_ = -1.f; });
	InputC->MapControlBinding(sf::Keyboard::Scancode::Left,
					  InputHandler::EButtonState::EBS_Held,
					  [&]() -> void { rotationControl_ = -1.f; });
	InputC->MapControlBinding(sf::Keyboard::Scancode::D,
					  InputHandler::EButtonState::EBS_Held,
					  [&]() -> void { rotationControl_ = 1.f; });
	InputC->MapControlBinding(sf::Keyboard::Scancode::Right,
					  InputHandler::EButtonState::EBS_Held,
					  [&]() -> void { rotationControl_ = 1.f; });
	// nos
	InputC->MapControlBinding(sf::Keyboard::Scancode::LShift,
					  InputHandler::EButtonState::EBS_Held,
					  [&]() -> void { bNos_ = true; });

	// shooting
	InputC->MapControlBinding(sf::Mouse::Left,
		InputHandler::EButtonState::EBS_Held,
		[this](int, int)
			{
				if (const auto shooter = ShootingComp.lock())
					shooter->TryShoot();
			});
	InputC->MapControlBinding(sf::Keyboard::Scancode::Space,
		InputHandler::EButtonState::EBS_Held,
		[this]()->void
			{
				if (const auto shooter = ShootingComp.lock())
					shooter->TryShoot();
			});

	// shooting strats
	InputC->MapControlBinding(sf::Keyboard::Scancode::Num1,
							  InputHandler::EButtonState::EBS_Pressed,
							  [this]() -> void
								  {
									  if (const auto shooter = ShootingComp.lock())
										  shooter->SwitchShootStrategy(1);
								  });
	InputC->MapControlBinding(sf::Keyboard::Scancode::Num2,
							  InputHandler::EButtonState::EBS_Pressed,
							  [this]() -> void
								  {
									  if (const auto shooter = ShootingComp.lock())
										  shooter->SwitchShootStrategy(2);
								  });
	InputC->MapControlBinding(sf::Keyboard::Scancode::Num3,
							  InputHandler::EButtonState::EBS_Pressed,
							  [this]() -> void
								  {
									  if (const auto shooter = ShootingComp.lock())
										  shooter->SwitchShootStrategy(3);
								  });
}

bool PlayerShip::IsCollisionAllowed() const
{
	return !GetIsInCollisionCooldown();
}

void PlayerShip::SetLives(int Lives)
{
	livesRemaining_ = Lives;
}
