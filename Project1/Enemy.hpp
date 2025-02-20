#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"
#include "Player.hpp"

extern float deltaTime;


class Enemy : public Entity {
public:
    CircleShape sightRadiusCircle,rangeRadiusCircle;
    static constexpr float SPEED = 100.0f;
    Enemy(float x, float y,float sightRadius,float rangeRadius);
    void update(float deltaTime, Grid& grid) override;
    virtual void enemyMove(Vector2f direction, Grid& grid) = 0;
};

#endif // ENEMY_HPP