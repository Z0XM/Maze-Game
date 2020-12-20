#include "camera.hpp"

Camera::Camera(sf::Vector2f size) {
    pos = sf::Vector2f(0, 0);
    winSize = size;
}
void Camera::apply(sf::Shape& s) {
    s.move(pos);
}
void Camera::apply(sf::Sprite& s) {
    s.move(pos);
}
void Camera::apply(Hitbox& h) {
    h.move(pos);
}
void Camera::update(sf::Vector2f p) {
    pos.x = winSize.x / 2 - p.x;
    pos.y = winSize.y / 2 - p.y;
}
sf::Vector2f Camera::getPosition() {
    return pos;
}