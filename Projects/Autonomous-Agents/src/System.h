#pragma once
#include "pch.h"
#include "Event.h"
#include "Game.h"
#include "Input/InputHandler.h"

class System
{
public:
	System();
	~System();
	
	Event<> Event_LevelStart;
	Event<> Event_LevelEnd;
	
	Event<std::weak_ptr<class GameEntity>> Event_EntityCreated;
	Event<std::weak_ptr<class GameEntity>> Event_EntityDestroyed;
	
	static System* GetInstance();

	void Initialize();
	void Update(float DeltaTime);
	void Render();
	void Terminate();

	Game& GetGame();
	class InputHandler& GetInputHandler();
	class ObjectManager* GetObjectMgr() const;
	class TimerManager* GetTimerManager() const;
	class FSMManager* GetFSMManager() const;
	class ParticleSystemManager* GetParticleSystemManager() const;
	class ScreenShaker* GetScreenShaker() const;
	
	bool IsWindowOpen() const;
	bool IsWindowClosePending() const;
	float GetWindowWidth() const;
	float GetWindowHeight() const;

	void CloseWindowDeferred();
	void CloseWindow();

private:
	static System* singleton_;

	sf::RenderWindow window_;
	std::shared_ptr<sf::View> camera_;
	Game gameInst_;
	InputHandler inputHandler_;
	std::shared_ptr<class ObjectManager> objectMgr_;
	std::shared_ptr<class TimerManager> timerMgr_;
	std::shared_ptr<class FSMManager> fsmMgr_;
	std::shared_ptr<class ParticleSystemManager> particleSysMgr_;
	std::shared_ptr<class ScreenShaker> screenShaker_;

	bool bPendingWindowClose_ = false;

	// this is here bc we will be creating a fsm for the game states and they will change levels
	int currentLevel_ = 1;
	
	sf::VideoMode videoMode_;
	float windowWidth_ = 800.f;
	float windowHeight_ = 600.f;
};
