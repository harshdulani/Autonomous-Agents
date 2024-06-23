#pragma once
#include <memory>
#include <SFML/Window/VideoMode.hpp>
#include "Event.h"
#include "Game.h"

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
	void Update(float DeltaTime);
	void Render() const;

	ObjectManager* GetObjectMgr() const;
	/*
	TimerManager* GetTimerManager() const;
	ScreenShaker* GetScreenShaker() const;
	FSMManager* GetFSMManager() const;
	UIManager* GetUIManager() const;
	ParticleSystemManager* GetParticleSystemManager() const;
	std::weak_ptr<GroupAttackingPolicy> GetGroupAttackingPolicy() const;
	*/
	bool IsWindowOpen() const;
	float GetWindowWidth() const;
	float GetWindowHeight() const;

private:
	static System* Singleton;

	Game GameInst;
	std::shared_ptr<sf::RenderWindow> Window;
	std::shared_ptr<ObjectManager> ObjectMgrInst;
	/*
	std::shared_ptr<TimerManager> timerManager_;
	std::shared_ptr<FSMManager> fsmManager_;
	std::shared_ptr<UIManager> uiManager_;
	std::shared_ptr<ParticleSystemManager> particleSysMgr_;
	*/
	
	sf::VideoMode VideoMode;
	float WindowWidth = 800.f;
	float WindowHeight = 600.f;
};
