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
	vector<Vector2f> availablePatrolPath;
	vector<Vector2i> patrolBackPath;
	bool isPatroling = false;
	vector<Vector2i> path;
	Vector2i position;
	int currentPathIndex = 0;
	int hp;
	bool next = false;

public:
	EnemyBT(float x, float y, float sightRadius, float rangeRadius, Blackboard& bl, Grid& grid, shared_ptr<Player> pl);

	void update(float deltaTime, Grid& grid) override;

	void patrol(float deltaTime, Grid& grid);

	void chase(float deltaTime, Grid& grid);

	void flee(float deltaTime, Grid& grid);

	bool isPathClear(const Vector2i& start, const Vector2i& end, Grid& grid);
};



#endif // ENEMYBT_H