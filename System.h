#pragma once
#include <memory>
#include <SFML/Window/VideoMode.hpp>

namespace sf
{
	class RenderWindow;
}

class ObjectManager;
class Game;

class System
{
public:
	System();
	
	static System* GetInstance();

	void Initialize();
	void Update(float DeltaTime);
	void Render() const;
	
	Game* GetGame() const;
	ObjectManager* GetObjectMgr() const;

	bool IsWindowOpen() const;
	float GetWindowWidth() const;
	float GetWindowHeight() const;

private:
	static System* Singleton;

	std::shared_ptr<sf::RenderWindow> Window;
	std::shared_ptr<Game> GameInst;
	std::shared_ptr<ObjectManager> ObjectMgrInst;

	sf::VideoMode VideoMode;
	float WindowWidth = 800.f;
	float WindowHeight = 600.f;
};
