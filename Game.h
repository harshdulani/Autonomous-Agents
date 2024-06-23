#pragma once
#include "SFML/Graphics.hpp"

class GameEntity;

class Game
{
public:	
	void UpdateWindowData(sf::RenderWindow* Window);
	void PollEvents(sf::RenderWindow* Window);
	void UpdateInput(sf::RenderWindow* Window);

	sf::Vector2f GetMousePosition() const;

	void StartGame();
	
private:
	void InitEnemies();
	void InitPlayer();
	void InitAsteroids();
	
public:

private:
	sf::Event Event;
	sf::Vector2f MousePos;

	// static colors
	static sf::Color Red;
	static sf::Color Green;
	static sf::Color Blue;
	static sf::Color Yellow;
};

