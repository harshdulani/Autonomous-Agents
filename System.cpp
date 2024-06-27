#include "System.h"
#include "Game.h"
#include "ObjectManager.h"

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
	Window.create(VideoMode, "Asteroids SFML", sf::Style::Titlebar | sf::Style::Close);
	Window.setFramerateLimit(60);

	ObjectMgr = std::make_shared<ObjectManager>();
	ObjectMgr->SetWindowVals(GetWindowWidth(), GetWindowHeight());
	TimerMgr = std::make_shared<TimerManager>();
	/*
		fsmManager_ = make_shared<FSMManager>();
		uiManager_ = make_shared<UIManager>();
		particleSysMgr_ = make_shared<ParticleSystemManager>();
	*/
	GameInst.StartGame();
}

void System::PollWindowEvents()
{
	sf::Event Event;
	while (Window.pollEvent(Event))
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
	TimerMgr->UpdateTimers(DeltaTime);

	ObjectMgr->UpdateAllObjects(DeltaTime);
}

void System::Render()
{
	Window.clear();

	ObjectMgr->RenderAllObjects(Window);

	Window.display();
}

void System::Terminate()
{
	ObjectMgr.reset();
	TimerMgr.reset();
	/*
	fsmManager_.reset();
	uiManager_.reset();
	particleSysMgr_.reset();
	*/
}

void System::CloseWindow()
{
	Window.close();
}

ObjectManager* System::GetObjectMgr() const { return ObjectMgr.get(); }
TimerManager* System::GetTimerManager() const { return TimerMgr.get(); }
Game& System::GetGame() { return GameInst; }

bool System::IsWindowOpen() const { return Window.isOpen(); }
bool System::IsWindowClosePending() const { return bPendingWindowClose; }

float System::GetWindowWidth() const { return WindowWidth; }
float System::GetWindowHeight() const { return WindowHeight; }