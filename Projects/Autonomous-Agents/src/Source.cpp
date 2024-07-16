#include "pch.h"
#include "Game.h"
#include "System.h"

int main()
{
	System SystemInst;	
	sf::Clock DeltaClock;
	while (SystemInst.IsWindowOpen())
	{
		if(SystemInst.IsWindowClosePending())
		{
			SystemInst.Terminate();
			SystemInst.CloseWindow();
			break;
		}
		// Update
		SystemInst.Update(DeltaClock.restart().asSeconds());
		// Render
		SystemInst.Render();
	}

	return 0;
}
