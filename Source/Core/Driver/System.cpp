#include "System.h"
#include "Graph/FSM/FSMManager.h"
#include "Game.h"
#include "ObjectManager.h"
#include "ScreenShaker.h"
#include "TimerManager.h"
#include "ParticleSystem/ParticleSystemManager.h"

// initializing static variables
System* System::singleton_ = nullptr;

System::System()
{
	Initialize();
}

System::~System()
= default;

System* System::GetInstance() { return singleton_; }

void System::Initialize()
{
	if (!singleton_)
	{
		singleton_ = this;
	}

	videoMode_.height = static_cast<unsigned int>(windowHeight_);
	videoMode_.width = static_cast<unsigned int>(windowWidth_);
	window_.create(videoMode_, "Asteroids SFML");
	//window_.setFramerateLimit(60);
	// create a view with its center and size
	camera_ = std::make_shared<sf::View>(sf::Vector2f(windowWidth_ * 0.5f, windowHeight_ * 0.5f),
										 sf::Vector2f(windowWidth_, windowHeight_));

	objectMgr_ = std::make_shared<ObjectManager>();
	objectMgr_->SetWindowVals(GetWindowWidth(), GetWindowHeight());
	timerMgr_ = std::make_shared<TimerManager>();
	fsmMgr_ = std::make_shared<FSMManager>();
	particleSysMgr_ = std::make_shared<ParticleSystemManager>();

	gameInst_.InitLevel(currentLevel_);
	screenShaker_ = std::make_shared<ScreenShaker>();
	screenShaker_->SetViewCam(camera_);
	Event_LevelStart.Invoke();

	if (currentLevel_ == 1)
		gameInst_.ResetScore();
}

void System::Update(float DeltaTime)
{
	inputHandler_.PollEvents(window_);
	
	//@todo: add a game state machine here - boot, title, game, high score, end
	timerMgr_->UpdateTimers(DeltaTime);

	objectMgr_->UpdateAllObjects(DeltaTime);
	fsmMgr_->UpdateFSMs(DeltaTime);
	particleSysMgr_->UpdateAllParticleSystems(DeltaTime);
	screenShaker_->UpdateScreenShakes(DeltaTime);
}

void System::Render()
{
	window_.clear();

	window_.setView(*camera_);
	objectMgr_->RenderAllObjects(window_);

	window_.display();
}

void System::Terminate()
{
	objectMgr_.reset();
	screenShaker_.reset();
	timerMgr_.reset();
	fsmMgr_.reset();
	particleSysMgr_.reset();
}

void System::CloseWindow()
{
	window_.close();
}

Game& System::GetGame() { return gameInst_; }
InputHandler& System::GetInputHandler() { return inputHandler_; }
ObjectManager* System::GetObjectMgr() const { return objectMgr_.get(); }
TimerManager* System::GetTimerManager() const { return timerMgr_.get(); }
FSMManager* System::GetFSMManager() const { return fsmMgr_.get(); }
ParticleSystemManager* System::GetParticleSystemManager() const { return particleSysMgr_.get(); }
ScreenShaker* System::GetScreenShaker() const { return screenShaker_.get(); }

bool System::IsWindowOpen() const { return window_.isOpen(); }

bool System::IsWindowClosePending() const { return bPendingWindowClose_; }

float System::GetWindowWidth() const { return windowWidth_; }
float System::GetWindowHeight() const { return windowHeight_; }

void System::CloseWindowDeferred()
{
	bPendingWindowClose_ = true;
}

std::string System::GetAssetPath(const std::string& fileName)
{
	return std::string(ASSETS_DIR) + fileName;
}