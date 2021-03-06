#include "Player.hpp"
#include "Camera.hpp"
#include "Powerups.hpp"
#include "Game.hpp"

void Player::loadTextures()
{
    goalPointer_t.loadFromFile("assets/images/goalPointer.png");
    player_t.loadFromFile("assets/images/player.png");
}

Player::Player(Game* game)
{
    this->game = game;
}

void Player::set(sf::Vector2f position) {
    shape.setRotation(0);
    shape.setRadius(game->tileSize.x * float(0.5));
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(position);
    shape.setTexture(&player_t);

    hitbox = Hitbox();
    hitbox.setSize(game->tileSize * float(0.5));
    hitbox.setPosition(shape.getPosition() - hitbox.getGlobalCenter());

    light.setRotation(0);
    light.setRadius(150);
    light.setOrigin(light.getRadius(), light.getRadius());
    light.setPosition(shape.getPosition());
    light.setPointCount(100);

    goalPointer.setTexture(&goalPointer_t);
    goalPointer.setRadius(180);
    goalPointer.setOrigin(goalPointer.getRadius(), goalPointer.getRadius());
    goalPointer.setPosition(game->window.getSize().x / 2, game->window.getSize().y / 2);
    goalPointer.setRotation(-(180 / 3.14) * atan((light.getPosition().x - game->endTile.hitbox.getGlobalCenter().x) / (light.getPosition().y - game->endTile.hitbox.getGlobalCenter().y)));

    darklit = false;
    lightFactor = 1;
    isInvincible = false;
    rotateView = true;
    speed = 3;
}
void Player::setView(int radius) {
    light.setRadius(radius);
    light.setOrigin(light.getRadius(), light.getRadius());
}
float Player::getLightRadius() {
    return light.getRadius();
}
void Player::draw(sf::RenderTarget& win) {
    game->cam.apply(shape);
    game->cam.apply(hitbox);
    win.draw(shape);
}
void Player::move(float dmove) {
    hitbox.move(sf::Vector2f(dmove * sin((3.14 / 180) * shape.getRotation()), 0));
    if (checkCollision())hitbox.move(sf::Vector2f(-dmove * sin((3.14 / 180) * shape.getRotation()), 0));
    hitbox.move(sf::Vector2f(0, -dmove * cos((3.14 / 180) * shape.getRotation())));
    if (checkCollision())hitbox.move(sf::Vector2f(0, dmove * cos((3.14 / 180) * shape.getRotation())));
    shape.setPosition(hitbox.getGlobalCenter());
}
void Player::rotate(double angle) {
    double prev = shape.getRotation();
    shape.setRotation(angle);
    if (checkCollision())shape.setRotation(prev);
}
void Player::lightUp(sf::RenderTexture& win) {
    game->cam.update(shape.getPosition());
    win.clear(sf::Color(141, 35, 166));//dark violet
    game->drawMap(win);
    draw(win);
    win.display();
    light.setTexture(&win.getTexture());
    light.setTextureRect(sf::IntRect(shape.getPosition().x - light.getRadius() * lightFactor, shape.getPosition().y - light.getRadius() * lightFactor, 2 * light.getRadius() * lightFactor, 2 * light.getRadius() * lightFactor));
}
void Player::showLight(sf::RenderTarget& win) {
    light.setPosition(shape.getPosition());
    if (rotateView)light.setRotation(-shape.getRotation());
    goalPointer.setRotation(-(180 / 3.14) * atan((light.getPosition().x - game->endTile.hitbox.getGlobalCenter().x) / (light.getPosition().y - game->endTile.hitbox.getGlobalCenter().y)));
    if (rotateView)goalPointer.rotate(light.getRotation());
    sf::CircleShape lit;
    if (darklit) {
        lit.setRadius(light.getRadius() + 2);
        lit.setOrigin(lit.getRadius(), lit.getRadius());
        lit.setFillColor(sf::Color(0, 0, 0, 150));
        lit.setPosition(light.getPosition());
        lit.scale(1.2, 1.2);
    }
    light.scale(1.2, 1.2);
    goalPointer.scale(1.2, 1.2);
    win.draw(goalPointer);
    win.draw(light);
    light.scale(1 / 1.2, 1 / 1.2);
    goalPointer.scale(1 / 1.2, 1 / 1.2);
    if (darklit)win.draw(lit);
}
bool Player::checkCollision() {
    if (isInvincible)return false;
    for (unsigned int i = 0; i < game->wall.size(); i++) {
        if (hitbox.intersects(game->wall[i].hitbox))return true;
    }
    return false;
}
void Player::findVacantSpot() {
    bool find = false;
    for (int i = 0; i < game->wall.size(); i++)
    {
        if (hitbox.getGlobalCenter().x < game->wall[i].hitbox.right && hitbox.getGlobalCenter().x > game->wall[i].hitbox.left &&
            hitbox.getGlobalCenter().y < game->wall[i].hitbox.top && hitbox.getGlobalCenter().y > game->wall[i].hitbox.bottom)
            find = true;
    }
    if (find) {
        int di = 0;
        for (unsigned int i = 1; i < game->path.size(); i++) {
            if (abs(game->path[i].hitbox.getGlobalCenter().x - hitbox.getGlobalCenter().x) <= abs(game->path[di].hitbox.getGlobalCenter().x - hitbox.getGlobalCenter().x) &&
                abs(game->path[i].hitbox.getGlobalCenter().y - hitbox.getGlobalCenter().y) <= abs(game->path[di].hitbox.getGlobalCenter().y - hitbox.getGlobalCenter().y))di = i;
        }
        hitbox.setPosition(game->path[di].hitbox.getPosition());
        shape.setPosition(hitbox.getGlobalCenter());
        light.setPosition(hitbox.getGlobalCenter());
    }
}
void Player::checkPowerup() {
    for (auto it = Powerup::ActiveGroup.begin(); it != Powerup::ActiveGroup.end();) {
        if (it->timer.getElapsedTime().asSeconds() >= it->maxTime.asSeconds()) {
            if (it->type == 0)setView(150);
            if (it->type == 1)darklit = false;
            if (it->type == 2)game->visiblePath = false;
            if (it->type == 3)lightFactor = 1;
            if (it->type == 4)lightFactor = 1;
            if (it->type == 5) { findVacantSpot(); isInvincible = false; shape.setFillColor(sf::Color(255, 255, 255, 255)); }
            if (it->type == 6)rotateView = true;
            if (it->type == 7)speed = 3;
            it = Powerup::ActiveGroup.erase(it);
        }
        else it++;
    }
    auto it  = Powerup::Group.begin();
    for (; it != Powerup::Group.end(); it++) {
        if (hitbox.intersects(it->hitbox))break;
    }
    if (it != Powerup::Group.end()) {
        if (it->type == 0)setView(250);
        if (it->type == 1)darklit = true;
        if (it->type == 2)game->visiblePath = true;
        if (it->type == 3)lightFactor *= 2;
        if (it->type == 4)lightFactor /= 2;
        if (it->type == 5) { isInvincible = true;  shape.setFillColor(sf::Color(255, 255, 255, 150)); }
        if (it->type == 6) { light.setRotation(0); rotateView = false; }
        if (it->type == 7)speed /= 2;
        auto ait = Powerup::ActiveGroup.begin();
        for (; ait != Powerup::ActiveGroup.end(); ait++) {
            if (ait->type == it->type)break;
        }
        if (ait == Powerup::ActiveGroup.end()) { 
            Powerup::ActiveGroup.push_back(*it); Powerup::ActiveGroup.back().start(); 
        }
        else ait->start();
        Powerup::Group.erase(it);
    }
    for (int i = 0; i < Powerup::Group.size(); i++)
        Powerup::Group[i].image.setRotation(-light.getRotation());
}
bool Player::checkWin() {
    if (hitbox.intersects(game->endTile.hitbox))return true;
    return false;
}

sf::Vector2f Player::getPosition()
{
    return this->shape.getPosition();
}
