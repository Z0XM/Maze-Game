#include "Hitbox.hpp"

Hitbox::Hitbox() :left(0), top(0), right(0), bottom(0)
{
}
Hitbox::Hitbox(float x, float y, float w, float h) : left(x), right(y), top(x + w), bottom(y + h)
{
}
Hitbox::Hitbox(sf::Vector2f pos, sf::Vector2f size) : left(pos.x), right(pos.x + size.x), top(pos.y), bottom(pos.y + size.y)
{
}
void Hitbox::setPosition(float x, float y) {
    sf::Vector2f size = getSize();
    left = x; right = left + size.x;
    top = y; bottom = top + size.y;
}
void Hitbox::setPosition(sf::Vector2f pos) {
    sf::Vector2f size = getSize();
    left = pos.x; right = left + size.x;
    top = pos.y; bottom = top + size.y;
}
void Hitbox::move(float dx, float dy) {
    sf::Vector2f size = getSize();
    left += dx; right = left + size.x;
    top += dy; bottom = top + size.y;
}
void Hitbox::move(sf::Vector2f d) {
    sf::Vector2f size = getSize();
    left += d.x; right = left + size.x;
    top += d.y; bottom = top + size.y;
}
sf::Vector2f Hitbox::getPosition() {
    return sf::Vector2f(left, top);
}
sf::Vector2f Hitbox::getSize() {
    return sf::Vector2f(right - left, bottom - top);
}
void Hitbox::setSize(float w, float h) {
    right = left + w;
    bottom = top + h;
}
void Hitbox::setSize(sf::Vector2f s) {
    right = left + s.x;
    bottom = top + s.y;
}
sf::Vector2f Hitbox::getCenter() {
    return getSize() * 0.5f;
}
sf::Vector2f Hitbox::getGlobalCenter() {
    return getPosition() + getCenter();
}
bool Hitbox::intersects(Hitbox& rect) {
    if (left < rect.right && right > rect.left && top < rect.bottom && bottom > rect.top)return true;
    return false;
}
