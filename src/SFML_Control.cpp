#include "SFML_Control.hpp"
#include "GUI.hpp"
#include "Game.hpp"
#include <iostream>

void SFML_Control::InitWindow()
{
	window.create(sf::VideoMode(500, 600), "mazeGame");
	window.setKeyRepeatEnabled(false);
	window.setFramerateLimit(60);
}

SFML_Control::SFML_Control()
{
	this->InitWindow();
	this->running = true;
	game.setup(Game::MENU);
}

SFML_Control::~SFML_Control()
{
	gui::deleteAllGuiGroups();
}

bool SFML_Control::isRunning()
{
	return this->running;
}

void SFML_Control::update()
{
	this->pollEvents();
	if (this->game.hasGameStarted)this->game.update(this->getMousePosition());
}

void SFML_Control::pollEvents()
{
	sf::Event event;
	sf::Vector2f mousePos = this->getMousePosition();
	while (this->window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			this->running = false;

		if (event.type == sf::Event::MouseButtonPressed) {
			gui::registerClick(event.mouseButton.button);
			if (this->game.state == Game::RUNNING && !this->game.hasGameStarted) {
				this->game.hasGameStarted = true;
				this->game.resetClock();
			}
		}
		else if (event.type == sf::Event::MouseButtonReleased) {
			gui::unregisterClick(event.mouseButton.button, this->getMousePosition());
			if (this->game.state == Game::END)this->running = false;
		}
		else if (event.type == sf::Event::MouseMoved) {
			gui::updateMousePointer(this->getMousePosition());
		}
		else if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Escape) {
				if (this->game.state == Game::RUNNING)this->game.setup(Game::PAUSED);
				else if (this->game.state == Game::PAUSED)this->game.setup(Game::RUNNING);
			}
		}
	}
}

void SFML_Control::render()
{		
	this->window.clear();

	game.draw(this->window);
	gui::render(this->window);
	this->window.display();
}

sf::Vector2f SFML_Control::getMousePosition()
{
	return window.mapPixelToCoords(sf::Mouse::getPosition(this->window));
}