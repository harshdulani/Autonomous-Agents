#include "EnemyShip.h"
#include "Physics/Collider.h"
//#include "Portal.h"
#include "Asteroid.h"
#include "Debug.h"
#include "Bullet.h"
#include "Maths.h"
#include <cassert>
#include <utility>
#include "ShootingComponent.h"
#include "ScreenShaker.h"
#include "GroupAttackingPolicy.h"
#include "System.h"
#include "ParticleSystem/ParticleSystemManager.h"
#include "ParticleSystem/ParticleSystem.h"
#include "Graph/FSM/FSMManager.h"
#include "Graph/FSM/FSM.h"
#include "AutoShoot_WaitAndShootState.h"
#include "AutoShoot_SuspendedState.h"
#include "Enemy_ChaseState.h"
#include "Enemy_RevolveState.h"
#include "PlayerShip.h"
#include "TimerManager.h"
#include "Graph/TransitionEdge.h"

EnemyShip::EnemyShip()
{
	groupAttack_ = System::GetInstance()->GetGame().GetGroupAttackingPolicy();

	lowHealthColor_ = {255, 69, 69, 55};
	maxHealthColor_ = {153, 255, 0, 55};
	
	auto weakDraw = CreateDrawable<MultiTriShape>();
	if (auto Draw = weakDraw.lock())
	{
		std::vector<sf::Vector2f> Verts{
			{10.f, 30.f},
			{0.f, 15.f},
			{15.f, 0.f},
			{30.f, 15.f},
			{20.f, 30.f},
			{15.f, 0.f},
		};
		Draw->SetTris(Verts, {255, 204, 69});
		Draw->setScale(1.f, 1.f);
		Draw->setPosition(-7.5f, -7.5f);
	}
	SetScale(1.f);
	
}

void EnemyShip::Initialise()
{
	System::GetInstance()->GetGame().Event_EnemyShipCreated.Invoke(std::static_pointer_cast<EnemyShip>(System::GetInstance()->GetObjectMgr()->GetWeakPtr(this).lock()));
	
	InitialisePerception(100.f);
	InitObstacleAvoidance(70.f, 20.f);
	InitThrusterParticles();
	InitialiseHealthIndicator(20.f);	
	InitialiseChaseFSM(200.f, 175.f);
	InitialiseAutoShootFSM();
}

void EnemyShip::Update(const float deltaTime)
{
	AddFlockingForces();
	AddForce(AvoidObstacles() * 1.f);
	AddForce(SeekTarget() * 0.65f);

	UpdateVelocity(deltaTime);
	
	//Position
	SetPosition(GetPosition() + velocity);

	//Rotation
	auto angle = Maths::RadsToDegs(atan2f(velocity.y, velocity.x)) + 90.f;
	//angle = Math::WrapModulo(angle, 360.f);

	SetRotation(angle);
	//SetRotation(Math::LerpClamped(GetRotation(), angle, 7.5f * deltaTime));
}

void EnemyShip::OnCollision(std::weak_ptr<GameEntity> other)
{
	// For flocking
	Bot::OnCollision(other);

	if (other.expired())
	{
		return;
	}
	auto that = other.lock();

	// For Collision Avoidance
	if (std::dynamic_pointer_cast<Asteroid>(that) ||
		//std::dynamic_pointer_cast<Portal>(that) ||
		std::dynamic_pointer_cast<PlayerShip>(that))
	{
		float distance = Maths::GetVectorMagnitude(GetPosition() - that->GetPosition()) -
			that->GetComponentOfType<Collider>().lock()->GetRadius();

		if (distance < obstacleAvoiderRadius_)
		{
			TryAddToList<GameEntity>(that, obstacles_);
		}
	}

	if (GetIsInCollisionCooldown())
	{
		return;
	}

	if (auto bullet = std::dynamic_pointer_cast<Bullet>(that))
	{
		float distance = Maths::GetVectorMagnitude(GetPosition() - that->GetPosition()) -
			that->GetComponentOfType<Collider>().lock()->GetRadius();
		if (distance < collisionRadius_)
		{
			if (!bullet->GetIsPlayerOwned())
			{
				return;
			}
			BulletHit();
			//destroy bullet
			bullet->Kill();
		}
	}
}

void EnemyShip::SetTarget(std::weak_ptr<GameEntity> target)
{
	target_ = std::move(target);
}

void EnemyShip::InitObstacleAvoidance(float avoidanceRadius, float collisionRadius)
{
	obstacleAvoiderRadius_ = avoidanceRadius;
	if (GetPerceptionRadius() < avoidanceRadius)
	{
		assert(GetPerceptionRadius() > 0.f && "Perception not initialized.");
		Debug::Print("WARNING: Avoidance radius set higher than perception radius.");
	}
	collisionRadius_ = collisionRadius;
}

void EnemyShip::InitThrusterParticles()
{
	//Particle system
	thruster = System::GetInstance()->GetParticleSystemManager()->CreateNewParticleSystem(this);
	std::shared_ptr<ParticleSystem> p = thruster.lock();

	p->SetLocalPosition({ 2.5f, 2.5f});
	p->SetLocalRotation(180.f);

	p->SetLooping(true)
		.SetDuration(0.5f)

		.SetEmissionShape(LogicalParticleShape::HLineSamplingFunction, 8)
		.SetEmissionMode(EmissionMode::Random)
		.SetEmitterRotationMode(TransformSpace::Local)
		.SetEmissionVelocityMode(EmissionVelocityMode::Normal)
		.SetEmitterScaling(6.f)
		.SetEmitterRate(32.f)
		.SetEmissionSpeed(100.f)

		.SetParticleShape(LogicalParticleShape::VLineSamplingFunction, 2)
		.SetParticleScale(5.f)
		.SetParticleLifeTime(0.15f, 0.45f)
		.SetColorOverLifeTime(sf::Color(255, 155, 0), sf::Color::Black);
}

void EnemyShip::InitialiseHealthIndicator(const float radius)
{
	healthIndicator_ = CreateDrawable<sf::CircleShape>(radius);

	auto health = healthIndicator_.lock();
	health->setPosition(-radius * 0.5f, -radius * 0.5f);
	health->setFillColor(sf::Color::Transparent);
	health->setOutlineThickness(5.f);
	
	UpdateHealthIndicator();
}

void EnemyShip::InitialiseChaseFSM(const float minDistance,
								   const float pathRadius)
{
	auto chaseFSM = System::GetInstance()->GetFSMManager()->CreateFSM(GetWeakSelf());

	auto strongFsm = chaseFSM.lock();
	assert(strongFsm != nullptr && "Invalid FSM");

	SetChaseFSMHandle(strongFsm->GetHandle());

	//fsm construction
	//parameters
	strongFsm->SetParameter("shouldRevolve", false);

	//states

	//chase state
	std::weak_ptr<Enemy_ChaseState> chase = strongFsm->CreateNode<Enemy_ChaseState>();
	auto strongChase = chase.lock();
	assert(strongChase != nullptr && "Invalid State Created");

	strongChase->SetGameEntity(GetWeakSelf());
	strongChase->InitialiseState(minDistance);
	strongFsm->AssignDefaultState(chase);

	//revolve state
	std::weak_ptr<Enemy_RevolveState> revolve = strongFsm->CreateNode<Enemy_RevolveState>();
	auto strongRevolve = revolve.lock();
	assert(strongRevolve != nullptr && "Invalid State Created");

	strongRevolve->SetGameEntity(GetWeakSelf());
	strongRevolve->InitialiseState(minDistance, pathRadius, 7.5f);

	//transitions
	std::weak_ptr<StateNode> chaseStateNode = std::static_pointer_cast<StateNode>(strongChase);
	std::weak_ptr<StateNode> revolveStateNode = std::static_pointer_cast<StateNode>(strongRevolve);

	//transition chase to revolve
	std::weak_ptr<TransitionEdge> currentEdge = strongFsm->CreateEdge(chaseStateNode, revolveStateNode);
	//transition from chase to revolve when shouldRevolve turns true
	currentEdge.lock()->AddTransitionCondition("shouldRevolve", TransitionEdgeComparator::Equals, true);

	//transition revolve to chase
	currentEdge = strongFsm->CreateEdge(revolveStateNode, chaseStateNode);
	//transition from chase to revolve when shouldRevolve turns false
	currentEdge.lock()->AddTransitionCondition("shouldRevolve", TransitionEdgeComparator::Equals, false);

	strongFsm->Initialise();
}

void EnemyShip::InitialiseAutoShootFSM()
{
	// Shooting Component
	shootingComponent_ = AddComponent<ShootingComponent>();
	if (auto shooter = shootingComponent_.lock())
	{
		shooter->SetLocalPosition({0.f, -15.f});
		shooter->SetIsPlayer(false);
	}

	// FSM
	auto autoShootFSM = System::GetInstance()->GetFSMManager()->CreateFSM(GetWeakSelf());

	auto strongFsm = autoShootFSM.lock();
	assert(strongFsm != nullptr && "Invalid FSM");

	fsmHandle_AutoShootFsm_ = strongFsm->GetHandle();

	//fsm construction
	//parameters
	strongFsm->SetParameter("isSuspended", true);

	//states
	//waitAndShoot state
	std::weak_ptr<AutoShoot_WaitAndShootState> waitAndShoot = strongFsm->CreateNode<AutoShoot_WaitAndShootState>();
	auto strongWaitShoot = waitAndShoot.lock();
	assert(strongWaitShoot != nullptr && "Invalid State Created");

	strongWaitShoot->SetGameEntity(GetWeakSelf());
	strongWaitShoot->SetStateDuration(1.f);
	strongWaitShoot->InitialiseState(GetTarget(), 200.f);

	//suspended state
	std::weak_ptr<AutoShoot_SuspendedState> suspended = strongFsm->CreateNode<AutoShoot_SuspendedState>();
	auto strongSuspended = suspended.lock();
	assert(strongSuspended != nullptr && "Invalid State Created");

	strongSuspended->SetGameEntity(GetWeakSelf());
	strongFsm->AssignDefaultState(suspended);

	// transitions
	std::weak_ptr<StateNode> waitShootStateNode = std::static_pointer_cast<StateNode>(strongWaitShoot);
	std::weak_ptr<StateNode> suspendedStateNode = std::static_pointer_cast<StateNode>(strongSuspended);

	//transition waitShoot to suspended
	std::weak_ptr<TransitionEdge> currentEdge = strongFsm->CreateEdge(waitShootStateNode, suspendedStateNode);
	//transition from waitShoot to suspended when isSuspended turns true
	currentEdge.lock()->AddTransitionCondition("isSuspended", TransitionEdgeComparator::Equals, true);

	//transition suspended to waitShoot
	currentEdge = strongFsm->CreateEdge(suspendedStateNode, waitShootStateNode);
	//transition from suspended to waitShoot when isSuspended turns false
	currentEdge.lock()->AddTransitionCondition("isSuspended", TransitionEdgeComparator::Equals, false);

	strongFsm->Initialise();
}

sf::Vector2f EnemyShip::SeekTarget() const
{
	// Velocity
	return SeekLocation(GetSeekLocation());
}

void EnemyShip::UpdateHealthIndicator()
{
	sf::Color currentHealthColor = Maths::LerpColor(lowHealthColor_,
												   maxHealthColor_,					  
												   Maths::RemapClamped(0.5f, 1.f, 0.f, 1.f, GetLivesLeft() / static_cast<float>(GetTotalLives())));

	if (auto health = GetHealthIndicator().lock())
	{
		health->setOutlineColor(currentHealthColor);
	}
}

sf::Vector2f EnemyShip::AvoidObstacles()
{
	sf::Vector2f steerForce;
	int count = 0;
	sf::Vector2f sum;

	for (int i = 0; i < obstacles_.size(); i++)
	{
		if (obstacles_[i].expired())
		{
			//remove expired ref
			RemoveFromList(i, obstacles_);
			continue;
		}
		if (auto other = obstacles_[i].lock())
		{
			auto direction = GetPosition() - other->GetPosition();
			float objectRadius = 0.f;
			auto weakCollider = other->GetComponentOfType<Collider>();
			bool bInvalidCollider = weakCollider.expired();
			if (auto Coll = weakCollider.lock())
				objectRadius = Coll->GetRadius();

			// distance between obstacle collider boundary and player separation radius boundary
			float distance = Maths::GetVectorMagnitude(direction) - objectRadius;
			if (other->IsPendingKill() ||
				!other->GetActive() ||
				bInvalidCollider ||
				distance > obstacleAvoiderRadius_)
			{
				// remove refs to dead or faraway friends
				RemoveFromList(i, obstacles_);
				continue;
			}

			// the farther away someone is from us, lesser the force to be applied
			float factor = 1.f - (Maths::InverseLerpUnclamped(0.f, obstacleAvoiderRadius_, distance));

			sum += Maths::Normalize(direction) * factor;
			count++;
		}
	}
	if (count > 0)
	{
		sum /= static_cast<float>(count);
		sum = Maths::Normalize(sum) * maxSpeed;
		steerForce = sum - velocity;
		steerForce = Maths::LimitVector(steerForce, maxSteerForce);
	}
	return steerForce;
}

void EnemyShip::BulletHit()
{
	if (LoseALife())
	{
		// if ship survives collision/ losing a life
		bCollisionCooldown_ = true;
		GetTimerManager()->ResetTimer(collisionTimerHandle_);
		SetAllCollidersStatus(false);
		UpdateHealthIndicator();
		SpawnExplosion(false);
		return;
	}

	// if ship doesn't have any lives remaining
	SpawnExplosion(true);
	System::GetInstance()->GetGame().Event_EnemyShipDestroyed.Invoke(
		std::static_pointer_cast<EnemyShip>(System::GetInstance()->GetObjectMgr()->GetWeakPtr(this).lock()));
	SetActive(false);
	DestroyAllComponentsOfType<Collider>();
	if (auto t = thruster.lock())
	{
		t->Stop();
	}
}

std::weak_ptr<FSM> EnemyShip::GetAutoShootFSM() const
{
	return System::GetInstance()->GetFSMManager()->GetFSM(fsmHandle_AutoShootFsm_);
}

std::weak_ptr<FSM> EnemyShip::GetChaseFSM() const
{
	return System::GetInstance()->GetFSMManager()->GetFSM(fsmHandle_ChaseFsm_);
}

void EnemyShip::AttackComplete()
{
	if (groupAttack_.expired())
	{
		Debug::Print("Group attacking reference not found.");
		return;
	}
	groupAttack_.lock()->AttackComplete();
}

bool EnemyShip::IsAttacking() const
{
	if (auto shooter = GetAutoShootFSM().lock())
	{
		return !shooter->GetBool("isSuspended");
	}
	return false;
}

void EnemyShip::SpawnExplosion(bool bDeath)
{
	float size = bDeath ? 5.f : 3.f;
	System::GetInstance()->GetScreenShaker()->CreateImpulse(bDeath ? 0.5f : 0.35f, size, 25.0f);
	auto particleSys = System::GetInstance()->GetParticleSystemManager()->CreateNewParticleSystem(this);
	std::shared_ptr<ParticleSystem> p = particleSys.lock();

	p->SetWorldPosition(GetPosition());

	std::function<void()> onStart = []() -> void
	{
		System::GetInstance()->GetGame().ExplosionCount++;
	};

	std::function<void()> onEnd = [this, bDeath]() -> void
	{
		Game& game = System::GetInstance()->GetGame();
		game.ExplosionCount--;
		game.QueueNextLevel();
		if (bDeath)
		{
			Kill();
		}
	};
	
	p->SetOnStart(onStart)
	 .SetOnComplete(onEnd);
	
	p->SetDuration(0.125f)
	 .SetParticleLifeTime(0.25f * size, 0.5f * size)

	 .SetEmissionShape(LogicalParticleShape::CircleSamplingFunction, 32)
	 .SetEmissionMode(EmissionMode::Random)
	 .SetEmitterScaling(3.f * size)
	 .SetEmitterRate(0.f)
	 .AddEmitterBurst(0.f, 16)
	 .SetEmissionSpeed(50.f)

	 .SetParticleShape(LogicalParticleShape::CircleSamplingFunction, 32)
	 .SetScaleOverLifetime(1.5f * size, 0.f)
	 .SetColorOverLifeTime(sf::Color(255, 255, 0), sf::Color::Black);
}

void EnemyShip::SetAttacking(bool val)
{
	if (auto shooter = GetAutoShootFSM().lock())
	{
		shooter->SetParameter("isSuspended", !val);
	}
}
