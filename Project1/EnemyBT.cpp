#include "EnemyBT.hpp"


EnemyBT::EnemyBT(float x, float y) : Enemy(x, y)
{
	root = make_unique<SelectorNode>();
	auto chaseAction = make_unique<ActionNode>(1);
	root->AddChild(move(chaseAction));
}
