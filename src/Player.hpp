#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include "Hitbox.hpp"
#include "Tile.hpp"
#include "Camera.hpp"

class Game;

class Player {
    sf::CircleShape shape;
    sf::CircleShape light;
    bool darklit;
    float lightFactor;
    bool isInvincible;
    bool rotateView;
    Game* game;
public:
    Player(Game* game);

    float speed;
    Hitbox hitbox;
    sf::CircleShape goalPointer;
    void set(sf::Vector2f position);
    void setView(int radius);
    float getLightRadius();
    sf::Vector2f getPosition();
    void draw(sf::RenderTarget& win);
    void move(float dmove);
    void rotate(double angle);
    void lightUp(sf::RenderTexture& win);
    void showLight(sf::RenderTarget& win);
    bool checkCollision();
    void findVacantSpot();
    void checkPowerup();
    bool checkWin();

    void loadTextures();

    sf::Texture player_t, goalPointer_t;
};