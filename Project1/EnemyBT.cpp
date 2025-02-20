#include "EnemyBT.hpp"


EnemyBT::EnemyBT(float x, float y, float sightRadius, float rangeRadius, Blackboard& bl, Grid& grid, shared_ptr<Player> pl) : Enemy(x, y,sightRadius,rangeRadius), blackboard(bl)
{
	root = make_unique<SelectorNode>();
	auto patrolAction = make_unique<ActionNode>(1);

	auto chaseAction = make_unique<ActionNode>(2);
	auto hpCondition = make_unique<ConditionNode>(blackboard, "hp", -50);

	auto chaseSequence = make_unique<SequenceNode>();
	chaseSequence->AddChild(move(hpCondition));
	chaseSequence->AddChild(move(chaseAction));

	auto fleeAction = make_unique<ActionNode>(3);

	auto chaseSelect = make_unique<SelectorNode>();
	chaseSelect->AddChild(move(chaseSequence));
	chaseSelect->AddChild(move(fleeAction));

	auto vueCondition = make_unique<ConditionNode>(blackboard, "vue", 250);
	auto vueSequence = make_unique<SequenceNode>();
	vueSequence->AddChild(move(vueCondition));
	vueSequence->AddChild(move(chaseSelect));

	root->AddChild(move(vueSequence));
	root->AddChild(move(patrolAction));

	pathfinding = make_unique<Pathfinding>(grid);
	hp = 100.0f;
	player = pl;
	action = 0;


	patrolPath.push_back(Vector2f(400, 400));
	patrolPath.push_back(Vector2f(500, 400));
	patrolPath.push_back(Vector2f(500, 500));
	patrolPath.push_back(Vector2f(400, 500));
	patrolPath.push_back(Vector2f(400, 400));
}

void EnemyBT::update(float deltaTime, Grid& grid) 
{
	if (shape.getGlobalBounds().intersects(player->shape.getGlobalBounds()))
		hp -= 0.1f;


	blackboard.SetValue("hp", -hp);
	blackboard.SetValue("vue", length(Vector2f(shape.getPosition().x - player->shape.getPosition().x, shape.getPosition().y - player->shape.getPosition().y)));
	
	
	action = 0;
	root->execute(action);

	switch (action)
	{
	case 2:
		isPatroling = false;
		chase(deltaTime, grid);
		break;
	case 1:
		patrol(deltaTime, grid);
		break;
	case 3:
		isPatroling = false;
		flee(deltaTime, grid);
		break;
	}
}

void EnemyBT::patrol(float deltaTime, Grid& grid)
{
	if (hp < 50)
	{
		availablePatrolPath.clear();
		for (auto point : patrolPath)
		{
			if (!(abs(point.x - player->shape.getPosition().x) < 250 && abs(point.y - player->shape.getPosition().y) < 250))
			{
				availablePatrolPath.push_back(point);
			}
		}
	}
	else
	{
		availablePatrolPath = patrolPath;
	}
	
	if (isPatroling)
	{

		if (availablePatrolPath.size() > 0) 
		{
		
			if (length(Vector2f(availablePatrolPath[currentPathIndex].x - shape.getPosition().x, availablePatrolPath[currentPathIndex].y - shape.getPosition().y)) < 5.0f)
			{
			currentPathIndex = (currentPathIndex + 1) % availablePatrolPath.size();
		
			}

			Vector2f direction = Vector2f(availablePatrolPath[currentPathIndex].x - shape.getPosition().x, availablePatrolPath[currentPathIndex].y - shape.getPosition().y);
			direction = direction / length(direction);

			velocity = direction * SPEED;
			shape.move(velocity * deltaTime);
		}
	}
	else
	{
		if (availablePatrolPath.size() > 0) {
			if (length(Vector2f(availablePatrolPath[currentPathIndex].x - shape.getPosition().x, availablePatrolPath[currentPathIndex].y - shape.getPosition().y)) < 5.0f)
		{
			isPatroling = true;
		}
			else
			{

				if (isPathClear(Vector2i(shape.getGlobalBounds().left / 40, shape.getGlobalBounds().top / 40), Vector2i(availablePatrolPath[currentPathIndex].x / 40, availablePatrolPath[currentPathIndex].y / 40), grid) &&
					isPathClear(Vector2i((shape.getGlobalBounds().left + shape.getGlobalBounds().width) / 40, shape.getGlobalBounds().top / 40), Vector2i(availablePatrolPath[currentPathIndex].x / 40, availablePatrolPath[currentPathIndex].y / 40), grid) &&
					isPathClear(Vector2i(shape.getGlobalBounds().left / 40, (shape.getGlobalBounds().top + shape.getGlobalBounds().height) / 40), Vector2i(availablePatrolPath[currentPathIndex].x / 40, availablePatrolPath[currentPathIndex].y / 40), grid) &&
					isPathClear(Vector2i((shape.getGlobalBounds().left + shape.getGlobalBounds().width) / 40, (shape.getGlobalBounds().top + shape.getGlobalBounds().height) / 40), Vector2i(availablePatrolPath[currentPathIndex].x / 40, availablePatrolPath[currentPathIndex].y / 40), grid))
				{

					Vector2f direction = Vector2f(availablePatrolPath[currentPathIndex].x - shape.getPosition().x, availablePatrolPath[currentPathIndex].y - shape.getPosition().y);
					direction = Vector2f(direction.x / length(direction), direction.y / length(direction));
					velocity = Vector2f(direction.x * SPEED, direction.y * SPEED);
					shape.move(Vector2f(velocity.x * deltaTime, velocity.y * deltaTime));
				}
				else
				{
					if (position != Vector2i(shape.getPosition().x / 40, shape.getPosition().y / 40))
					{
						position = Vector2i(shape.getPosition().x / 40, shape.getPosition().y / 40);

						next = false;
					}

					if (static_cast<int>(shape.getPosition().x) % 40 < 25 && static_cast<int>(shape.getPosition().x) % 40 > 15 && static_cast<int>(shape.getPosition().y) % 40 < 25 && static_cast<int>(shape.getPosition().y) % 40 > 15 && !next)
					{
						next = true;
						position = Vector2i(shape.getPosition().x / 40, shape.getPosition().y / 40);
					}

					if ((static_cast<int>(shape.getPosition().x) % 40 >= 25 || static_cast<int>(shape.getPosition().x) % 40 <= 15 || static_cast<int>(shape.getPosition().y) % 40 >= 25 || static_cast<int>(shape.getPosition().y) % 40 <= 15) && !next)
					{
						Vector2f direction = Vector2f(position.x * 40 + 20, position.y * 40 + 20) - shape.getPosition();
						direction = Vector2f(direction.x / length(direction), direction.y / length(direction));
						velocity = Vector2f(direction.x * SPEED, direction.y * SPEED);
						shape.move(Vector2f(velocity.x * deltaTime, velocity.y * deltaTime));
					}
					else if (next)
					{
						patrolBackPath = pathfinding->findPath(grid, Vector2i(shape.getPosition().x / 40, shape.getPosition().y / 40), Vector2i(availablePatrolPath[currentPathIndex].x / 40, availablePatrolPath[currentPathIndex].y / 40));

						Vector2f direction;
						if (patrolBackPath.size() > 1)
						{
							direction = Vector2f(patrolBackPath[1].x - position.x, patrolBackPath[1].y - position.y);
						}
						else if (patrolBackPath.size() == 1)
						{
							direction = Vector2f(patrolBackPath[0].x - position.x, patrolBackPath[0].y - position.y);
						}
						else
						{
							direction = Vector2f(availablePatrolPath[currentPathIndex].x - position.x, availablePatrolPath[currentPathIndex].y - position.y);
						}

						direction = Vector2f(direction.x / length(direction), direction.y / length(direction));
						velocity = Vector2f(direction.x * SPEED, direction.y * SPEED);
						shape.move(Vector2f(velocity.x * deltaTime, velocity.y * deltaTime));
					}
				}
			}
		}
	}
}

void EnemyBT::chase(float deltaTime, Grid& grid)
{
	if (isPathClear(Vector2i(shape.getGlobalBounds().left / 40, shape.getGlobalBounds().top / 40), Vector2i(player->shape.getGlobalBounds().left / 40, player->shape.getGlobalBounds().top / 40), grid) &&
		isPathClear(Vector2i((shape.getGlobalBounds().left + shape.getGlobalBounds().width) / 40, shape.getGlobalBounds().top / 40), Vector2i((player->shape.getGlobalBounds().left + player->shape.getGlobalBounds().width) / 40, player->shape.getGlobalBounds().top / 40), grid) &&
		isPathClear(Vector2i(shape.getGlobalBounds().left / 40, (shape.getGlobalBounds().top + shape.getGlobalBounds().height) / 40), Vector2i(player->shape.getGlobalBounds().left / 40, (player->shape.getGlobalBounds().top + player->shape.getGlobalBounds().height) / 40), grid) &&
		isPathClear(Vector2i((shape.getGlobalBounds().left + shape.getGlobalBounds().width) / 40, (shape.getGlobalBounds().top + shape.getGlobalBounds().height) / 40), Vector2i((player->shape.getGlobalBounds().left + player->shape.getGlobalBounds().width) / 40, (player->shape.getGlobalBounds().top + player->shape.getGlobalBounds().height) / 40), grid))
	{
		
		Vector2f direction = Vector2f(player->shape.getPosition().x - shape.getPosition().x, player->shape.getPosition().y - shape.getPosition().y);
		direction = Vector2f(direction.x / length(direction), direction.y / length(direction));
		velocity = Vector2f(direction.x * SPEED, direction.y * SPEED);
		shape.move(Vector2f(velocity.x * deltaTime, velocity.y * deltaTime));
	}
	else
	{
		path = pathfinding->findPath(grid, Vector2i(shape.getPosition().x / 40, shape.getPosition().y / 40), Vector2i(player->shape.getPosition().x / 40, player->shape.getPosition().y / 40));
		if (path.size() == 0)
		{
			action = 1;
			return;
		}
		if (position != Vector2i(shape.getPosition().x / 40, shape.getPosition().y / 40))
		{
			position = Vector2i(shape.getPosition().x / 40, shape.getPosition().y / 40);

			next = false;
		}

		if (static_cast<int>(shape.getPosition().x) % 40 < 25 && static_cast<int>(shape.getPosition().x) % 40 > 15 && static_cast<int>(shape.getPosition().y) % 40 < 25 && static_cast<int>(shape.getPosition().y) % 40 > 15 && !next)
		{
			next = true;
			position = Vector2i(shape.getPosition().x / 40, shape.getPosition().y / 40);
		}

		if ((static_cast<int>(shape.getPosition().x) % 40 >= 25 || static_cast<int>(shape.getPosition().x) % 40 <= 15 || static_cast<int>(shape.getPosition().y) % 40 >= 25 || static_cast<int>(shape.getPosition().y) % 40 <= 15) && !next)
		{
			Vector2f direction = Vector2f(position.x * 40 + 20, position.y * 40 + 20) - shape.getPosition();
			direction = Vector2f(direction.x / length(direction), direction.y / length(direction));
			velocity = Vector2f(direction.x * SPEED, direction.y * SPEED);
			shape.move(Vector2f(velocity.x * deltaTime, velocity.y * deltaTime));
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
			shape.move(Vector2f(velocity.x * deltaTime, velocity.y * deltaTime));
		}
	}
}

void EnemyBT::flee(float deltaTime, Grid& grid)
{
	Vector2f direction = Vector2f(shape.getPosition().x - player->shape.getPosition().x, shape.getPosition().y - player->shape.getPosition().y);
	direction = Vector2f(direction.x / length(direction), direction.y / length(direction));
	velocity = direction * SPEED * deltaTime;
	
	enemyMove(velocity, grid);
}

bool EnemyBT::isPathClear(const Vector2i& start, const Vector2i& end, Grid& grid) {

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
		if (x1 >= 0 && x1 < GRID_WIDTH && y1 >= 0 && y1 < GRID_HEIGHT) {

			if (!grid.getCell(x1, y1).Ewalkable) {
				return false;
			}
		}
		else return false;

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

void EnemyBT::enemyMove(Vector2f direction, Grid& grid)
{
	auto isWalkable = [&](float x, float y) {
		int gridX = static_cast<int>(x / CELL_SIZE);
		int gridY = static_cast<int>(y / CELL_SIZE);
		return gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT && grid.getCell(gridX, gridY).Ewalkable;
		};

	Vector2f newPosition = Vector2f(shape.getGlobalBounds().left, shape.getGlobalBounds().top) + direction;
	FloatRect newBounds(newPosition, shape.getSize());

	if (direction.x > 0)
	{
		if (isWalkable(newBounds.left + newBounds.width + 2, shape.getGlobalBounds().top) &&
			isWalkable(newBounds.left + newBounds.width + 2, shape.getGlobalBounds().top + shape.getGlobalBounds().height - 1))
		{
			shape.move(Vector2f(direction.x, 0));
		}
		else
		{
			cout << "obstacle" << endl;
		}
	}
	else
	{
		if (isWalkable(newBounds.left - 2, shape.getGlobalBounds().top) &&
			isWalkable(newBounds.left - 2, shape.getGlobalBounds().top + shape.getGlobalBounds().height - 1))
		{
			shape.move(Vector2f(direction.x, 0));
		}
		else
		{
			cout << "obstacle" << endl;
		}
	}

	if (direction.y > 0)
	{
		if (isWalkable(shape.getGlobalBounds().left, newBounds.top + newBounds.height + 2) &&
			isWalkable(shape.getGlobalBounds().left + shape.getGlobalBounds().width, newBounds.top + newBounds.height + 2))
		{
			shape.move(Vector2f(0, direction.y));
		}
	}
	else
	{
		if (isWalkable(shape.getGlobalBounds().left, newBounds.top - 2) &&
			isWalkable(shape.getGlobalBounds().left + shape.getGlobalBounds().width - 1, newBounds.top - 2))
		{
			shape.move(Vector2f(0, direction.y));
		}
	}
}