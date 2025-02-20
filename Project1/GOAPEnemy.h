
#ifndef GOAPENEMY_H
#define GOAPENEMY_H
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
    virtual void execute(State& state, RectangleShape& shape, Vector2f playerPos, shared_ptr<Player> player, Grid& grid) = 0;
    virtual ~Action() {}

    int totalcost = 0;
    Vector2i position;
    
    int getcost();
    int gettotalCost();
    void settotalCost(int i);
    void increasetotalCost(int i);
    void decreasetotalCost(int i);
};


class FollowAction : public Action {
public:
    FollowAction(Vector2i pos);// chiffres a equilibrer
    bool canExecute(State& state);
    void execute(State& state, RectangleShape& shape, Vector2f playerPos, shared_ptr<Player> player, Grid& grid) override;
};


class AttackAction : public Action {
public:
    AttackAction() : Action(3) {}// chiffres a equilibrer
    bool canExecute(State& state) override;

    void execute(State& state, RectangleShape& shape, Vector2f playerPos, shared_ptr<Player> player, Grid& grid)override;
};
class FleeAction : public Action {
public:
    FleeAction() : Action(0) {}
    bool canExecute(State& state);

    void execute(State& state, RectangleShape& shape, Vector2f playerPos, shared_ptr<Player> player, Grid& grid) override;
};
enum class Goal {
    
    Follow,
    Attack,
    Flee
};
class GOAPPlanner {
public:
    std::vector<Action*> Plan(State& state, Goal goal, Vector2i position);
};

class GOAPAgent {
private:

    GOAPPlanner planner;

public:
    Vector2i position;
    void PerformActions(State& state, RectangleShape& shape, Vector2f playerPos, shared_ptr<Player> player, Grid& grid);

    void PrintState(State& state);
};

class GOAPEnemy : public Enemy {
private:
public:
    float sightDetectionRadius, rangeDetectionRadius;
    Clock damageClock, researchPlayer, attackClock;
    bool reversed=false;
	State state;
    
    GOAPPlanner planner;
    GOAPAgent agent;
    Vector2f position;
    vector<Vector2f> waypoints = {};


	GOAPEnemy(float  x,float y, float sightRadius, float rangeRadius);
	void update(Grid& grid, shared_ptr<Player> );
	bool isColliding(shared_ptr<Player>);
    bool detectPlayer(shared_ptr<Player> player);
    bool detectRangePlayer(shared_ptr<Player> player);
    void attack();
    void patrol();
    void follow(Vector2f playerPos);
    void flee(Vector2f);
    void color(int i);
    void enemyMove(Vector2f direction, Grid& grid) override;
};
bool isPathClear(const Vector2i& start, const Vector2i& end, Grid& grid);
extern unique_ptr<Pathfinding> pathfinding;
extern vector<Vector2f> patrolPath;
extern vector<Vector2i> path;
#endif