#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

class Entity {
public:
    sf::RectangleShape shape;
    sf::Vector2f velocity;

    Entity(float x, float y, sf::Color color);
    virtual void update(float deltaTime) = 0;
};

#endif // ENTITY_HPP