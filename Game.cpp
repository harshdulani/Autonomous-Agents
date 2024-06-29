#include "Game.h"
#include "Asteroid.h"
#include "ImplicitGrid.h"
#include "Math.h"
#include "ObjectManager.h"
#include "PlayerShip.h"
#include "System.h"

//init static colors
sf::Color Game::Red{255, 69, 69};
sf::Color Game::Green{0, 204, 153};
sf::Color Game::Blue{69, 204, 255};
sf::Color Game::Yellow{255, 204, 69};

void Game::InitLevel(int levelNum)
{
	CreateCollisionSystem();
	totalAsteroids_ = 0;
	hasSpawnedBots_ = false;
	levelNum_ = levelNum;

	SpawnPlayer();
	SpawnAsteroids(levelNum);

	if (!uiFont_.loadFromFile("RecursiveSansLnrSt-Med.ttf"))
	{
		// error...
	}

	livesText_.setString("Lives: " + std::to_string(player_.lock()->GetLivesLeft()));
	livesText_.setPosition(25.f, 25.f);

	scoreText_.setString("Score: 0");
	scoreText_.setPosition(System::GetInstance()->GetWindowWidth() - scoreText_.getLocalBounds().getSize().x - 25.f, 25.f);
}

void Game::CreateCollisionSystem()
{
	collisionGrid_ = System::GetInstance()->GetObjectMgr()->CreateGameEntity<ImplicitGrid>({});
	if (auto Grid = collisionGrid_.lock())
	{
		Grid->InitGrid(
			static_cast<int>(System::GetInstance()->GetWindowWidth()),
			static_cast<int>(System::GetInstance()->GetWindowHeight()));
		Grid->SetRenderPriority(-101);

		Grid->SetDebugGridVisibility(false);
		Grid->SetDebugTextVisibility(false);
	}
}

void Game::SpawnBots(int SpawnCount)
{
	return;
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
	if (auto player = player_.lock())
	{
		oldLives = player->GetLivesLeft();
	}
	DeletePlayer();
	player_ = System::GetInstance()->GetObjectMgr()->CreateGameEntity<PlayerShip>(
		{System::GetInstance()->GetWindowWidth() / 2, System::GetInstance()->GetWindowHeight() / 2});

	if (std::shared_ptr<PlayerShip> player = player_.lock())
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
	if (auto player = player_.lock())
	{
		player->Kill();
	}
	player_.reset();
}

void Game::SpawnAsteroids(int numAsteroids)
{
	for (int i = 0; i < numAsteroids; i++)
	{
		SpawnAsteroidAt(CreateRandomPosition(), 3);
	}
}

sf::Vector2f Game::CreateRandomPosition()
{
	return {Math::GetRandFloat(0, System::GetInstance()->GetWindowWidth()),
			Math::GetRandFloat(0, System::GetInstance()->GetWindowHeight())};
}

void Game::SpawnAsteroidAt(sf::Vector2f position, int size)
{
	const float MAX_ASTEROID_SPEED = 1.0f;

	float angularVelocity = Math::GetRandFloat(0.f, MAX_ASTEROID_SPEED);

	std::weak_ptr<Asteroid> asteroid = System::GetInstance()->GetObjectMgr()
															->CreateGameEntity<Asteroid>(position);

	if (std::shared_ptr<Asteroid> roid = asteroid.lock())
	{
		//@todo: randomise (between) colors
		roid->InitAsteroid(size * 5.f,
						   sf::Color::Cyan,
						   angularVelocity, size);
	}
	asteroidCount_++;
	totalAsteroids_++;
}

int Game::GetAsteroidsAliveCount() const
{
	return totalAsteroids_ - asteroidsDestroyed;
}

void Game::UpdateScore(const int oldSize)
{
	score += 5 * oldSize;
	scoreText_.setString("Score: " + std::to_string(score));
}

void Game::ResetScore()
{
	score = 0;
}

void Game::SetAsteroidsDestroyed(int val)
{
	asteroidsDestroyed = val;
	if (asteroidsDestroyed < 2 || hasSpawnedBots_)
	{
		return;
	}
	hasSpawnedBots_ = true;
	int spawnCount = std::min(4, levelNum_ - 1);
	if (spawnCount > 0)
	{
		SpawnBots(spawnCount);
	}
}

bool Game::IsLevelComplete() const
{
	return (GetAsteroidsAliveCount() == 0 && explosionCount == 0);
}

bool Game::IsGameOver() const
{
	if (std::shared_ptr<PlayerShip> player = player_.lock())
	{
		return (player->GetLivesLeft() <= 0 && explosionCount == 0);
	}
	return true;
}

void Game::UpdateLivesRemaining(const int lives)
{
	livesText_.setString(std::string("Lives: ") + std::to_string(lives));
}
