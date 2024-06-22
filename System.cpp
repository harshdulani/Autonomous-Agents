#include "System.h"
#include "Game.h"

System::System()
{
	Initialize();
}

// initializing static variables
System* System::Singleton = nullptr;

System* System::GetInstance() { return Singleton; }

void System::Initialize()
{
	GameInst = std::make_shared<Game>();
}

Game* System::GetGame() const { return GameInst.get(); }
