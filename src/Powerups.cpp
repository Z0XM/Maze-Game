#include "powerups.hpp"

std::vector<sf::Texture> Powerup::allTextures;
std::vector<Powerup> Powerup::Group;
std::vector<Powerup> Powerup::ActiveGroup;

Powerup::Powerup(int t, Hitbox& h) {
    type = t;
    hitbox = h;
    active = false;
    isAvailable = true;
    if (type == 0)maxTime = sf::seconds(7);//light increase
    else if (type == 1)maxTime = sf::seconds(7);//brightness
    else if (type == 2)maxTime = sf::seconds(5);//pathfinder
    else if (type == 3)maxTime = sf::seconds(10);//more view in light but distorted
    else if (type == 4)maxTime = sf::seconds(10);//half view
    else if (type == 5)maxTime = sf::seconds(5);//invincibility
    else if (type == 6)maxTime = sf::seconds(7);//no rotation
    else if (type == 7)maxTime = sf::seconds(5);//speed

    image.setTexture(allTextures[type]);
    image.setOrigin(hitbox.getGlobalCenter() - hitbox.getPosition());
    image.setPosition(hitbox.getGlobalCenter());
}
void Powerup::start() {
    active = true;
    isAvailable = false;
    timer.restart();
}
void Powerup::update() {
    image.setPosition(hitbox.getGlobalCenter());
}

void Powerup::loadTextures()
{
    for (int i = 0; i < 8; i++)
        allTextures.push_back(sf::Texture());

    std::string dir = "assets/images/powerups/";

    allTextures[0].loadFromFile(dir + "cyan.png");
    allTextures[1].loadFromFile(dir + "black.png");
    allTextures[2].loadFromFile(dir + "yellow.png");
    allTextures[3].loadFromFile(dir + "black.png");
    allTextures[4].loadFromFile(dir + "black.png");
    allTextures[5].loadFromFile(dir + "white.png");
    allTextures[6].loadFromFile(dir + "violet.png");
    allTextures[7].loadFromFile(dir + "black.png");
}
