#pragma once
#ifndef GOAPENEMY_H
#define GOAPENEMY_H
#include "GOAP.h"
#include <iostream>
#include <vector>
#include <string>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Pathfinding.hpp"
#include "Grid.hpp"
using namespace std;
class State {
private:
    bool isAttacked = false;
    bool playerInSight = false;
    bool playerInRange = false;
    bool lowHealth = false;
    int hp = 0;
public: 
    bool getisAttacked() const;
    void setisAttacked(bool attack);
    bool getplayerInSight();
    void setplayerInSight(bool sight);
    bool getplayerInRange();
    void setplayerInRange(bool range);
    bool getlowHealth();
    void setlowHealth(bool health);
    void setHp(int i);
    void increaseHp(int i = 1);
    void decreaseHp(int i = 1);
    int getHp();
};

class Action {
public:
    int cost;
    Action(int c) :cost(c) {};
    virtual bool canExecute(State& state) = 0;
    virtual void execute(State& state) = 0;
    virtual ~Action() {}

    int totalcost = 0;

    int getcost();

    int gettotalCost();
    void settotalCost(int i);
    void increasetotalCost(int i);
    void decreasetotalCost(int i);
};

class PatrolAction : public Action {
public:
    PatrolAction() : Action(1) {}// chiffres a equilibrer
    bool canExecute(State& state) override;
    void execute(State& state) override;

};

class FollowAction : public Action {
public:
    FollowAction() : Action(2) {}// chiffres a equilibrer
    bool canExecute(State& state);
    void execute(State& state) override;
};


class AttackAction : public Action {
public:
    AttackAction() : Action(4) {}// chiffres a equilibrer
    bool canExecute(State& state) override;

    void execute(State& state) override;
};
class FleeAction : public Action {
public:
    FleeAction() : Action(0) {}
    bool canExecute(State& state);

    void execute(State& state) override;
};
enum class Goal {
    Patrol,
    Follow,
    Attack,
    Flee
};
class GOAPPlanner {
public:
    std::vector<Action*> Plan(State& state, Goal goal);
};

class GOAPAgent {
private:

    GOAPPlanner planner;

public:

    void PerformActions(State& state, RectangleShape shape);

    void PrintState(State& state);
};


class GOAPEnemy : public Enemy {
private:
	float detectionRadius;
public:
    bool reversed;
	State state;
    AttackAction attackaction;
    FleeAction fleeaction;
    FollowAction followaction;
    PatrolAction patrolaction;
    GOAPPlanner planner;
    GOAPAgent agent;
	Clock damageClock,researchPlayer, attackClock;
    Vector2f position;
    vector<Vector2f> waypoints = {};
	GOAPEnemy(int x,int y,float radius);
	void update(float deltaTime, Grid& grid, Player& );
	bool isColliding(Player&);
    bool detectPlayer(Vector2f playerPos);
    bool detectRangePlayer(Vector2f playerPos);
    void patrol();
    void chase(Vector2f playerPos);
    void attack();
    void flee(Vector2f);
};

#endif