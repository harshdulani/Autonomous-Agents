#include "Game.h"
#include "Asteroid.h"
#include "Background.h"
#include "Ships/EnemyShip.h"
#include "GameHUD.h"
#include "Physics/ImplicitGrid.h"
#include "Maths.h"
#include "ObjectManager.h"
#include "Ships/PlayerShip.h"
#include "System.h"
#include "Ships/GroupAttackingPolicy.h"
#include "Portal/Portal.h"

//init static colors
sf::Color Game::Red{255, 69, 69};
sf::Color Game::Green{0, 204, 153};
sf::Color Game::Blue{69, 204, 255};
sf::Color Game::Yellow{255, 204, 69};

Game::~Game()
{
	if (Event_ShutdownGame.GetSubscriberCount() > 0)
		Event_ShutdownGame.Invoke();
}

void Game::InitGame()
{
	CreateCollisionSystem();
	SpawnPlayer();
	SpawnHUD();
	
	Event_EnemyShipCreated.Subscribe(BindSubscriber(&Game::OnEnemyShipCreated, this));
	Event_EnemyShipDestroyed.Subscribe(BindSubscriber(&Game::OnEnemyShipDestroyed, this));
	
	groupAttackingPolicy_ = System::GetInstance()->GetObjectMgr()->CreateObject<GroupAttackingPolicy>();
	SpawnBackgrounds();
	
	InitLevel();
}

void Game::InitLevel()
{
	SetAsteroidsDestroyed(0);
	totalAsteroids_ = 0;
	enemyCount_ = 0;
	hasSpawnedBots_ = false;
	
	SpawnAsteroids(currentLevel_);
	
	SpawnBots(3);
	
	for (auto Portal : System::GetInstance()->GetObjectMgr()->GetAllEntitiesOfType<Portal>())
	{
		Portal.lock()->Kill();
	}
	SpawnPortals();	
	
	Event_LevelStart.Invoke();
}

void Game::ResetGame()
{
	for (auto Asteroid : System::GetInstance()->GetObjectMgr()->GetAllEntitiesOfType<Asteroid>())
	{
		Asteroid.lock()->Kill();
	}	
	for (auto Ship : System::GetInstance()->GetObjectMgr()->GetAllEntitiesOfType<EnemyShip>())
	{
		Ship.lock()->Kill();
	}
	for (auto Portal : System::GetInstance()->GetObjectMgr()->GetAllEntitiesOfType<Portal>())
	{
		Portal.lock()->Kill();
	}
	
	SpawnPlayer();
	currentLevel_ = 1;
	ResetScore();
}

void Game::CreateCollisionSystem()
{
	collisionGrid_ = System::GetInstance()->GetObjectMgr()->CreateGameEntity<ImplicitGrid>(Maths::ZeroVector());
	if (auto Grid = collisionGrid_.lock())
	{
		Grid->InitGrid(
			static_cast<int>(System::GetInstance()->GetWindowWidth()),
			static_cast<int>(System::GetInstance()->GetWindowHeight()));
		Grid->SetRenderPriority(-101);

		Grid->SetupDebugControls();
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

		float random = Maths::GetRandFloat(0.f, 1.f);
		if (random > 0.75f)
		{
			//left wall
			x = Maths::GetRandFloat(0.f, 25.f);
			y = Maths::GetRandFloat(Height * 0.1f, Height * 0.9f);
		}
		else if (random > 0.5f)
		{
			//top wall
			x = Maths::GetRandFloat(Width * 0.1f, Width * 0.9f);
			y = Height - Maths::GetRandFloat(0.f, 25.f);
		}
		else if (random > 0.25f)
		{
			//right wall
			x = Width - Maths::GetRandFloat(0.f,25.f);
			y = Maths::GetRandFloat(Height * 0.1f, Height * 0.9f);
		}
		else //if (random > 0.f)
		{
			//bottom wall
			x = Maths::GetRandFloat(Width * 0.1f, Width * 0.9f);
			y = -Height + Maths::GetRandFloat(0.f,25.f);
		}

		std::weak_ptr<EnemyShip> bot;
		bot = System::GetInstance()->GetObjectMgr()->CreateGameEntity<EnemyShip>({x, y});

		if (auto autobot = bot.lock())
		{
			autobot->SetTotalLives(2);
			autobot->ResetLives();
			autobot->SetTarget(player_);
			autobot->Initialise();

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

void Game::SpawnPortals()
{
	std::weak_ptr<Portal> orangePortal = System::GetInstance()->GetObjectMgr()->CreateGameEntity<Portal>(CreateRandomPosition());
	std::weak_ptr<Portal> bluePortal = System::GetInstance()->GetObjectMgr()->CreateGameEntity<Portal>(CreateRandomPosition());

	constexpr float portalRadius = 10.0f * 3;

	if (auto orange = orangePortal.lock())
	{
		orange->SetPortalRadius(portalRadius);
		orange->CreateCollider(portalRadius);
		orange->SetLinkedPortal(bluePortal);

		orange->InitialiseFSM();
		orange->AddVisuals();
	}
	if (auto blue = bluePortal.lock())
	{
		blue->SetPortalRadius(portalRadius);
		blue->CreateCollider(portalRadius);
		blue->SetLinkedPortal(orangePortal);

		blue->InitialiseFSM();
		blue->AddVisuals();
	}
}

void Game::SpawnBackgrounds() const
{
	auto foreground = System::GetInstance()->GetObjectMgr()->CreateGameEntity<Background>(Maths::ZeroVector());
	auto background =  System::GetInstance()->GetObjectMgr()->CreateGameEntity<Background>(Maths::ZeroVector());

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
	auto gameHUD = System::GetInstance()->GetObjectMgr()->CreateGameEntity<GameHUD>(Maths::ZeroVector());
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
	return {Maths::GetRandFloat(0, System::GetInstance()->GetWindowWidth()),
			Maths::GetRandFloat(0, System::GetInstance()->GetWindowHeight())};
}

void Game::SpawnAsteroidAt(sf::Vector2f position, int size)
{
	const float MAX_ASTEROID_SPEED = 1.0f;

	float angularVelocity = Maths::GetRandFloat(0.f, MAX_ASTEROID_SPEED);

	std::weak_ptr<Asteroid> asteroid = System::GetInstance()->GetObjectMgr()
															->CreateGameEntity<Asteroid>(position);

	if (std::shared_ptr<Asteroid> roid = asteroid.lock())
	{
		//@todo: randomise (between) colors
		roid->InitAsteroid(size * 5.f,
						   sf::Color::Cyan,
						   angularVelocity, size);
	}
	AsteroidCount++;
	totalAsteroids_++;
}

int Game::GetAsteroidsAliveCount() const
{
	return totalAsteroids_ - asteroidsDestroyed_;
}

int Game::GetEnemiesAliveCount() const
{
	return enemyCount_;
}

void Game::OnEnemyShipCreated(std::weak_ptr<EnemyShip> enemyShip)
{
	enemyCount_++;
}

void Game::OnEnemyShipDestroyed(std::weak_ptr<EnemyShip> enemyShip)
{
	enemyCount_--;
	
	//StartNextLevelIfDone();
}

void Game::UpdateScore(const int oldSize)
{
	score_ += 5 * oldSize;
	Event_ScoreUpdate.Invoke(score_);
}

void Game::ResetScore()
{
	score_ = 0;
}

void Game::SetAsteroidsDestroyed(int val)
{
	asteroidsDestroyed_ = val;
	if (asteroidsDestroyed_ < 2 || hasSpawnedBots_)
	{
		return;
	}
	hasSpawnedBots_ = true;
	int spawnCount = std::min(4, currentLevel_ - 1);
	if (spawnCount > 0)
	{
		SpawnBots(spawnCount);
	}
}

int Game::GetAsteroidsDestroyed() const { return asteroidsDestroyed_; }

void Game::StartNextLevelDeferred()
{
	currentLevel_++;
	InitLevel();
	bDefer = false;
}

void Game::QueueNextLevel()
{
	if (IsLevelComplete())
		bDefer = true;
}

bool Game::IsLevelComplete() const
{
	return (GetAsteroidsAliveCount() == 0 && GetEnemiesAliveCount() == 0);
}

bool Game::IsGameOver() const
{
	if (std::shared_ptr<PlayerShip> player = player_.lock())
	{
		return (player->GetLivesLeft() <= 0 && ExplosionCount == 0);
	}
	return true;
}

std::weak_ptr<ImplicitGrid> Game::GetCollisionSystem() const { return collisionGrid_; }
std::weak_ptr<GroupAttackingPolicy> Game::GetGroupAttackingPolicy() const { return groupAttackingPolicy_; }