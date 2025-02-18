#include "GOAPEnemy.h"

Clock attackCD;

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
void PatrolAction::execute(State& state, RectangleShape& shape, Vector2f playerPos) {
    cout << "L'agent patrouille.\n";
    //patrol();
}

bool FollowAction::canExecute(State& state) {
    return state.getplayerInSight();
}
void FollowAction::execute(State& state, RectangleShape& shape,Vector2f playerPos) {
    cout << "L'agent suit le joueur.\n";
    Vector2f position = shape.getPosition();
    //cout << position.x<<" "<<position.y<<endl;
    Vector2f direction = playerPos - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    //if (shape.getPosition().y >= 350) { patrol(); }
    //if (shape.getPosition().y <= 250) { patrol(); }
    //if (shape.getPosition().x >= 600) { patrol(); }
    //if (shape.getPosition().x <= 0) { patrol(); }
    if (distance > 0) {
        direction /= distance;
        position += direction * 150.0f * deltaTime;
    }

    //cout << direction.x << " " << direction.y;

    shape.move(direction);
    
    //shape.setPosition(position);
}

bool AttackAction::canExecute(State& state) {
    cout << "test";
    return state.getplayerInRange();
}
void AttackAction::execute(State& state, RectangleShape& shape, Vector2f playerPos) {
    cout << "L'agent attaque le joueur.\n";
    cout << "Elapsed time: " << attackCD.getElapsedTime().asSeconds() << " seconds\n";
    if (attackCD.getElapsedTime().asSeconds() > 1) {
        state.increaseHp();
        attackCD.restart();
    }
}

bool FleeAction::canExecute(State& state) {
    return state.getlowHealth();
}
void FleeAction::execute(State& state, RectangleShape& shape, Vector2f playerPos) {
    cout << "L'agent fuit.\n";
    if (attackCD.getElapsedTime().asSeconds() > 1) {
        state.decreaseHp(2);// chiffres a equilibrer
        attackCD.restart();
    }
    state.setlowHealth(false);
}

vector<Action*> GOAPPlanner::Plan(State& state, Goal goal)
{
    vector<Action*> plan;
    // SEUIL CRITIQUE N�1 (hp>10)
    if (state.getHp() > 10) {
        goal = Goal::Flee;
    }
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
 
GOAPEnemy::GOAPEnemy(float x, float y, float sightRadius, float rangeRadius) 
    : sightDetectionRadius(sightRadius), rangeDetectionRadius(rangeRadius), Enemy(x, y, sightRadius, rangeRadius)
{
    position = Vector2f(x, y);
    state.setHp(0);
    shape.setOutlineColor(Color::Yellow); shape.setOutlineThickness(2); 
}


void GOAPEnemy::update(float deltaTime, Grid& grid, shared_ptr<Player> player){
 //   if (player.shape.getGlobalBounds().intersects(shape.getGlobalBounds())) { // to be removed
	//	if (damageClock.getElapsedTime().asSeconds() > 1) {
 //           cout << state.getlowHealth()<<endl;
 //           cout << "hp GOAPenemy " << state.getHp() << endl;
 //           state.increaseHp(); // si rien dans () alors increaseHP(1);;
 //           cout << "hp GOAPenemy " << state.getHp() << endl;
 //           cout << state.getlowHealth() << endl;
 //           damageClock.restart();
	//	}
	//}
    sightRadiusCircle.setPosition(shape.getPosition().x, shape.getPosition().y);
    rangeRadiusCircle.setPosition(shape.getPosition().x, shape.getPosition().y);
    if (detectPlayer(player)) { state.setplayerInSight(true); } else  state.setplayerInSight(false);
    if (detectRangePlayer(player)) { state.setplayerInRange(true); }else state.setplayerInRange(false);



    color(state.getHp());
    if (state.getHp() >= 5) {
        state.setlowHealth(true);
    }
    else { state.setlowHealth(false); }




    agent.PerformActions(state,shape,player->shape.getPosition());

    agent.PrintState(state);

}

bool GOAPEnemy::isColliding(shared_ptr<Player> player) {
	return player->shape.getGlobalBounds().intersects(shape.getGlobalBounds());
}

void GOAPAgent::PerformActions(State& state,RectangleShape& shape, Vector2f playerPos) {
    
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
            totalCost1 += 1; // moins enclin � patrol si bas en vie, mais prefere patrol que follow si bas en vie
        }
    }
    for (auto action : plan2) {
        totalCost2 += action->gettotalCost();
        if (state.getlowHealth()) {
            totalCost1 += 2;// moins enclin a follow si bas en vie
        }
        if (state.getplayerInRange()) {
            totalCost2 += 2; // si peut attaquer, pr�f�re attaquer mais ne plus follow
        }
        if (state.getplayerInSight()) {
            totalCost2 -= 5; // envie (fr�n�tique) de follow � vue
        }
    }
    for (auto action : plan3) {
        totalCost2 += action->gettotalCost();
        if (state.getlowHealth()) {
            totalCost1 += 2; // moins enclin � attaquer si bas en vie
        }
        else if (!state.getlowHealth() and state.getplayerInRange()) {
            totalCost3 -= 50; // si pas bas en vie et an range, volont�, d'attaquer, � 100%
        }
    }
    //if (totalCost1 < totalCost2 and totalCost1 < totalCost3) {

    //for (auto action : plan1) {
    //    if (action->canExecute(state)) {
    //        action->execute(state,shape,playerPos);  // Ex�cute l'action
    //    }
    //    else {
    //        cout << "Action impossible : " << typeid(*action).name() << "\n";
    //    }
    //    delete action;  // Lib�rer la m�moire
    //}
    //}
    if (totalCost2 < totalCost1 and totalCost2 < totalCost3) {
        for (auto action : plan2) {
            if (action->canExecute(state)) {
                action->execute(state, shape, playerPos);  // Ex�cute l'action
            }
            else {
                cout << "Action impossible : " << typeid(*action).name() << "\n";
            }
            delete action;  // Lib�rer la m�moire
        }
    }
    else if (totalCost3 < totalCost1 and totalCost3 < totalCost2) {
        for (auto action : plan3) {
            if (action->canExecute(state)) {
                action->execute(state, shape, playerPos);  // Ex�cute l'action
            }
            else {
                cout << "Action impossible : " << typeid(*action).name() << "\n";
            }
            delete action;  // Lib�rer la m�moire
        }
    }
}

bool GOAPEnemy::detectPlayer(shared_ptr<Player> player) {
    bool isColliding;
    //float distance = sqrt(pow(playerPos.x - shape.getPosition().x , 2) + pow(playerPos.y - shape.getPosition().y, 2));
    //if (distance < sightDetectionRadius) { shape.setFillColor(Color::Red); }*
    if (sightRadiusCircle.getGlobalBounds().intersects(player->shape.getGlobalBounds())) { isColliding = true; shape.setFillColor(Color::Green); }
    else { isColliding = false; shape.setFillColor(Color::Green); }
    return (isColliding);
}

bool GOAPEnemy::detectRangePlayer(shared_ptr<Player> player) {
    bool isColliding;
    /*float distance = sqrt(pow(playerPos.x - shape.getPosition().x, 2) + pow(playerPos.y - shape.getPosition().y, 2));
    if (distance < rangeDetectionRadius) { cout << "uzu"; }*/
    if (rangeRadiusCircle.getGlobalBounds().intersects(player->shape.getGlobalBounds())) { isColliding = true; cout << "uzu"; }
    else { isColliding = false; }
    return isColliding;
}

void GOAPEnemy::patrol() {
    position = shape.getPosition();
    cout << "e";
    //waypoints.clear();
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
    //shape.setPosition(position);
    shape.move(position);
}

void GOAPEnemy::follow(Vector2f playerPos) {
    position = shape.getPosition();
    //cout << position.x<<" "<<position.y<<endl;
    Vector2f direction = playerPos - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    //if (shape.getPosition().y >= 350) { patrol(); }
    //if (shape.getPosition().y <= 250) { patrol(); }
    //if (shape.getPosition().x >= 600) { patrol(); }
    //if (shape.getPosition().x <= 0) { patrol(); }
    if (distance > 0) {
        direction /= distance;
        position += direction * 150.0f * deltaTime;
    }
    
    //cout << direction.x << " " << direction.y;
    shape.move(direction);
    //shape.setPosition(position);
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
void GOAPEnemy::color(int i) {
    switch (i) {
    case 0: shape.setOutlineColor(Color::Black); break;
    case 3: shape.setOutlineColor(Color::Cyan);break;
    case 5: shape.setOutlineColor(Color::White); break;
    case 8: shape.setOutlineColor(Color::Yellow);;
    case 10: shape.setOutlineColor(Color::Red); break;
    case 15: shape.setOutlineColor(Color::Magenta);break;
    }
}