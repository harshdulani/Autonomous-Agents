#include <iostream>
#include "Game.h"
#include "Asteroid.h"
#include "Component.h"
#include "GameEntity.h"
#include "Math.h"

//init static colors
sf::Color Game::Red{255, 69, 69};
sf::Color Game::Green{0, 204, 153};
sf::Color Game::Blue{69, 204, 255};
sf::Color Game::Yellow{255, 204, 69};

Game::Game()
{
	Init();
	InitAsteroids();
}

void Game::Init()
{
	VideoMode.height = static_cast<unsigned int>(Height);
	VideoMode.width = static_cast<unsigned int>(Width);
	Window = std::make_shared<sf::RenderWindow>(VideoMode, "Asteroids SFML", sf::Style::Titlebar | sf::Style::Close);
	//Window->setFramerateLimit(165);
}

void Game::InitEnemies()
{
	std::vector<sf::Vector2f> Verts{
		{10.f, 30.f},
		{0.f, 15.f},
		{15.f, 0.f},
		{30.f, 15.f},
		{20.f, 30.f},
		{15.f, 0.f},
	};

	/*
	Enemy.setOrigin(15.f, 15.f);
	Enemy.SetTris(Verts, {255, 69, 69});
	Enemy.setScale(0.75f, 0.75f);
	Enemy.setPosition(50, 50);
	*/
}

void Game::InitPlayer()
{
}

void Game::InitAsteroids()
{
	int NumAsteroids = Math::GetRandInt(3, 10);
	for(int i = 0; i < NumAsteroids; ++i)
	{
		auto roid = std::make_shared<Asteroid>();
		Entities.push_back(roid);
		roid->setPosition(Math::GetRandFloat(0, GetWindowWidth()),
			Math::GetRandFloat(0, GetWindowHeight()));
		roid->InitAsteroid(Math::GetRandFloat(15.f, 30.f), sf::Color::Cyan);
	}
}

void Game::WrapEntity(GameEntity* Entity) const
{
	auto EntityPosition = Entity->getPosition();
	if (EntityPosition.x > GetWindowWidth() || EntityPosition.x < 0 ||
		EntityPosition.y > GetWindowHeight() || EntityPosition.y < 0)
	{
		EntityPosition.x = Math::WrapModulo(EntityPosition.x, 0, GetWindowWidth());
		EntityPosition.y = Math::WrapModulo(EntityPosition.y, 0, GetWindowHeight());
		Entity->setPosition(EntityPosition);
	}
}

void Game::Update(float DeltaTime)
{
	// poll events
	PollEvents();
	UpdateInput();

	// update Entities, Components & Drawables (separately bc drawables not [yet] integrated as objects)
	for (auto& Entity : Entities)
	{
		Entity->Update(DeltaTime);
		WrapEntity(Entity.get());
		for (auto& Component : Entity->Components)
		{
			if (Component.get())
			{
				Component->Update(DeltaTime);
			}
		}
	}
}

void Game::Render() const
{
	Window->clear();

	// draw entities
	for (const auto& Entity : Entities)
	{
		Entity->Render(Window.get());
		for (auto& Drawer : Entity->Drawables)
		{
			if (Drawer.get())
			{
				sf::RenderStates states;
				states.transform *= Entity->getTransform();
				if (auto Transformer = std::dynamic_pointer_cast<sf::Transformable>(Drawer))
				{
					states.transform *= Transformer->getTransform();
				}
				Window->draw(*Drawer, states);
			}
		}
	}

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

void Game::UpdateInput()
{
	MousePos = Window->mapPixelToCoords(sf::Mouse::getPosition(*Window));
	//std::cout << MousePos.x << ", " << MousePos.y << '\n';
}

bool Game::IsWindowOpen() const
{
	return Window->isOpen();
}

float Game::GetWindowWidth() const { return Width; }
float Game::GetWindowHeight() const { return Height; }
sf::Vector2f Game::GetMousePosition() const { return MousePos; }
