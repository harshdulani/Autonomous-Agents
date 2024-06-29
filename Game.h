#pragma once
#include "SFML/Graphics.hpp"

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

	bool IsLevelComplete() const;
	bool IsGameOver() const;

	// For GameOver checking
	void UpdateLivesRemaining(int lives);
	// score
	void UpdateScore(int oldSize);
	void ResetScore();

private:
	void CreateCollisionSystem();
	
	void SpawnBots(int SpawnCount);
	
	void SpawnAsteroids(int numAsteroids);
	// For Level Complete checking
	int GetAsteroidsAliveCount() const;

	void SpawnPlayer();
	void DeletePlayer();	

	static sf::Vector2f CreateRandomPosition();

public:

private:
	std::weak_ptr<PlayerShip> player_;
	std::weak_ptr<ImplicitGrid> collisionGrid_;

	sf::Text scoreText_;
	sf::Text livesText_;
	sf::Font uiFont_;
	
	bool hasSpawnedBots_ = false;

	// For level complete checking
	int asteroidCount_ = 0;
	int totalAsteroids_ = 0;
	int asteroidsDestroyed = 0;
	
	int score = 0;
	int levelNum_ = 0;
	int explosionCount = 0;

	// static colors
	static sf::Color Red;
	static sf::Color Green;
	static sf::Color Blue;
	static sf::Color Yellow;
};

