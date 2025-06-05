#include <iostream>
#include <random>
#include "Array.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>


int main()
{
	sf::RenderWindow window(sf::VideoMode({ 1920,1080 }), "Snake Game", sf::State::Windowed);

	window.setFramerateLimit(60);

	// Grid settings
	constexpr int cellsize = 32;
	constexpr int gridWidth = 1920 / cellsize;
	constexpr int gridHeight = 1080 / cellsize;

	// we are allocating 2 columns x and y axis, using this instead of vector for preallocation instead of dynamic resizing
	Array2D<int> snake(2, 20);
	snake.Get(0, 0) = 10;
	snake.Get(1, 0) = 10;
	snake.Get(0, 1) = 9;
	snake.Get(1, 1) = 10;
	snake.Get(0, 2) = 8;
	snake.Get(1, 2) = 10;

	int snakeLength = 3;

	// Vector storing movement of snake (right)
	sf::Vector2i direction = { 1,0 };

	// Timer to handle snake movement 0.15 is the interval
	sf::Clock movementClock;
	const float moveDelay = 0.15f;

	// Random Generator food
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> xDist(0, gridWidth - 1);
	std::uniform_int_distribution<int> yDist(0, gridHeight - 1);

	// Food Position
	sf::Vector2i food = { xDist(rng), yDist(rng) };

	// Snake Head need to use our Array 
	sf::Vector2i newHead = { snake.Get(0,0), snake.Get(1,0) };

	//Game Loop starts 
	while (window.isOpen())
	{
		// Event Processing 
		while (const std::optional event = window.pollEvent())
		{
			// If window is closed
			if (event->is<sf::Event::Closed>() ||
				(event->is<sf::Event::KeyPressed>() &&
					event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape))
			{
				window.close();
			}

			if (event->is<sf::Event::KeyPressed>())
			{
				auto key = event->getIf<sf::Event::KeyPressed>()->code;

				if (key == sf::Keyboard::Key::W && direction != sf::Vector2i{ 0, 1 }) direction = { 0, -1 };
				if (key == sf::Keyboard::Key::S && direction != sf::Vector2i{ 0, -1 }) direction = { 0, 1 };
				if (key == sf::Keyboard::Key::A && direction != sf::Vector2i{ 1, 0 }) direction = { -1, 0 };
				if (key == sf::Keyboard::Key::D && direction != sf::Vector2i{ -1, 0 }) direction = { 1, 0 };
			}
		}

		// Snake Movement
		if (movementClock.getElapsedTime().asSeconds() >= moveDelay)
		{
			movementClock.restart();

			// Add new head in the current direction
			sf::Vector2i currentHead = { snake.Get(0,0), snake.Get(1,0) };
			newHead = currentHead + direction;

			// Check if we need to Resize
			if (snakeLength >= snake.Height())
			{
				snake.Resize(2, snake.Height() * 2); // double the buffer
			}

			// Shift all segments down one row
			for (int i = snakeLength; i > 0; i--)
			{
				snake.Get(0, i) = snake.Get(0, i - 1); // copy x 
				snake.Get(1, i) = snake.Get(1, i - 1); // copy y
			}

			// We place the new head at row 0
			snake.Get(0, 0) = newHead.x;
			snake.Get(1, 0) = newHead.y;
			snakeLength++;
			if (newHead == food)
			{
				bool foodOnSnake;
				do {
					food = { xDist(rng), yDist(rng) };
					foodOnSnake = false;
					for (int i = 0; i < snakeLength; i++)
					{
						if (snake.Get(0, i) == food.x && snake.Get(1, i) == food.y)
						{
							foodOnSnake = true;
							break;
						}
					}
				} while (foodOnSnake);
			}
			else
			{
				snakeLength--;
			}
		
			// Wall Collision
			if (newHead.x < 0 || newHead.x >= gridWidth ||
				newHead.y < 0 || newHead.y >= gridHeight)
			{
				window.close(); // Game Over
			}

			for (int i = 1; i < snakeLength; ++i)
			{
				if (snake.Get(0, i) == newHead.x && snake.Get(1, i) == newHead.y)
				{
					window.close();
				}
			}

		}

		// Clear the frame
		window.clear();

		// Drawing the snake
		for (int i = 0; i < snakeLength; i++)
		{
			sf::RectangleShape rect({ static_cast<float>(cellsize),static_cast<float>(cellsize) });
			rect.setFillColor(sf::Color::Green);

			float xPos = static_cast<float>(snake.Get(0, i) * cellsize);
			float yPos = static_cast<float>(snake.Get(1, i) * cellsize);

			rect.setPosition(sf::Vector2f(xPos, yPos));
			window.draw(rect);
		}

		// Draw Food
		sf::RectangleShape foodRect({ static_cast<float>(cellsize), static_cast<float>(cellsize) });
		foodRect.setFillColor(sf::Color::Red);
		float xPos = static_cast<float>(food.x * cellsize);
		float yPos = static_cast<float>(food.y * cellsize);
		sf::Vector2f position(xPos, yPos);
		foodRect.setPosition(position);
		window.draw(foodRect);

		// Update thhe frame 
		window.display();
	}

	return 0;
}