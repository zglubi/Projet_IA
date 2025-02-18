#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"

class Player : public Entity {
public:
    static constexpr float SPEED = 175.f;
    Player(float x, float y);
    void update(float deltaTime, Grid& grid) override;
};

#endif // PLAYER_HPP