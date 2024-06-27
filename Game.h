#pragma once
#include "SFML/Graphics.hpp"

class ObjectManager;
class PlayerShip;
class GameEntity;

class Game
{
public:
	void UpdateInput(sf::RenderWindow* Window);

	sf::Vector2f GetMousePosition() const;

	void StartGame();
	
private:
	void InitEnemies();
	void SpawnPlayer();
	void DeletePlayer();
	void SpawnAsteroids();
	
public:

private:
	sf::Vector2f MousePos;

	std::weak_ptr<PlayerShip> Player;
	
	// static colors
	static sf::Color Red;
	static sf::Color Green;
	static sf::Color Blue;
	static sf::Color Yellow;
};

