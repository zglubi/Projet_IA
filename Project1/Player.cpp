// player.cpp
#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>

Player::Player(float x, float y) : Entity(x, y, sf::Color::Blue) {}

void Player::update(float deltaTime, Grid& grid) {
    sf::Vector2f movement(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) movement.y -= SPEED * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += SPEED * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) movement.x -= SPEED * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += SPEED * deltaTime;

    sf::Vector2f newPosition = Vector2f (shape.getGlobalBounds().left , shape.getGlobalBounds().top) + movement;
    sf::FloatRect newBounds(newPosition, shape.getSize());

    // Vérifier les quatre coins du joueur
    auto isWalkable = [&](float x, float y) {
        int gridX = static_cast<int>(x / CELL_SIZE);
        int gridY = static_cast<int>(y / CELL_SIZE);
        return gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT && grid.getCell(gridX, gridY).walkable;
        };


    if (movement.x > 0)
    {
        if (isWalkable(newBounds.left + newBounds.width - 1, shape.getGlobalBounds().top) &&
            isWalkable(newBounds.left + newBounds.width - 1, shape.getGlobalBounds().top + shape.getGlobalBounds().height))
        {
            shape.move(movement.x, 0);
        }
    }
    else
    {
        if (isWalkable(newBounds.left, shape.getGlobalBounds().top) &&
            isWalkable(newBounds.left, shape.getGlobalBounds().top + shape.getGlobalBounds().height))
        {
            shape.move(movement.x, 0);
        }
    }

    if (movement.y > 0)
    {
        if (isWalkable(shape.getGlobalBounds().left, newBounds.top + newBounds.height - 1) &&
            isWalkable(shape.getGlobalBounds().left + shape.getGlobalBounds().width - 1, newBounds.top + newBounds.height - 1))
        {
            shape.move(0, movement.y);
        }
    }
    else
    {
        if (isWalkable(shape.getGlobalBounds().left, newBounds.top) &&
            isWalkable(shape.getGlobalBounds().left + shape.getGlobalBounds().width - 1, newBounds.top))
        {
            shape.move(0, movement.y);
        }
    }


    //if (isWalkable(newBounds.left, newBounds.top) &&
    //    isWalkable(newBounds.left + newBounds.width - 1, newBounds.top) &&
    //    isWalkable(newBounds.left, newBounds.top + newBounds.height - 1) &&
    //    isWalkable(newBounds.left + newBounds.width - 1, newBounds.top + newBounds.height - 1)) {
    //    shape.move(movement);
    //}
}