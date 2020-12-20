#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include "Tile.hpp"
#include "Camera.hpp"
#include "Player.hpp"

class Game
{
private:
	sf::Font font;
	sf::Vector2i mapSize[6];
    int levels, currentLvl;
    sf::Sprite menuImg;
    Player player;
    std::vector<sf::Vector2i> pathXY;
    sf::Clock mainClock;
    sf::Time clockTime;
    int nMinutes, nSeconds;
    sf::Texture clockDigit_t[11];
    sf::Sprite clockDigits[11];

    void InitTextures();
    void InitWindow();
    void InitVariables();

public:
	Game();
	~Game();

    sf::Vector2f tileSize;
	enum State { PAUSED, MENU, RUNNING, END , LEVEL};
    State state;
	void setup(State);
    void draw(sf::RenderWindow&);
    void update(sf::Vector2f mousePos);

    void setMap();
    void drawMap(sf::RenderTarget& window);
    void loadNextLevel();
    void pathFind(int lvl);
    void resetClock();

    sf::RenderTexture window;
    std::vector<Tile> path, wall, shortestPath, nonWall;
    Tile endTile;
    bool visiblePath;
    bool hasGameStarted;
    Camera cam;
};