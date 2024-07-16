#include "pch.h"
#include "Portal.h"
#include "../FSM.h"
#include "../FSMManager.h"
#include "../PlayerShip.h"
#include "Portal_ReadyState.h"
#include "Portal_WaitState.h"
#include "../Maths.h"
#include "../System.h"
#include "../ParticleSystem/ParticleSystemManager.h"
#include "../ParticleSystem/ParticleSystem.h"

Portal::Portal()
{
	fsmMgr_ = System::GetInstance()->GetFSMManager();
	slice_ = Math::TWO_PI / static_cast<float>(resolution_);
	readyColor_ = sf::Color(37, 189, 255);
	waitColor_ = sf::Color(255, 0, 41);
}

void Portal::AddVisuals()
{
	auto particleSys = System::GetInstance()->GetParticleSystemManager()->CreateNewParticleSystem(this);
	std::shared_ptr<ParticleSystem> p = particleSys.lock();

	p->SetLooping(true)
		.SetDuration(0.5f)

		.SetEmissionShape(LogicalParticleShape::CircleSamplingFunction, 32)
		.SetEmissionMode(EmissionMode::LinearLooping)
		.SetEmissionVelocityMode(EmissionVelocityMode::Outward)
		.SetEmitterScaling(portalRadius_)
		.SetEmitterRate(32.f)
		.SetEmissionSpeed(10.f)

		.SetParticleShape(LogicalParticleShape::HLineSamplingFunction, 2)
		.SetParticleScale(15.f)
		.SetParticleLifeTime(0.35f)
		.SetColorOverLifeTime(GetReadyColor(), sf::Color::Black);
	/*
	auto weakCircle = CreateDrawable<sf::CircleShape>(portalRadius_, 32);
	if (auto circle = weakCircle.lock())
	{
		circle->setFillColor(sf::Color::Transparent);
		circle->setOutlineColor(GetRenderColor());
		circle->setOutlineThickness(2.5f);
		circle->setPosition(-portalRadius_ * 0.5f, -portalRadius_ * 0.5f);
	}
	*/
}

void Portal::SetPortalRadius(const float radius)
{
	portalRadius_ = radius;
}

void Portal::SetLinkedPortal(const std::weak_ptr<Portal>& linkedPortal)
{
	linkedPortal_ = linkedPortal;
}

void Portal::InitialiseFSM()
{
	//create fsm
	auto strongFsm = System::GetInstance()->GetFSMManager()->CreateFSM(GetWeakSelf()).lock();
	assert(strongFsm != nullptr && "Invalid FSM");
	
	stateMachineHandle_ = strongFsm->GetHandle();

	//fsm construction
	//parameters
	strongFsm->SetParameter("isInCooldown", false);

	//states
	std::weak_ptr<Portal_ReadyState> ready;
	std::weak_ptr<Portal_WaitState> waiting;

	//ready state
	ready = strongFsm->CreateNode<Portal_ReadyState>();
	auto strongReady = ready.lock();
	assert(strongReady != nullptr && "Invalid State Created");

	strongReady->SetGameEntity(GetWeakSelf());
	strongFsm->AssignDefaultState(ready);

	//waiting state
	waiting = strongFsm->CreateNode<Portal_WaitState>();
	auto strongWaiting = waiting.lock();
	assert(strongWaiting != nullptr && "Invalid State Created");

	strongWaiting->SetGameEntity(GetWeakSelf());
	strongWaiting->SetStateDuration(4.f);

	std::weak_ptr<StateNode> readyStateNode = std::static_pointer_cast<StateNode>(strongReady);
	std::weak_ptr<StateNode> waitStateNode = std::static_pointer_cast<StateNode>(strongWaiting);

	//transition ready to waiting
	std::weak_ptr<TransitionEdge> currentEdge = strongFsm->CreateEdge(readyStateNode, waitStateNode);

	//transition from ready to waiting when isInCooldown turns true
	currentEdge.lock()->AddTransitionCondition("isInCooldown", TransitionEdgeComparator::Equals, true);

	//transition waiting to ready
	currentEdge = strongFsm->CreateEdge(waitStateNode, readyStateNode);

	//transition from ready to waiting when isInCooldown turns false
	currentEdge.lock()->AddTransitionCondition("isInCooldown", TransitionEdgeComparator::Equals, false);

	strongFsm->Initialise();
}

void Portal::OnCollision(std::weak_ptr<GameEntity> other)
{
	if (isInCooldown_)
	{
		return;
	}
	
	if (auto ship = std::dynamic_pointer_cast<PlayerShip>(other.lock()))
	{
		StartFSMCooldown();
		if (auto sharedLink = linkedPortal_.lock())
			sharedLink->StartFSMCooldown();

		SendToLinkedPortal(ship);
	}
}

void Portal::StartFSMCooldown()
{
	if (auto fsm = fsmMgr_->GetFSM(stateMachineHandle_).lock())
	{
		fsm->SetParameter("isInCooldown", true);
	}
	auto pSys = GetComponentOfType<ParticleSystem>();
	if (auto p = pSys.lock())
	{
		p->SetColorOverLifeTime(GetWaitColor(), sf::Color::Black);
	}

	SetIsInCooldown(true);
}

void Portal::EndFSMCooldown()
{
	if (auto fsm = fsmMgr_->GetFSM(stateMachineHandle_).lock())
	{
		fsm->SetParameter("isInCooldown", false);
	}
	auto pSys = GetComponentOfType<ParticleSystem>();
	if (auto p = pSys.lock())
	{
		p->SetColorOverLifeTime(GetReadyColor(), sf::Color::Black);
	}
	SetIsInCooldown(false);
}

sf::Color Portal::GetReadyColor() const
{
	return readyColor_;
}

sf::Color Portal::GetWaitColor() const
{
	return waitColor_;
}

void Portal::SetIsInCooldown(bool status)
{
	isInCooldown_ = status;
}

void Portal::SendToLinkedPortal(const std::weak_ptr<GameEntity>& other)
{
	auto sharedOther = other.lock();
	auto sharedLink = linkedPortal_.lock();
	if(sharedLink && sharedOther)
	{
		sharedOther->SetPosition(sharedLink->GetPosition());
	}
}

sf::Color Portal::GetRenderColor() const { return renderColor_; }

void Portal::SetRenderColor(const sf::Color& color)
{
	renderColor_ = color;
}
