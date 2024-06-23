#include "System.h"
#include "Game.h"
#include "ObjectManager.h"

System::System()
{
	Initialize();
}

// initializing static variables
System* System::Singleton = nullptr;

System* System::GetInstance() { return Singleton; }

void System::Initialize()
{
	if(!Singleton)
	{
		Singleton = this;
	}
	
	VideoMode.height = static_cast<unsigned int>(WindowHeight);
	VideoMode.width = static_cast<unsigned int>(WindowWidth);
	Window = std::make_shared<sf::RenderWindow>(VideoMode, "Asteroids SFML", sf::Style::Titlebar | sf::Style::Close);
	//Window->setFramerateLimit(165);

	ObjectMgrInst = std::make_shared<ObjectManager>();
	ObjectMgrInst->SetWindowVals(GetWindowWidth(), GetWindowHeight());

	GameInst.StartGame();
}

void System::Update(float DeltaTime)
{
	GameInst.UpdateWindowData(Window.get());
	
	ObjectMgrInst->UpdateAllObjects(DeltaTime);	
}

void System::Render() const
{
	Window->clear();

	ObjectMgrInst->RenderAllObjects(Window.get());

	Window->display();
}

ObjectManager* System::GetObjectMgr() const { return ObjectMgrInst.get(); }

bool System::IsWindowOpen() const { return Window->isOpen(); }
float System::GetWindowWidth() const { return WindowWidth; }
float System::GetWindowHeight() const { return WindowHeight; }
