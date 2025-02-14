#include "GOAPEnemy.h"
bool State::getisAttacked() const { return isAttacked; } void State::setisAttacked(bool attack) { isAttacked = attack; }
bool State::getplayerInSight() { return playerInSight; } void State::setplayerInSight(bool sight) { playerInSight = sight; }
bool State::getplayerInRange() { return playerInRange; } void State::setplayerInRange(bool range) { playerInRange = range; }
bool State::getlowHealth() { return lowHealth; }	     void State::setlowHealth(bool health) { lowHealth = health; }

void State::setHp(int i) { hp = i; }
void State::increaseHp(int i) { hp += i; }
void State::decreaseHp(int i) { hp -= i; }
int State::getHp() { return hp; }

int Action::getcost() { return cost; }

int Action::gettotalCost() { return totalcost; }
void Action::settotalCost(int i) { totalcost = i; }
void Action::increasetotalCost(int i) { totalcost += i; }
void Action::decreasetotalCost(int i) { totalcost -= i; }

bool PatrolAction::canExecute(State& state) {
    return !state.getplayerInRange();
}
void PatrolAction::execute(State& state) {
    cout << "L'agent patrouille.\n";
}

bool FollowAction::canExecute(State& state) {
    return state.getplayerInSight();
}
void FollowAction::execute(State& state) {
    cout << "L'agent suit le joueur.\n";
}

bool AttackAction::canExecute(State& state) {
    return state.getplayerInRange();
}
void AttackAction::execute(State& state) {
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

vector<Action*> GOAPPlanner::Plan(State& state, Goal goal)
{
    vector<Action*> plan;

    if (goal == Goal::Patrol) {
        if (!state.getplayerInSight() and !state.getplayerInRange()) {
            plan.push_back(new PatrolAction());
        }
    }
    if (goal == Goal::Follow) {
        if (state.getplayerInSight() and !state.getplayerInRange())
            plan.push_back(new FollowAction());
    }
    if (goal == Goal::Attack) {
        if (state.getplayerInRange()) {
            plan.push_back(new AttackAction());
        }
    }
    if (goal == Goal::Flee) {
        if (state.getlowHealth()) {
            plan.push_back(new FleeAction());
        }
    }
    return plan;
}

void GOAPAgent::PrintState(State& state) {
    srand(time(0));
    cout << "Vie: " << state.getHp() << "\n";
    cout << "Est bas en vie: " << (state.getlowHealth() ? "Oui" : "Non") << "\n";
    cout << "Est en vision: " << (state.getplayerInSight() ? "Oui" : "Non") << "\n";
    cout << "Est en range: " << (state.getplayerInRange() ? "Oui" : "Non") << "\n\n";
}
 
GOAPEnemy::GOAPEnemy(int x, int y, float radius) : detectionRadius(radius), Enemy(x, y) 
{
    state.setHp(0);
    shape.setOutlineColor(Color::Yellow); shape.setOutlineThickness(2); 
}

void GOAPEnemy::update(float deltaTime, Grid& grid, Player& player){
    if (player.shape.getGlobalBounds().intersects(shape.getGlobalBounds())) {
		if (damageClock.getElapsedTime().asSeconds() > 1) {
            cout << state.getlowHealth()<<endl;
            cout << "hp GOAPenemy " << state.getHp() << endl;
            state.increaseHp(); // si rien dans () alors increaseHP(1);;
            cout << "hp GOAPenemy " << state.getHp() << endl;
            cout << state.getlowHealth() << endl;
            damageClock.restart();
		}
	}
    agent.PerformActions(state, shape);


    agent.PrintState(state);

}

bool GOAPEnemy::isColliding(Player& player) {
	return player.shape.getGlobalBounds().intersects(shape.getGlobalBounds());
}

void GOAPAgent::PerformActions(State& state,RectangleShape shape) {
    if (state.getHp() >= 5) {
        state.setlowHealth(true);
        shape.setOutlineColor(Color::Cyan);
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

bool GOAPEnemy::detectPlayer(Vector2f playerPos) {
    float distance = sqrt(pow(playerPos.x - shape.getPosition().x, 2) + pow(playerPos.y - shape.getPosition().y, 2));
    if (distance < detectionRadius) { shape.setFillColor(Color::Red); }
    else { shape.setFillColor(Color::Green); }
    return (distance < detectionRadius);
}

bool GOAPEnemy::detectRangePlayer(Vector2f playerPos) {
    float distance = sqrt(pow(playerPos.x - shape.getPosition().x, 2) + pow(playerPos.y - shape.getPosition().y, 2));
    if (distance < detectionRadius) { attack(); }
    return distance < detectionRadius;
}

void GOAPEnemy::patrol() {
    cout << "e";
    /* waypoints.clear();*/    
    cout << waypoints.size();
    static int currentWaypoint = 0;
    Vector2f target = waypoints[currentWaypoint];
    Vector2f direction = target - position;
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);


    cout << (int)shape.getPosition().x/* / 40*/ << " " << (int)shape.getPosition().y/* / 40*/ << endl;
    if (Vector2i(shape.getPosition().x/* / 40*/, shape.getPosition().y/* / 40*/) == Vector2i(11*40, 7*40) and !reversed) {
        cout << "eeee";
        currentWaypoint = 0;
        reverse(waypoints.begin(), waypoints.end());
        reversed = true;
    }
    if (Vector2i(shape.getPosition().x / 40, shape.getPosition().y / 40) == Vector2i(2, 2) and reversed) {
        currentWaypoint = 0;
        reverse(waypoints.begin(), waypoints.end());
        reversed = false;
    }
    if (distance < 5.0f) {
        currentWaypoint = (currentWaypoint + 1) % 5000;
    }
    else {
        direction /= distance;
        position += direction * 150.0f * deltaTime;
    }
    shape.setPosition(position);
}

void GOAPEnemy::chase(Vector2f playerPos) {

    Vector2f direction = playerPos - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (shape.getPosition().y >= 350) { patrol(); }
    if (shape.getPosition().y <= 250) { patrol(); }
    if (shape.getPosition().x >= 600) { patrol(); }
    if (shape.getPosition().x <= 0) { patrol(); }
    if (distance > 0) {
        direction /= distance;
        position += direction * 150.0f * deltaTime;
    }
    shape.setPosition(position);
}

void GOAPEnemy::attack() { // attaquer donne des hp a l'ennemi et change la couleur en fonction de ses hp;;
    if (attackClock.getElapsedTime().asSeconds() > 1) {
        state.increaseHp();
        attackClock.restart();
    }
}
void GOAPEnemy::flee(Vector2f playerPos) {
    //waypoints.clear();
    //oppositePositionX = 2 * shape.getPosition().x - playerPos.x; // nouvelle direction de enemy = symetrie de pos du joueur par rapport a pos de enemy
    //oppositePositionY = 2 * shape.getPosition().y - playerPos.y;
    //for (Vector2i pos : Pathfinding::findPath(grid, Vector2i(shape.getPosition().x / 40, shape.getPosition().y / 40), Vector2i(oppositePositionX / 40, oppositePositionY / 40)))
    //{
    //    waypoints.push_back(Vector2f(pos));
    //}
    //shape.setPosition(Vector2f(oppositePositionX, oppositePositionY));

    //DOESNT WORK ATM // OPPOSITEPOSITIONS TO BE REVIEW AND CORRECTED ASAP;;
}
