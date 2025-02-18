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
};

#endif // ENEMY_HPP