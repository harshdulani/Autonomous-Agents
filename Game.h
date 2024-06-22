#pragma once

#include "TriConcave.h"
#include "SFML/Graphics.hpp"

class GameEntity;

class Game
{
public:
	Game();
	
	void Update(float DeltaTime);
	void Render() const;
	void PollEvents();
	void UpdateInput();

	bool IsWindowOpen() const;
	float GetWindowWidth() const;
	float GetWindowHeight() const;

	sf::Vector2f GetMousePosition() const;

private:
	void Init();
	void InitEnemies();
	void InitPlayer();
	void InitAsteroids();
	void WrapEntity(GameEntity* Entity) const;
	
public:

private:
	std::vector<std::shared_ptr<GameEntity>> Entities;
	
	std::shared_ptr<sf::RenderWindow> Window;
	sf::VideoMode VideoMode;
	sf::Event Event;
	sf::Vector2f MousePos;

	float Width = 800.f;
	float Height = 600.f;

	// static colors
	static sf::Color Red;
	static sf::Color Green;
	static sf::Color Blue;
	static sf::Color Yellow;
};

