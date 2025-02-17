#pragma once
#ifndef ENEMYBT_H
#define ENEMYBT_H
#include "Enemy.hpp"
#include "BT.hpp"
#include "Pathfinding.hpp"
#include "Player.hpp"

class EnemyBT : public Enemy
{
private:
	shared_ptr<Player> player;
	unique_ptr<SelectorNode> root;
	Blackboard& blackboard;
	unique_ptr<Pathfinding> pathfinding;
	vector<Vector2f> patrolPath;
	vector<Vector2i> path;
	Vector2i position;
	int currentPathIndex = 0;
	int hp;
	bool next = false;

public:
	EnemyBT(float x, float y, Blackboard& bl, Grid& grid, shared_ptr<Player> pl);

	void update(float deltaTime, Grid& grid) override;

	void patrol(float deltaTime);

	void chase(float deltaTime, Grid& grid);

	void flee(float deltaTime, Grid& grid);
};



#endif // ENEMYBT_H