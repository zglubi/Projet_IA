#include "Entity.hpp"

Entity::Entity(float x, float y, sf::Color color) {
    shape.setSize({ 40, 40 });
    shape.setPosition(x, y);
    shape.setFillColor(color);
}
