#include "System.h"
#include "FSMManager.h"
#include "Game.h"
#include "ObjectManager.h"
#include "TimerManager.h"
#include "ParticleSystem/ParticleSystemManager.h"

// initializing static variables
System* System::Singleton = nullptr;

System::System()
{
	Initialize();
}

System* System::GetInstance() { return Singleton; }

void System::Initialize()
{
	if (!Singleton)
	{
		Singleton = this;
	}

	VideoMode.height = static_cast<unsigned int>(WindowHeight);
	VideoMode.width = static_cast<unsigned int>(WindowWidth);
	window_.create(VideoMode, "Asteroids SFML", sf::Style::Titlebar | sf::Style::Close);
	window_.setFramerateLimit(60);

	objectMgr_ = std::make_shared<ObjectManager>();
	objectMgr_->SetWindowVals(GetWindowWidth(), GetWindowHeight());
	timerMgr_ = std::make_shared<TimerManager>();
	fsmMgr_ = std::make_shared<FSMManager>();
	particleSysMgr_ = std::make_shared<ParticleSystemManager>();

	gameInst_.InitLevel(currentLevel);
	Event_LevelStart.Invoke();

	if (currentLevel == 1)
		gameInst_.ResetScore();
}

void System::PollWindowEvents()
{
	sf::Event Event;
	while (window_.pollEvent(Event))
	{
		switch (Event.type)
		{
			case sf::Event::Closed:
				bPendingWindowClose = true;
				break;
			case sf::Event::KeyPressed:
				if (Event.key.code == sf::Keyboard::Escape)
					bPendingWindowClose = true;
				break;
			default: ;
		}
	}
}

void System::Update(float DeltaTime)
{
	//@todo: add a game state machine here - boot, title, game, high score, end
	timerMgr_->UpdateTimers(DeltaTime);

	objectMgr_->UpdateAllObjects(DeltaTime);
	fsmMgr_->UpdateFSMs(DeltaTime);
	particleSysMgr_->UpdateAllParticleSystems(DeltaTime);
}

void System::Render()
{
	window_.clear();

	objectMgr_->RenderAllObjects(window_);

	window_.display();
}

void System::Terminate()
{
	objectMgr_.reset();
	timerMgr_.reset();
	fsmMgr_.reset();
	/*
	particleSysMgr_.reset();
	*/
}

void System::CloseWindow()
{
	window_.close();
}

ObjectManager* System::GetObjectMgr() const { return objectMgr_.get(); }
TimerManager* System::GetTimerManager() const { return timerMgr_.get(); }
FSMManager* System::GetFSMManager() const { return fsmMgr_.get(); }

ParticleSystemManager* System::GetParticleSystemManager() const { return particleSysMgr_.get(); }

Game& System::GetGame() { return gameInst_; }

bool System::IsWindowOpen() const { return window_.isOpen(); }
bool System::IsWindowClosePending() const { return bPendingWindowClose; }

float System::GetWindowWidth() const { return WindowWidth; }
float System::GetWindowHeight() const { return WindowHeight; }