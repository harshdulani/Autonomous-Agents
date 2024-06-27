#include "Game.h"
#include "Asteroid.h"
#include "GridBackground.h"
#include "Math.h"
#include "ObjectManager.h"
#include "PlayerShip.h"
#include "System.h"

//init static colors
sf::Color Game::Red{255, 69, 69};
sf::Color Game::Green{0, 204, 153};
sf::Color Game::Blue{69, 204, 255};
sf::Color Game::Yellow{255, 204, 69};

void Game::StartGame()
{
	System::GetInstance()->GetObjectMgr()->CreateGameEntity<GridBackground>({});
	
	SpawnPlayer();
	SpawnAsteroids();
}

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

void Game::SpawnPlayer()
{
	int oldLives = -1;
	if (auto player = Player.lock())
	{
		oldLives = player->GetLivesLeft();
	}
	DeletePlayer();
	Player = System::GetInstance()->GetObjectMgr()->CreateGameEntity<PlayerShip>(
		{System::GetInstance()->GetWindowWidth() / 2, System::GetInstance()->GetWindowHeight() / 2});

	if (std::shared_ptr<PlayerShip> player = Player.lock())
	{
		player->CreateCollider(15.0f);
		player->InitialiseComponents();
		if (oldLives == -1)
		{
			player->SetTotalLives(3);
			player->ResetLives();
		}
		else
		{
			// survival mode type lives, dont reset to 0 on level change
			//player->SetLives(oldLives);
		}
	}
}

void Game::DeletePlayer()
{
	if (auto player = Player.lock())
	{
		player->Kill();
	}
	Player.reset();
}

void Game::SpawnAsteroids()
{
	int NumAsteroids = Math::GetRandInt(3, 6);
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

void Game::UpdateInput(sf::RenderWindow* Window)
{
	MousePos = Window->mapPixelToCoords(sf::Mouse::getPosition(*Window));
	//std::cout << MousePos.x << ", " << MousePos.y << '\n';
}

sf::Vector2f Game::GetMousePosition() const { return MousePos; }
