#include "GOAPEnemy.h"
unique_ptr<Pathfinding> pathfinding;
vector<Vector2f> patrolPath;
vector<Vector2i> path;
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

FollowAction::FollowAction(Vector2i pos) : Action(2) {
    position = pos;
}
bool FollowAction::canExecute(State& state) {
    return state.getplayerInSight();
}
void FollowAction::execute(State& state, RectangleShape& shape, Vector2f playerPos, shared_ptr<Player> player, Grid& grid) {
    static bool next = false;
    Vector2f velocity;
    float SPEED = 150;
    auto isWalkable = [&](float x, float y) {
        int gridX = static_cast<int>(x / CELL_SIZE);
        int gridY = static_cast<int>(y / CELL_SIZE);
        return gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT && grid.getCell(gridX, gridY).Ewalkable;
        };

    if (isPathClear(Vector2i(shape.getGlobalBounds().left / 40, shape.getGlobalBounds().top / 40), Vector2i(player->shape.getGlobalBounds().left / 40, player->shape.getGlobalBounds().top / 40), grid) &&
        isPathClear(Vector2i((shape.getGlobalBounds().left + shape.getGlobalBounds().width) / 40, shape.getGlobalBounds().top / 40), Vector2i((player->shape.getGlobalBounds().left + player->shape.getGlobalBounds().width) / 40, player->shape.getGlobalBounds().top / 40), grid) &&
        isPathClear(Vector2i(shape.getGlobalBounds().left / 40, (shape.getGlobalBounds().top + shape.getGlobalBounds().height) / 40), Vector2i(player->shape.getGlobalBounds().left / 40, (player->shape.getGlobalBounds().top + player->shape.getGlobalBounds().height) / 40), grid) &&
        isPathClear(Vector2i((shape.getGlobalBounds().left + shape.getGlobalBounds().width) / 40, (shape.getGlobalBounds().top + shape.getGlobalBounds().height) / 40), Vector2i((player->shape.getGlobalBounds().left + player->shape.getGlobalBounds().width) / 40, (player->shape.getGlobalBounds().top + player->shape.getGlobalBounds().height) / 40), grid))
    {
        if (
            !isWalkable(player->shape.getGlobalBounds().left, player->shape.getGlobalBounds().top) ||
            !isWalkable(player->shape.getGlobalBounds().left + player->shape.getGlobalBounds().width, player->shape.getGlobalBounds().top) ||
            !isWalkable(player->shape.getGlobalBounds().left, player->shape.getGlobalBounds().top + player->shape.getGlobalBounds().height) ||
            !isWalkable(player->shape.getGlobalBounds().left + player->shape.getGlobalBounds().width, player->shape.getGlobalBounds().top + player->shape.getGlobalBounds().height))
        {
            return;
        }
        Vector2f direction = Vector2f(player->shape.getPosition().x - shape.getPosition().x, player->shape.getPosition().y - shape.getPosition().y);
        direction = Vector2f(direction.x / length(direction), direction.y / length(direction));
        velocity = Vector2f(direction.x * SPEED, direction.y * SPEED);
        shape.move(Vector2f(velocity.x * 0.016, velocity.y * 0.016));
    }
    else
    {


        path = pathfinding->findPath(grid, Vector2i(shape.getPosition().x / 40, shape.getPosition().y / 40), Vector2i(player->shape.getPosition().x / 40, player->shape.getPosition().y / 40));
        if (path.size() == 0 || (
            !isWalkable(player->shape.getGlobalBounds().left, player->shape.getGlobalBounds().top) ||
            !isWalkable(player->shape.getGlobalBounds().left + player->shape.getGlobalBounds().width, player->shape.getGlobalBounds().top) ||
            !isWalkable(player->shape.getGlobalBounds().left, player->shape.getGlobalBounds().top + player->shape.getGlobalBounds().height) ||
            !isWalkable(player->shape.getGlobalBounds().left + player->shape.getGlobalBounds().width, player->shape.getGlobalBounds().top + player->shape.getGlobalBounds().height)))
        {
            return;
        }
        if (position != Vector2i(shape.getPosition().x / 40, shape.getPosition().y / 40))
        {
            position = Vector2i(shape.getPosition().x / 40, shape.getPosition().y / 40);

            next = false;
        }

        if ((static_cast<int>(shape.getPosition().x) % 40 < 25 && static_cast<int>(shape.getPosition().x) % 40 > 15 && static_cast<int>(shape.getPosition().y) % 40 < 25 && static_cast<int>(shape.getPosition().y) % 40 > 15) && !next)
        {
            next = true;
            position = Vector2i(shape.getPosition().x / 40, shape.getPosition().y / 40);
        }

        if ((static_cast<int>(shape.getPosition().x) % 40 >= 25 || static_cast<int>(shape.getPosition().x) % 40 <= 15 || static_cast<int>(shape.getPosition().y) % 40 >= 25 || static_cast<int>(shape.getPosition().y) % 40 <= 15) && !next)
        {
            Vector2f direction = Vector2f(position.x * 40 + 20, position.y * 40 + 20) - shape.getPosition();
            direction = Vector2f(direction.x / length(direction), direction.y / length(direction));
            velocity = Vector2f(direction.x * SPEED, direction.y * SPEED);
            shape.move(Vector2f(velocity.x * 0.016, velocity.y * 0.016));
        }
        else if (next)
        {
            Vector2f direction;
            if (path.size() > 1)
            {
                direction = Vector2f(path[1].x - position.x, path[1].y - position.y);
            }
            else if (path.size() == 1)
            {
                direction = Vector2f(path[0].x - position.x, path[0].y - position.y);
            }
            else
            {
                direction = Vector2f(player->shape.getPosition().x - position.x, player->shape.getPosition().y - position.y);
            }

            direction = Vector2f(direction.x / length(direction), direction.y / length(direction));
            velocity = Vector2f(direction.x * SPEED, direction.y * SPEED);
            shape.move(Vector2f(velocity.x * 0.016, velocity.y * 0.016));
        }
    }
}

bool AttackAction::canExecute(State& state) {
    return state.getplayerInRange();
}
void AttackAction::execute(State& state, RectangleShape& shape, Vector2f playerPos, shared_ptr<Player> player, Grid& grid) {
    if (attackCD.getElapsedTime().asSeconds() > 1) {
        state.increaseHp();
        attackCD.restart();
    }
}

bool FleeAction::canExecute(State& state) {
    return state.getlowHealth();
}
void FleeAction::execute(State& state, RectangleShape& shape, Vector2f playerPos, shared_ptr<Player> player, Grid& grid) {
    float SPEED = 150.f;
    Vector2f direction = Vector2f(shape.getPosition().x - player->shape.getPosition().x, shape.getPosition().y - player->shape.getPosition().y);
    direction = Vector2f(direction.x / length(direction), direction.y / length(direction));
    Vector2f velocity = Vector2f(direction.x * SPEED, direction.y * SPEED);

    Vector2f newPosition = Vector2f(shape.getGlobalBounds().left, shape.getGlobalBounds().top) + velocity * 0.016f;
    FloatRect newBounds(newPosition, shape.getSize());

    auto isWalkable = [&](float x, float y) {
        int gridX = static_cast<int>(x / CELL_SIZE);
        int gridY = static_cast<int>(y / CELL_SIZE);
        return gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT && grid.getCell(gridX, gridY).Ewalkable;
        };


    if (velocity.x > 0)
    {
        if (isWalkable(newBounds.left + newBounds.width - 1, shape.getGlobalBounds().top) &&
            isWalkable(newBounds.left + newBounds.width - 1, shape.getGlobalBounds().top + shape.getGlobalBounds().height - 1))
        {
            shape.move(Vector2f(velocity.x * 0.016, 0));
        }
    }
    else
    {
        if (isWalkable(newBounds.left, shape.getGlobalBounds().top) &&
            isWalkable(newBounds.left, shape.getGlobalBounds().top + shape.getGlobalBounds().height - 1))
        {
            shape.move(Vector2f(velocity.x * 0.016, 0));
        }
    }

    if (velocity.y > 0)
    {
        if (isWalkable(shape.getGlobalBounds().left, newBounds.top + newBounds.height - 1) &&
            isWalkable(shape.getGlobalBounds().left + shape.getGlobalBounds().width - 1, newBounds.top + newBounds.height - 1))
        {
            shape.move(Vector2f(0, velocity.y * 0.016));
        }
    }
    else
    {
        if (isWalkable(shape.getGlobalBounds().left, newBounds.top) &&
            isWalkable(shape.getGlobalBounds().left + shape.getGlobalBounds().width - 1, newBounds.top))
        {
            shape.move(Vector2f(0, velocity.y * 0.016));
        }
    }

    if (attackCD.getElapsedTime().asSeconds() > 5) {
        state.decreaseHp(5);// chiffres a equilibrer
        attackCD.restart();
    }
    state.setlowHealth(false);
}

vector<Action*> GOAPPlanner::Plan(State& state, Goal goal,Vector2i position)
{
    vector<Action*> plan;
    // SEUIL CRITIQUE N°1 (hp>10)
    if (state.getHp() > 10) {
        goal = Goal::Flee;
    }
    if (goal == Goal::Follow) {
        if (state.getplayerInSight() and !state.getplayerInRange())
            plan.push_back(new FollowAction(position));
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

void GOAPEnemy::update(Grid& grid, shared_ptr<Player> player) {
    sightRadiusCircle.setRadius(sightDetectionRadius);
    rangeRadiusCircle.setRadius(rangeDetectionRadius);
    sightRadiusCircle.setPosition(shape.getPosition().x, shape.getPosition().y);
    rangeRadiusCircle.setPosition(shape.getPosition().x, shape.getPosition().y);
    if (detectPlayer(player)) { state.setplayerInSight(true); }
    else  state.setplayerInSight(false);
    if (detectRangePlayer(player)) { state.setplayerInRange(true); }
    else state.setplayerInRange(false);


    color(state.getHp());
    if (state.getHp() > 10) {
        state.setlowHealth(true);
    }
    else { state.setlowHealth(false); }

    agent.PerformActions(state, shape, player->shape.getPosition(), player, grid);
}

bool GOAPEnemy::isColliding(shared_ptr<Player> player) {
    return player->shape.getGlobalBounds().intersects(shape.getGlobalBounds());
}

void GOAPAgent::PerformActions(State& state, RectangleShape& shape, Vector2f playerPos, shared_ptr<Player> player, Grid& grid) {

    int totalCost1 = 0, totalCost2 = 0;

    Goal goal1 = Goal::Follow;
    Goal goal2 = Goal::Attack;

    vector<Action*> plan1 = planner.Plan(state, goal1, position);
    vector<Action*> plan2 = planner.Plan(state, goal2, position);


    for (auto action : plan1) {
        totalCost1 += action->gettotalCost();
        if (state.getlowHealth()) {
            totalCost1 += 2;// moins enclin a follow si bas en vie
        }
        if (state.getplayerInRange()) {
            totalCost1 += 2; // si peut attaquer, préfère attaquer mais ne plus follow
        }
        if (state.getplayerInSight()) {
            totalCost1 -= 5; // envie (frénétique) de follow à vue
        }
    }
    for (auto action : plan2) {
        totalCost2 += action->gettotalCost();
        if (state.getlowHealth()) {
            totalCost2 += 2; // moins enclin à attaquer si bas en vie
        }
        else if (!state.getlowHealth() and state.getplayerInRange()) {
            totalCost2 -= 50; // si pas bas en vie et an range, volonté, d'attaquer, à 100%
        }
    }
    if (totalCost1 < totalCost2) {

        for (auto action : plan1) {
            if (action->canExecute(state)) {

                action->execute(state, shape, playerPos, player, grid);  // Exécute l'action
                position = action->position;
            }
            else {
                cout << "Action impossible : " << typeid(*action).name() << "\n";
            }
            delete action;  // Libérer la mémoire
        }
    }
    if (totalCost2 < totalCost1) {
        for (auto action : plan2) {
            if (action->canExecute(state)) {
                action->execute(state, shape, playerPos, player, grid);  // Exécute l'action
            }
            else {
                cout << "Action impossible : " << typeid(*action).name() << "\n";
            }
            delete action;  // Libérer la mémoire
        }
    }
}

bool GOAPEnemy::detectPlayer(shared_ptr<Player> player) {
    bool isColliding;
    if (sightRadiusCircle.getGlobalBounds().intersects(player->shape.getGlobalBounds())) { isColliding = true; shape.setFillColor(Color::Green); }
    else { isColliding = false; shape.setFillColor(Color::Green); }
    return (isColliding);
}

bool GOAPEnemy::detectRangePlayer(shared_ptr<Player> player) {
    bool isColliding;
    if (rangeRadiusCircle.getGlobalBounds().intersects(player->shape.getGlobalBounds())) { isColliding = true; }
    else { isColliding = false; }
    return isColliding;
}

void GOAPEnemy::patrol() {
    position = shape.getPosition();
    static int currentWaypoint = 0;
    Vector2f target = waypoints[currentWaypoint];
    Vector2f direction = target - position;
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);


    if (Vector2i(shape.getPosition().x, shape.getPosition().y) == Vector2i(11 * 40, 7 * 40) and !reversed) {
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
        position += direction * 150.0f * 0.016f;
    }
    shape.move(position);
}

void GOAPEnemy::follow(Vector2f playerPos) {
    position = shape.getPosition();
    Vector2f direction = playerPos - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (distance > 0) {
        direction /= distance;
        position += direction * 150.0f * 0.016f;
    }

    shape.move(direction);
}

void GOAPEnemy::attack() { // attaquer donne des hp a l'ennemi et change la couleur en fonction de ses hp;;
    if (attackClock.getElapsedTime().asSeconds() > 1) {
        state.increaseHp();
        attackClock.restart();
    }
}
void GOAPEnemy::flee(Vector2f playerPos) {
}
void GOAPEnemy::color(int i) {
    switch (i) {
    case 0: shape.setOutlineColor(Color::Black); break;
    case 3: shape.setOutlineColor(Color::Cyan); break;
    case 5: shape.setOutlineColor(Color::White); break;
    case 8: shape.setOutlineColor(Color::Yellow);;
    case 10: shape.setOutlineColor(Color::Red); break;
    case 15: shape.setOutlineColor(Color::Magenta); break;
    }
}

bool isPathClear(const Vector2i& start, const Vector2i& end, Grid& grid)
{

    int x1 = start.x;
    int y1 = start.y;
    int x2 = end.x;
    int y2 = end.y;

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        // Vérifiez si la cellule est un mur
        if (!grid.getCell(x1, y1).Ewalkable) {
            return false;
        }

        // Si nous avons atteint la fin du chemin
        if (x1 == x2 && y1 == y2) {
            break;
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }

    return true;
}

void GOAPEnemy::enemyMove(Vector2f direction, Grid& grid)
{

}