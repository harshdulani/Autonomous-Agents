#pragma once
#include "Event.h"
#include "SFML/Graphics.hpp"

class GroupAttackingPolicy;
class ImplicitGrid;
class ObjectManager;
class PlayerShip;
class GameEntity;

class Game
{
public:
	void InitLevel(int levelNum);
		
	// spawning smaller asteroids on asteroid hits
	void SpawnAsteroidAt(sf::Vector2f position, int size);
	void SetAsteroidsDestroyed(int val);
	int GetAsteroidsDestroyed() const;

	bool IsLevelComplete() const;
	bool IsGameOver() const;

	// score
	void UpdateScore(int oldSize);
	void ResetScore();

	std::weak_ptr<ImplicitGrid> GetCollisionSystem() const;
	std::weak_ptr<GroupAttackingPolicy> GetGroupAttackingPolicy() const;

private:
	void CreateCollisionSystem();
	
	void SpawnBots(int SpawnCount);
	
	void SpawnAsteroids(int numAsteroids);
	// For Level Complete checking
	int GetAsteroidsAliveCount() const;

	void SpawnPlayer();
	void DeletePlayer();	

	void SpawnBackgrounds() const;
	void SpawnHUD();

	static sf::Vector2f CreateRandomPosition();

public:
	Event<int> Event_ScoreUpdate;
	Event<int> Event_LivesUpdate;

	// For level complete checking
	int AsteroidCount = 0;
	int ExplosionCount = 0;
	
private:
	std::weak_ptr<PlayerShip> player_;
	std::weak_ptr<ImplicitGrid> collisionGrid_;
	std::weak_ptr<GroupAttackingPolicy> groupAttackingPolicy_;

	bool hasSpawnedBots_ = false;

	int totalAsteroids_ = 0;
	int asteroidsDestroyed_ = 0;
	
	int score_ = 0;
	int levelNum_ = 0;

	// static colors
	static sf::Color Red;
	static sf::Color Green;
	static sf::Color Blue;
	static sf::Color Yellow;
};

