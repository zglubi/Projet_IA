#ifndef GOAP_HPP
#define GOAP_HPP
#include <iostream>
#include <vector>
#include <string>
#include "Player.hpp"

using namespace std;


class State {
private:
    bool isAttacked = false;
    bool playerInSight=false;
    bool playerInRange=false;
    bool lowHealth=false;
    int hp;
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
    void increaseHp(int i=1);
    void decreaseHp(int i=1);
    int getHp();
};

class Action {
public:
    int cost;

    Action(int c) :cost(c) {};
    virtual bool canExecute( State& state) = 0;
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
    bool canExecute( State& state) override;
    void execute(State& state) override;
    
};

class FollowAction : public Action {
public:
    FollowAction() : Action(2) {}// chiffres a equilibrer
    bool canExecute( State& state);
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
    bool canExecute( State& state);

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
    std::vector<Action*> Plan(State& initialState, Goal goal);
};

class GOAPAgent {
private:
    State state;
    GOAPPlanner planner;

public:
    GOAPAgent();

    void PerformActions();

    void PrintState();
};

#endif
