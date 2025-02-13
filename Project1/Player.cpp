// player.cpp
#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>

Player::Player(float x, float y) : Entity(x, y, sf::Color::Blue) {}

void Player::update(float deltaTime) {
    sf::Vector2f movement(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) movement.y -= SPEED * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += SPEED * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) movement.x -= SPEED * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += SPEED * deltaTime;
    shape.move(movement);
}
