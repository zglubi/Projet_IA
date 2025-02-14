#pragma once
#ifndef ENEMYBT_H
#define ENEMYBT_H
#include "Enemy.hpp"
#include "BT.hpp"

class EnemyBT
{
private:
	unique_ptr<BTNode> root;
	int hp;

public:
	EnemyBT(float x, float y);
};



#endif // ENEMYBT_H