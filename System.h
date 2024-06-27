#pragma once
#include <memory>
#include <SFML/Window/VideoMode.hpp>
#include "Event.h"
#include "Game.h"
#include "TimerManager.h"

namespace sf
{
	class RenderWindow;
}

class GameEntity;
class ObjectManager;

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
	/*
	ScreenShaker* GetScreenShaker() const;
	FSMManager* GetFSMManager() const;
	UIManager* GetUIManager() const;
	ParticleSystemManager* GetParticleSystemManager() const;
	std::weak_ptr<GroupAttackingPolicy> GetGroupAttackingPolicy() const;
	*/
	bool IsWindowOpen() const;
	bool IsWindowClosePending() const;
	float GetWindowWidth() const;
	float GetWindowHeight() const;
	void CloseWindow();

private:
	static System* Singleton;

	Game GameInst;
	sf::RenderWindow Window;
	std::shared_ptr<ObjectManager> ObjectMgr;
	std::shared_ptr<TimerManager> TimerMgr;
	/*
	std::shared_ptr<FSMManager> fsmManager_;
	std::shared_ptr<UIManager> uiManager_;
	std::shared_ptr<ParticleSystemManager> particleSysMgr_;
	*/

	bool bPendingWindowClose = false;
	
	sf::VideoMode VideoMode;
	float WindowWidth = 800.f;
	float WindowHeight = 600.f;
};
