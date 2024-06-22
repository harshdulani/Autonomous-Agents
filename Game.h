#pragma once

#include "TriConcave.h"
#include "SFML/Graphics.hpp"

class Game
{
public:
	Game();
	
	void Update();
	void Render();
	void PollEvents();

	bool IsWindowOpen() const;
	int GetWindowWidth() const;
	int GetWindowHeight() const;

private:
	void InitVariables();
	void InitWindow();
	void InitEnemies();
	
public:

private:
	std::unique_ptr<sf::RenderWindow> Window;
	sf::VideoMode VideoMode;
	sf::Event Event;

	int Height = 480;
	int Width = 640;

	TriConcave Enemy;
	TriConcave Player;

	// static colors
	static sf::Color Red;
	static sf::Color Green;
	static sf::Color Blue;
	static sf::Color Yellow;
};

