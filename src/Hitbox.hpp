#pragma once

#include <SFML/System/Vector2.hpp>

class Hitbox {
public:
    float left, right, top, bottom;

    Hitbox();
    Hitbox(float x, float y, float w, float h);
    Hitbox(sf::Vector2f position, sf::Vector2f size);
    void setPosition(float x, float y);
    void setPosition(sf::Vector2f position);
    void setSize(float x, float y);
    void setSize(sf::Vector2f size);
    void move(float dx, float dy);
    void move(sf::Vector2f d);
    sf::Vector2f getPosition();
    sf::Vector2f getSize();
    sf::Vector2f getCenter();
    sf::Vector2f getGlobalCenter();
    bool intersects(Hitbox&);
};
