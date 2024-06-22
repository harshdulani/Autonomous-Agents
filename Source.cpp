#include "Game.h"
#include "System.h"

int main()
{
	const auto SystemInst = std::make_unique<System>();
	const auto Game = SystemInst->GetGame();
	if (!Game)
		return 1;

	sf::Clock DeltaClock;
	while (SystemInst->IsWindowOpen())
	{
		// Update
		SystemInst->Update(DeltaClock.restart().asSeconds());
		// Render
		SystemInst->Render();
	}

	return 0;
}
