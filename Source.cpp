#include "Game.h"
#include "System.h"

int main()
{
	System SystemInst;	
	sf::Clock DeltaClock;
	while (SystemInst.IsWindowOpen())
	{
		// Update
		SystemInst.Update(DeltaClock.restart().asSeconds());
		// Render
		SystemInst.Render();
	}

	return 0;
}
