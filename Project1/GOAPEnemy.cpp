#include "GOAPEnemy.h"

GOAPEnemy::GOAPEnemy(int x, int y,float radius) : Enemy(x,y) {}

void GOAPEnemy::update(float deltaTime, Grid& grid){
	
}

bool GOAPEnemy::isColliding(Player& player)
{
	return player.shape.getGlobalBounds().intersects(shape.getGlobalBounds());
}



