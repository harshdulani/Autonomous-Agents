#include <iostream>
#include "Game.h"
#include "Asteroid.h"
#include "Component.h"
#include "Math.h"
#include "ObjectManager.h"
#include "System.h"

//init static colors
sf::Color Game::Red{255, 69, 69};
sf::Color Game::Green{0, 204, 153};
sf::Color Game::Blue{69, 204, 255};
sf::Color Game::Yellow{255, 204, 69};


void Game::InitEnemies()
{
	std::vector<sf::Vector2f> Verts{
		{10.f, 30.f},
		{0.f, 15.f},
		{15.f, 0.f},
		{30.f, 15.f},
		{20.f, 30.f},
		{15.f, 0.f},
	};
}

void Game::InitPlayer()
{
}

void Game::InitAsteroids()
{
	int NumAsteroids = Math::GetRandInt(3, 10);
	for (int i = 0; i < NumAsteroids; ++i)
	{
		auto WeakRoid = System::GetInstance()->GetObjectMgr()->CreateGameEntity<Asteroid>(
			{Math::GetRandFloat(0, System::GetInstance()->GetWindowWidth()),
			 Math::GetRandFloat(0, System::GetInstance()->GetWindowHeight())
			});

		if (auto Roid = WeakRoid.lock())
		{
			//@todo: randomise (between) colors
			Roid->InitAsteroid(Math::GetRandFloat(15.f, 30.f), sf::Color::Cyan);
		}
	}
}

void Game::UpdateWindowData(sf::RenderWindow* Window)
{
	// poll exit game events
	PollEvents(Window);
	// update mouse vals
	UpdateInput(Window);
}

void Game::PollEvents(sf::RenderWindow* Window)
{
	while (Window->pollEvent(Event))
	{
		switch (Event.type)
		{
			case sf::Event::Closed:
				Window->close();
				break;
			case sf::Event::KeyPressed:
				if (Event.type == sf::Event::Closed || Event.KeyPressed == sf::Keyboard::Escape)
					Window->close();
				break;
			default: ;
		}
	}
}

void Game::UpdateInput(sf::RenderWindow* Window)
{
	MousePos = Window->mapPixelToCoords(sf::Mouse::getPosition(*Window));
	//std::cout << MousePos.x << ", " << MousePos.y << '\n';
}

sf::Vector2f Game::GetMousePosition() const { return MousePos; }

void Game::StartGame()
{
	InitAsteroids();
}
