#pragma once

#include <SFML/Graphics.hpp>

#include "Hitbox.hpp"

class Camera {
    sf::Vector2f pos;
    sf::Vector2f winSize;
public:
    Camera(sf::Vector2f);
    void apply(sf::Shape&);
    void apply(sf::Sprite&);
    void apply(Hitbox&);
    void update(sf::Vector2f);
    sf::Vector2f getPosition();
};
