#pragma once
#include <SFML/Graphics.hpp>

#include "Game.hpp"

class SFML_Control {
private:
	sf::RenderWindow window;

	Game game;
	bool running;

	void InitWindow();
public:
	SFML_Control();
	~SFML_Control();

	bool isRunning();

	void update();
	void pollEvents();
	void render();

	sf::Vector2f getMousePosition();
};
