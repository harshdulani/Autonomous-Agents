#pragma once
#include <memory>

class Game;

class System
{
public:
	System();
	
	static System* GetInstance();

	void Initialize();
	
	Game* GetGame() const;

private:
	static System* Singleton;

	std::shared_ptr<Game> GameInst;
};
