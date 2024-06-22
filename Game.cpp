#include "Game.h"

//init static colors
sf::Color Game::Red{255, 69, 69};
sf::Color Game::Green{0, 204, 153};
sf::Color Game::Blue{69, 204, 255};
sf::Color Game::Yellow{255, 204, 69};

Game::Game()
{
	InitVariables();
	InitWindow();
	InitEnemies();
}

void Game::InitVariables()
{
	Window = nullptr;
	VideoMode.height = Height;
	VideoMode.width = Width;
}

void Game::InitWindow()
{
	Window = std::make_unique<sf::RenderWindow>(VideoMode, "Asteroids SFML", sf::Style::Titlebar | sf::Style::Close);
}

void Game::InitEnemies()
{
	std::vector<sf::Vector2f> P{
		{0.f, 20.f},
		{12.5f, 0.f},
		{10.f, 20.f},
		{17.5f, 0.f},
		{30.f, 20.f},
		{20.f, 20.f},
		{0.f, 20.f},
		{30.f, 20.f},
		{15.f, 30.f},};

	std::vector<sf::Vector2f> Verts{
		{10.f, 30.f},
		{0.f, 15.f},
		{15.f, 0.f},
		{30.f, 15.f},
		{20.f, 30.f},
		{15.f, 0.f},
	};

	Enemy.setOrigin(15.f, 15.f);
	Enemy.SetTris(Verts, {255, 69, 69});
	Enemy.setScale(0.75f, 0.75f);
	Enemy.setPosition(50, 50);
}

void Game::Update()
{
}

void Game::Render()
{
	Window->clear();

	// draw entities
	Window->draw(Enemy);

	Window->display();
}

void Game::PollEvents()
{
	while (Window->pollEvent(Event))
	{
		switch (Event.type)
		{
			case sf::Event::Closed:
				Window->close();
				break;
			case sf::Event::KeyPressed:
				if (Event.type == sf::Event::Closed || Event.KeyPressed == sf::Keyboard::Escape)
					Window->close();
				break;
			default: ;
		}
	}
}

bool Game::IsWindowOpen() const
{
	return Window->isOpen();
}

int Game::GetWindowHeight() const { return Height; }
int Game::GetWindowWidth() const { return Width; }
