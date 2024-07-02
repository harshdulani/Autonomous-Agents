#pragma once
#include <memory>
#include <SFML/Window/VideoMode.hpp>
#include "Event.h"
#include "Game.h"

class ParticleSystemManager;
class FSMManager;

namespace sf
{
	class RenderWindow;
}

class GameEntity;
class ObjectManager;
class TimerManager;

class System
{
public:
	System();
	
	Event<> Event_LevelStart;
	Event<> Event_LevelEnd;
	
	Event<std::weak_ptr<GameEntity>> Event_EntityCreated;
	Event<std::weak_ptr<GameEntity>> Event_EntityDestroyed;
	
	static System* GetInstance();

	void Initialize();
	void PollWindowEvents();	
	void Update(float DeltaTime);
	void Render();
	void Terminate();

	Game& GetGame();
	ObjectManager* GetObjectMgr() const;
	TimerManager* GetTimerManager() const;
	FSMManager* GetFSMManager() const;
	ParticleSystemManager* GetParticleSystemManager() const;
	/*
	ScreenShaker* GetScreenShaker() const;
	*/
	bool IsWindowOpen() const;
	bool IsWindowClosePending() const;
	float GetWindowWidth() const;
	float GetWindowHeight() const;
	void CloseWindow();

private:
	static System* Singleton;

	sf::RenderWindow window_;
	Game gameInst_;
	std::shared_ptr<ObjectManager> objectMgr_;
	std::shared_ptr<TimerManager> timerMgr_;
	std::shared_ptr<FSMManager> fsmMgr_;
	std::shared_ptr<ParticleSystemManager> particleSysMgr_;

	bool bPendingWindowClose = false;

	// this is here bc we will be creating a fsm for the game states and they will change levels
	int currentLevel = 1;
	
	sf::VideoMode VideoMode;
	float WindowWidth = 800.f;
	float WindowHeight = 600.f;
};
