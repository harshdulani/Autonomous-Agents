#pragma once
#include "Event.h"
#include "SFML/Graphics.hpp"
#include <memory>

class EnemyShip;
class GroupAttackingPolicy;
class ImplicitGrid;
class ObjectManager;
class PlayerShip;
class GameEntity;

class Game
{
friend class System;
	
public:
	~Game();
	void InitGame();
	void InitLevel();
	void ResetGame();
	
	// spawning smaller asteroids on asteroid hits
	void SpawnAsteroidAt(sf::Vector2f position, int size);
	void SetAsteroidsDestroyed(int val);
	int GetAsteroidsDestroyed() const;

	void QueueNextLevel();
	bool IsLevelComplete() const;
	bool IsGameOver() const;

	// score
	void UpdateScore(int oldSize);
	void ResetScore();

	std::weak_ptr<ImplicitGrid> GetCollisionSystem() const;
	std::weak_ptr<GroupAttackingPolicy> GetGroupAttackingPolicy() const;

private:
	void CreateCollisionSystem();
	void StartNextLevelDeferred();	
	void SpawnBots(int SpawnCount);
		
	void SpawnAsteroids(int numAsteroids);
	// For Level Complete checking
	int GetAsteroidsAliveCount() const;
	int GetEnemiesAliveCount() const;

	void OnEnemyShipCreated(std::weak_ptr<EnemyShip> enemyShip);
	void OnEnemyShipDestroyed(std::weak_ptr<EnemyShip> enemyShip);

	void SpawnPlayer();
	void DeletePlayer();

	void SpawnPortals();

	void SpawnBackgrounds() const;
	void SpawnHUD();

	static sf::Vector2f CreateRandomPosition();

public:
	Event<int> Event_ScoreUpdate;
	Event<int> Event_LivesUpdate;
	
	Event<> Event_LevelStart;
	
	Event<> Event_ShutdownGame;
	
	Event<std::weak_ptr<EnemyShip>> Event_EnemyShipCreated;
	Event<std::weak_ptr<EnemyShip>> Event_EnemyShipDestroyed;
	
	// For level complete checking
	int AsteroidCount = 0;
	int ExplosionCount = 0;
	bool bDefer = false;

private:
	std::weak_ptr<PlayerShip> player_;
	std::weak_ptr<ImplicitGrid> collisionGrid_;
	std::weak_ptr<GroupAttackingPolicy> groupAttackingPolicy_;

	bool hasSpawnedBots_ = false;

	int totalAsteroids_ = 0;
	int asteroidsDestroyed_ = 0;
	int enemyCount_ = 0;

	uint32_t eventHandle_EnemyShipCreated_ = 0;
	uint32_t eventHandle_EnemyShipDestroyed_ = 0;
	
	int score_ = 0;
	// this is here bc we will be creating a fsm for the game states and they will change levels
	int currentLevel_ = 1;

	// static colors
	static sf::Color Red;
	static sf::Color Green;
	static sf::Color Blue;
	static sf::Color Yellow;
};

