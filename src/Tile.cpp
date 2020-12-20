#include "Tile.hpp"

std::map<std::string, sf::Texture> Tile::allTextures;

Tile::Tile()
{
}

Tile::Tile(sf::Vector2f p, sf::Vector2f size, sf::Texture& image_t) {
    hitbox.setPosition(p);
    hitbox.setSize(size);
    image.setTexture(image_t);
    image.setPosition(hitbox.getPosition());
}
void Tile::update() {
    image.setPosition(hitbox.getPosition());
}

void Tile::loadTextures()
{
    allTextures.insert(std::make_pair("path", sf::Texture()));
    allTextures.insert(std::make_pair("wall", sf::Texture()));
    allTextures.insert(std::make_pair("end", sf::Texture()));
    allTextures.insert(std::make_pair("find", sf::Texture()));

    std::string dir = "assets/images/tiles/";
    allTextures["path"].loadFromFile(dir + "grass.png");
    allTextures["wall"].loadFromFile(dir + "rock.png");
    allTextures["end"].loadFromFile(dir + "end/cave.png");
    allTextures["find"].loadFromFile(dir + "path/flowers.png");
}