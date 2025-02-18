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

	auto vueCondition = make_unique<ConditionNode>(blackboard, "vue", 3000);
	auto vueSequence = make_unique<SequenceNode>();
	vueSequence->AddChild(move(vueCondition));
	vueSequence->AddChild(move(chaseSelect));

	root->AddChild(move(vueSequence));
	root->AddChild(move(patrolAction));

	pathfinding = make_unique<Pathfinding>(grid);
	hp = 100;
	player = pl;
}

void EnemyBT::update(float deltaTime, Grid& grid) 
{
	
	blackboard.SetValue("hp", -hp);
	blackboard.SetValue("vue", length(Vector2f(shape.getPosition().x - player->shape.getPosition().x, shape.getPosition().y - player->shape.getPosition().y)));
	
	
	int action = 0;
	root->execute(action);

	switch (action)
	{
	case 1:
		patrol(deltaTime);
		break;
	case 2:
		chase(deltaTime, grid);
		break;
	case 3:
		flee(deltaTime, grid);
		break;
	}
}

void EnemyBT::patrol(float deltaTime)
{
	Vector2f direction = Vector2f(shape.getPosition().x - patrolPath[currentPathIndex].x, shape.getPosition().y - patrolPath[currentPathIndex].y);
	direction = direction / length(direction);
	velocity = direction * SPEED;
	shape.move(velocity * deltaTime);
}

void EnemyBT::chase(float deltaTime, Grid& grid)
{
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			if (grid.getCell(j, i).walkable == false)
			{
				grid.getCell(j, i).shape.setFillColor(sf::Color::White);
			}
			else
			{
				grid.getCell(j, i).shape.setFillColor(sf::Color::Black);
			}
		}
	}

	for (auto cell : path)
	{
		grid.getCell(cell.x, cell.y).shape.setFillColor(sf::Color::Green);
	}
	
	
	if (position != Vector2i(shape.getPosition().x / 40, shape.getPosition().y / 40))
	{
		cout << "next\n";
		position = Vector2i(shape.getPosition().x / 40, shape.getPosition().y / 40);

		next = false;
	}
	
	if (static_cast<int>(shape.getPosition().x) % 40 < 25 && static_cast<int>(shape.getPosition().x) % 40 > 15 && static_cast<int>(shape.getPosition().y) % 40 < 25 && static_cast<int>(shape.getPosition().y) % 40 > 15 && !next)
	{
		cout << "centered\n";
		next = true;
		position = Vector2i(shape.getPosition().x / 40, shape.getPosition().y / 40);
	}

	if ((static_cast<int>(shape.getPosition().x) % 40 >= 25 || static_cast<int>(shape.getPosition().x) % 40 <= 15 || static_cast<int>(shape.getPosition().y) % 40 >= 25 || static_cast<int>(shape.getPosition().y) % 40 <= 15) && !next)
	{
		cout << "centering\n";
		Vector2f direction = Vector2f(position.x * 40 + 20, position.y * 40 + 20) - shape.getPosition();
		direction = Vector2f(direction.x / length(direction), direction.y / length(direction));
		velocity = Vector2f(direction.x * SPEED, direction.y * SPEED);
		shape.move(Vector2f(velocity.x * deltaTime, velocity.y * deltaTime));
	}
	else if (next)
	{
		cout << "finding path\n";
		path = pathfinding->findPath(grid, Vector2i(shape.getPosition().x / 40, shape.getPosition().y / 40), Vector2i(player->shape.getPosition().x / 40, player->shape.getPosition().y / 40));

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

void EnemyBT::flee(float deltaTime, Grid& grid)
{

}