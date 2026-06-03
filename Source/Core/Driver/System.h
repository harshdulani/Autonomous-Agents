#pragma once
#include <memory>
#include <SFML/Window/VideoMode.hpp>
#include "Event.h"
#include "Game.h"
#include "Input/InputHandler.h"

class ScreenShaker;
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
	~System();
	
	static System* GetInstance();

	void Initialize();
	void Update(float DeltaTime);
	void Render();
	void Terminate();

	Game& GetGame();
	std::weak_ptr<ImplicitGrid> GetCollisionSystem();
	InputHandler& GetInputHandler();
	ObjectManager* GetObjectMgr() const;
	TimerManager* GetTimerManager() const;
	FSMManager* GetFSMManager() const;
	ParticleSystemManager* GetParticleSystemManager() const;
	ScreenShaker* GetScreenShaker() const;
	
	bool IsWindowOpen() const;
	bool IsWindowClosePending() const;
	float GetWindowWidth() const;
	float GetWindowHeight() const;

	void CloseWindowDeferred();
	void CloseWindow();

	static std::string GetAssetPath(const std::string& fileName);

private:
	static System* singleton_;

	sf::RenderWindow window_;
	std::shared_ptr<sf::View> camera_;
	Game gameInst_;
	InputHandler inputHandler_;
	std::shared_ptr<ObjectManager> objectMgr_;
	std::shared_ptr<TimerManager> timerMgr_;
	std::shared_ptr<FSMManager> fsmMgr_;
	std::shared_ptr<ParticleSystemManager> particleSysMgr_;
	std::shared_ptr<ScreenShaker> screenShaker_;

	bool bPendingWindowClose_ = false;
		
	sf::VideoMode videoMode_;
	float windowWidth_ = 800.f;
	float windowHeight_ = 600.f;
};
