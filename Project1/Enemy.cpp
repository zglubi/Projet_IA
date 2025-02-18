#include "Enemy.hpp"
#include <cmath>

Enemy::Enemy(float x, float y, float sightRadius, float rangeRadius) : Entity(x, y, sf::Color::Red) {
	sightRadiusCircle.setRadius(sightRadius);
	rangeRadiusCircle.setRadius(rangeRadius);
	sightRadiusCircle.setOrigin(sightRadius, sightRadius);
	rangeRadiusCircle.setOrigin(rangeRadius, rangeRadius);
	sightRadiusCircle.setFillColor(sf::Color(255, 0, 0, 100));
	rangeRadiusCircle.setFillColor(sf::Color(0, 0, 255, 100));
	sightRadiusCircle.setOutlineColor(sf::Color::Red);	
	rangeRadiusCircle.setOutlineColor(sf::Color::Blue);
	sightRadiusCircle.setOutlineThickness(1);
	rangeRadiusCircle.setOutlineThickness(1);
	sightRadiusCircle.setPosition(x, y);
	rangeRadiusCircle.setPosition(x, y);
}

void Enemy::update(float deltaTime, Grid& grid) {}

float deltaTime;