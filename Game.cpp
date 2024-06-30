#include "Game.h"
#include "Asteroid.h"
#include "Background.h"
#include "Collider.h"
#include "EnemyShip.h"
#include "GameHUD.h"
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
	SetAsteroidsDestroyed(0);
	totalAsteroids_ = 0;
	hasSpawnedBots_ = false;
	levelNum_ = levelNum;
	
	CreateCollisionSystem();
	groupAttackingPolicy_ = System::GetInstance()->GetObjectMgr()->CreateObject<GroupAttackingPolicy>();

	SpawnPlayer();
	SpawnAsteroids(levelNum);
	SpawnBots(3);

	SpawnBackgrounds();
	
	SpawnHUD();
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
	while (SpawnCount--)
	{
		float Width = System::GetInstance()->GetWindowWidth();
		float Height = System::GetInstance()->GetWindowHeight();

		float x = 0.f;
		float y = 0.f;

		float random = Math::GetRandFloat(0.f, 1.f);
		if (random > 0.75f)
		{
			//left wall
			x = Math::GetRandFloat(0.f, 25.f);
			y = Math::GetRandFloat(Height * 0.1f, Height * 0.9f);
		}
		else if (random > 0.5f)
		{
			//top wall
			x = Math::GetRandFloat(Width * 0.1f, Width * 0.9f);
			y = Height - Math::GetRandFloat(0.f, 25.f);
		}
		else if (random > 0.25f)
		{
			//right wall
			x = Width - Math::GetRandFloat(0.f,25.f);
			y = Math::GetRandFloat(Height * 0.1f, Height * 0.9f);
		}
		else //if (random > 0.f)
		{
			//bottom wall
			x = Math::GetRandFloat(Width * 0.1f, Width * 0.9f);
			y = -Height + Math::GetRandFloat(0.f,25.f);
		}

		std::weak_ptr<EnemyShip> bot;
		bot = System::GetInstance()->GetObjectMgr()->CreateGameEntity<EnemyShip>({x, y});
/*
		if (Math::GetRandFloat(0.f,1.f) > 0.5f)
		{
			bot = objectManager_->CreateGameEntity<KamikazeShip>(XMVectorSet(x, y, 0.0f, 0.0f));
		}
		else
		{
			bot = objectManager_->CreateGameEntity<EnemyShip>(XMVectorSet(x, y, 0.0f, 0.0f));
		}
*/
		if (auto autobot = bot.lock())
		{
			autobot->SetTotalLives(2);
			autobot->ResetLives();

			autobot->SetTarget(player_);
			autobot->InitialisePerception(100.f);
			auto weakCollider = autobot->GetComponentOfType<Collider>();
			if (auto coll = weakCollider.lock())
			{
				coll->SetColliderVisible(false);
			}
			autobot->InitObstacleAvoidance(70.f, 25.f);
			autobot->InitThrusterParticles();
			autobot->InitialiseHealthIndicator(20.f);
/*
			if (auto kamikaze = std::dynamic_pointer_cast<KamikazeShip>(autobot))
			{
				kamikaze->InitialiseExplosion(75.f, 3.f);
			}
*/
			autobot->InitialiseChaseFSM(200.f, 175.f);
			autobot->InitialiseAutoShootFSM();
		}
	}
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

void Game::SpawnBackgrounds() const
{
	auto foreground = System::GetInstance()->GetObjectMgr()->CreateGameEntity<Background>({});
	auto background =  System::GetInstance()->GetObjectMgr()->CreateGameEntity<Background>({});

	if (auto fg = foreground.lock())
	{
		// near stars
		fg->Initialise(player_, System::GetInstance()->GetWindowWidth(), System::GetInstance()->GetWindowHeight(), 156, 0.15f);
		fg->SetRenderPriority(-99);
	}

	if (auto bg = background.lock())
	{
		// far stars
		bg->Initialise(player_, System::GetInstance()->GetWindowWidth(), System::GetInstance()->GetWindowHeight(), 64, 0.05f);
		bg->SetRenderPriority(-100);
	}
	
}

void Game::SpawnHUD()
{
	auto gameHUD = System::GetInstance()->GetObjectMgr()->CreateGameEntity<GameHUD>({});
	if (auto hud = gameHUD.lock())
	{
		hud->InitGameHUD();
		hud->SetRenderPriority(100);
	}
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
	Event_ScoreUpdate.Invoke(score);
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

std::weak_ptr<ImplicitGrid> Game::GetCollisionSystem() const { return collisionGrid_; }
std::weak_ptr<GroupAttackingPolicy> Game::GetGroupAttackingPolicy() const { return groupAttackingPolicy_; }