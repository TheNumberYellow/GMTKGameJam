#include "SFML/Graphics.hpp"

#include "Tilemap.hpp"
#include "Player.hpp"
#include "MathUtils.hpp"

#include <iostream>

Level levels[10];



void ChangeLevel(Level newLevel, TileMap& tileMap, Player& player)
{
	tileMap.setLevel(newLevel);
	Scale::SetFactor(tileMap.getTileSize().x);
	player.SetSize(tileMap.getTileSize() * 0.8f);
	player.ScaleParameters();
	player.SetPosition(tileMap.getSpawnPosition() + (tileMap.getTileSize() / 2.0f));
	player.ResetTimers();
}

int main()
{
	sf::RenderWindow window;
	window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width / 2, (sf::VideoMode::getDesktopMode().width / 2) * (9.0f / 16.0f)), "Out of Control", sf::Style::Close);
	window.setFramerateLimit(60);

	sf::Vector2f windowSize = sf::Vector2f(window.getSize());

	Scale::SetFactor(windowSize.x / 100.0f);

	TileMap tileMap("images/tileset.png", sf::Vector2u(20, 20), windowSize);
	Player player("images/player.png");

	levels[0] = Level(
		{

			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0,	0, 0, 0, 0,	0, 0, 0, 1,
			1, 0, 0, 0,	0, 0, 0, 5,	0, 0, 0, 1,
			3, 3, 3, 3, 3, 3, 3, 3,	3, 3, 3, 3,
		},
		12, 6,
		sf::Vector2i(2, 4),
		sf::Vector2i(9, 4)
	);

	levels[1] = Level(
		{

			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0,	0, 0, 0, 0,	2, 2, 2, 1,
			1, 0, 0, 0,	0, 0, 0, 1,	1, 1, 1, 1,
			3, 3, 3, 3, 3, 3, 3, 3,	3, 3, 3, 3,
		},
		12, 6,
		sf::Vector2i(2, 4),
		sf::Vector2i(9, 2)
	);

	levels[2] = Level(
		{

			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0,	0, 0, 0, 0,	2, 2, 2, 1,
			1, 2, 2, 0,	0, 2, 0, 2,	2, 2, 2, 1,
			3, 3, 3, 4, 4, 3, 4, 3,	3, 3, 3, 3,
		},
		12, 6,
		sf::Vector2i(2, 3),
		sf::Vector2i(9, 2)
	);

	levels[3] = Level(
		{

			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 4,	4, 0, 0, 4,	4, 0, 0, 1,
			1, 0, 0, 0,	0, 0, 0, 0,	0, 0, 0, 1,
			3, 3, 3, 2, 2, 3, 3, 2,	2, 3, 3, 3,
		},
		12, 4,
		sf::Vector2i(2, 2),
		sf::Vector2i(9, 2)
	);

	levels[4] = Level(
		{

			1, 1, 1, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			3, 0, 0, 3,
			1, 0, 0, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			3, 0, 0, 3,
			1, 0, 0, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			3, 0, 0, 3,
			1, 0, 0, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			3, 0, 0, 3,
			1, 0, 0, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			3, 0, 0, 3,
			1, 0, 0, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			1, 0, 0, 1,
			3, 3, 3, 3,
		},
		4, 36,
		sf::Vector2i(1, 2),
		sf::Vector2i(2, 34)
		);

//	levels[1] = Level(
//		{
//			1, 1, 1, 1, 1,
//			1, 0, 0, 0, 1,
//			1, 0, 0, 0, 1,
//			1, 0, 0, 0, 1,
//			1, 1, 1, 1, 1,
//		},
//		5, 5,
//		sf::Vector2i(1, 3),
//		sf::Vector2i(3, 3)
//	);

	int currentLevel = 0;

	ChangeLevel(levels[currentLevel], tileMap, player);

	bool running = true;
	sf::Clock clock;
	while (running)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				running = false;
			}
		}

		float deltaT = clock.getElapsedTime().asMicroseconds() / 1e6;
		clock.restart();

		player.Update(deltaT, tileMap);
		tileMap.Update(deltaT, player);
		
		if (player.TouchedLevelEnd())
		{
			ChangeLevel(levels[++currentLevel], tileMap, player);
		}
		if (player.Died())
		{
			ChangeLevel(levels[currentLevel], tileMap, player);
			// Or ResetLevel() which could reset position without changing size, player speeds, etc.
		}
		if (player.WantsReset())
		{
			//ResetLevel();
		}

		window.clear();

		window.draw(tileMap);
		window.draw(player);

		window.display();
	}

	return 0;
}
