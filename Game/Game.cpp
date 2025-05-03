#include <iostream>
#include <vector>
#include <random>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 1920,1080 }), "Snake Game");

	window.setFramerateLimit(60);

	// Grid settings
	constexpr int cellsize = 32;
	constexpr int gridWidth = 1920 / cellsize;
	constexpr int gridHeight = 1080 / cellsize;

	// Snake is represented as grid positions (x/y)
	std::vector<sf::Vector2i> snake = {
		{10,10}, {9,10}, {8,10}
	};

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

	// Snake Head (for collision checks)
	sf::Vector2i newHead = snake.front();

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
			newHead = snake.front() + direction;
			snake.insert(snake.begin(), newHead);

			// Check if snake eats food
			if (newHead == food)
			{
				// Generate new food not on the snake
				do {
					food = { xDist(rng), yDist(rng) };
				} while (std::find(snake.begin(), snake.end(), food) != snake.end());
			}
			else
			{
				// No food eaten, remove tail
				snake.pop_back();
			}

			// Wall Collision
			if (newHead.x < 0 || newHead.x >= gridWidth ||
				newHead.y < 0 || newHead.y >= gridHeight)
			{
				window.close(); // Game Over
			}

			// Self Collision
			for (size_t i = 1; i < snake.size(); ++i)
			{
				if (snake[i] == newHead)
				{
					window.close(); // Game Over
				}
			}
		}

		// Clear the frame
		window.clear();

		// Drawing the snake
		for (const auto& segment : snake)
		{
			sf::RectangleShape rect({ static_cast<float>(cellsize), static_cast<float>(cellsize) });
			rect.setFillColor(sf::Color::Green);
			rect.setPosition(sf::Vector2f(segment.x * cellsize, segment.y * cellsize));
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
