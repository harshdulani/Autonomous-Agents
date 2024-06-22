#include "Game.h"
#include "System.h"

int main()
{
	const auto SystemInst = std::make_unique<System>();
	const auto Game = SystemInst->GetGame();
	if (!Game)
		return 1;

	sf::Clock DeltaClock;
	while (Game->IsWindowOpen())
	{
		// Update
		Game->Update(DeltaClock.restart().asSeconds());
		// Render
		Game->Render();
	}

	return 0;
}
