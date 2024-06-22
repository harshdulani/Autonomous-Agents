#include "Game.h"
#include "System.h"

int main()
{
	const auto SystemInst = std::make_unique<System>();
	if (const auto Game = SystemInst->GetGame())
	{
		while (Game->IsWindowOpen())
		{
			// poll events
			Game->PollEvents();
			// Update
			Game->Update();
			// Render
			Game->Render();
		}
	}

	return 0;
}
