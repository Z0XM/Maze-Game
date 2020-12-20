#pragma once

#include <SFML/Graphics.hpp>
#include "Hitbox.hpp"
#include <map>

class Tile {
public:
    Hitbox hitbox;
    sf::Sprite image;

    Tile();
    Tile(sf::Vector2f position, sf::Vector2f size, sf::Texture& image_t);
    void update();

    static void loadTextures();

    static std::map<std::string, sf::Texture> allTextures;
};
