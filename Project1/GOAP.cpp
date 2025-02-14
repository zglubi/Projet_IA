#include "GOAP.h"
State* State::getInstance() {
    static State* instance = new State();
    return instance;
}
State* state = State::getInstance();

bool State::getisAttacked() const { return isAttacked; } void State::setisAttacked(bool attack) { isAttacked = attack; }
bool State::getplayerInSight() { return playerInSight; } void State::setplayerInSight(bool sight) { playerInSight = sight; }
bool State::getplayerInRange() { return playerInRange; } void State::setplayerInRange(bool range) { playerInRange = range; }
bool State::getlowHealth() { return lowHealth; }	     void State::setlowHealth(bool health) { isAttacked = health; }

void State::setHp(int i) { hp = i; }
void State::increaseHp(int i) { hp += i; }
void State::decreaseHp(int i) { hp -= i; }
int State::getHp() { return hp; }

int Action::getcost() { return cost; }

int Action::gettotalCost() { return totalcost; }
void Action::settotalCost(int i) { totalcost = i; }
void Action::increasetotalCost(int i) { totalcost += i; }
void Action::decreasetotalCost(int i) { totalcost -= i; }

bool PatrolAction::canExecute(State& state)  {
    return !state.getplayerInRange();
}
void PatrolAction::execute(State& state)  {
    cout << "L'agent patrouille.\n";
    
}

bool FollowAction::canExecute(State& state)  {
    return state.getplayerInSight();
}
void FollowAction::execute(State& state)  {
    cout << "L'agent suit le joueur.\n";
}

bool AttackAction::canExecute(State& state)  {
    return state.getplayerInRange();
}
void AttackAction::execute(State& state)  {
    cout << "L'agent attaque le joueur.\n";
    state.increaseHp();
}

bool FleeAction::canExecute(State& state) {
    return state.getlowHealth();
}
void FleeAction::execute(State& state) {
    cout << "L'agent fuit.\n";
    state.setlowHealth(false);
}

GOAPAgent::GOAPAgent() {
    state.setisAttacked(true);
    state.setHp(0);
}
void GOAPAgent::PerformActions(){
    if (state.getHp() >= 3) {
        state.setlowHealth(true);
    }
    else { state.setlowHealth(false); }

    int totalCost1 = 0, totalCost2 = 0, totalCost3 = 0;
    Goal goal1 = Goal::Patrol;
    Goal goal2 = Goal::Follow;
    Goal goal3 = Goal::Attack;

    vector<Action*> plan1 = planner.Plan(state, goal1);
    vector<Action*> plan2 = planner.Plan(state, goal2);
    vector<Action*> plan3 = planner.Plan(state, goal3);

    for (auto action : plan1) {
        totalCost1 += action->gettotalCost();
        if (state.getlowHealth()) {
            totalCost1 += 1;// chiffres a equilibrer
        }
    }
    //for (auto action : plan2) {
    //    totalCost2 += action->gettotalCost();
    //    if (state.getlowHealth()) {
    //        totalCost1 += 2;// chiffres a equilibrer
    //    }
    //}
    //for (auto action : plan3) {
    //    totalCost2 += action->gettotalCost();
    //    if (state.getlowHealth()) {
    //        totalCost1 += 4;// chiffres a equilibrer
    //        state.decreaseHp(2);// chiffres a equilibrer
    //    }
    //    else if (!state.getlowHealth() and state.getplayerInRange()) {
    //        totalCost3 -= 50; // chiffres a equilibrer
    //    }
    //}
    //if (totalCost1 < totalCost2 and totalCost1 < totalCost3) {

        for (auto action : plan1) {
            if (action->canExecute(state)) {
                action->execute(state);  // Exécute l'action
            }
            else {
                cout << "Action impossible : " << typeid(*action).name() << "\n";
            }
            delete action;  // Libérer la mémoire
        }
    //}
    //else if (totalCost2 < totalCost1 and totalCost2 < totalCost3) {
    //    for (auto action : plan2) {
    //        if (action->canExecute(state)) {
    //            action->execute(state);  // Exécute l'action

    //        }
    //        else {
    //            cout << "Action impossible : " << typeid(*action).name() << "\n";
    //        }
    //        delete action;  // Libérer la mémoire
    //    }
    //}
    //else if (totalCost3 < totalCost1 and totalCost3 < totalCost2) {
    //    for (auto action : plan3) {
    //        if (action->canExecute(state)) {
    //            action->execute(state);  // Exécute l'action
    //        }
    //        else {
    //            cout << "Action impossible : " << typeid(*action).name() << "\n";
    //        }
    //        delete action;  // Libérer la mémoire
    //    }
    //}
}
vector<Action*> GOAPPlanner::Plan(State& initialState, Goal goal)
{
    vector<Action*> plan;

    if (goal == Goal::Patrol) {
        if (!initialState.getplayerInSight() and !initialState.getplayerInRange()) {
            plan.push_back(new PatrolAction());
        }
    }
    if (goal == Goal::Follow) {
        if (initialState.getplayerInSight() and !initialState.getplayerInRange())
            plan.push_back(new FollowAction());
    }
    if (goal == Goal::Attack) {
        if (initialState.getplayerInRange()) {
            plan.push_back(new AttackAction());
        }
    }
    if (goal == Goal::Flee) {
        if (initialState.getlowHealth()) {
            plan.push_back(new FleeAction());
        }
    }
    return plan;
}

void GOAPAgent::PrintState() {
    srand(time(0));
    cout << "Vie: " << state.getHp() << "\n";
    cout << "Est bas en vie: " << (state.getlowHealth() ? "Oui" : "Non") << "\n";
    cout << "Est en vision: " << (state.getplayerInSight() ? "Oui" : "Non") << "\n";
    cout << "Est en range: " << (state.getplayerInRange() ? "Oui" : "Non") << "\n\n";
}