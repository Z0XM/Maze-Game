#pragma once

#include <SFML/Graphics.hpp>
#include "Hitbox.hpp"
#include <vector>

class Powerup {
public:
    int type;
    Hitbox hitbox;
    sf::Sprite image;
    sf::Clock timer;
    sf::Time maxTime;
    bool active;
    bool isAvailable;

    Powerup(int, Hitbox&);
    void start();
    void update();

    static void loadTextures();

    static  std::vector<sf::Texture> allTextures;
    static std::vector<Powerup> Group;
    static std::vector<Powerup> ActiveGroup;
};

