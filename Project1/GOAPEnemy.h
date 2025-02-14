#pragma once
#ifndef GOAPENEMY_H
#define GOAPENEMY_H
#include "GOAP.h"
#include "Enemy.hpp"

class GOAPEnemy : public Enemy {
private:
	float detectionRadius;
public:
	GOAPEnemy(int x,int y,float radius);
	void update(float deltaTime, Grid& grid);
	bool isColliding(Player&);
};

#endif