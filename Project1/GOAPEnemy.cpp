#include "GOAPEnemy.h"

GOAPEnemy::GOAPEnemy(int x, int y, float radius) : detectionRadius(radius), Enemy(x, y) { shape.setOutlineColor(Color::Yellow); shape.setOutlineThickness(2); }

void GOAPEnemy::update(float deltaTime, Grid& grid){
	
}

bool GOAPEnemy::isColliding(Player& player) {
	return player.shape.getGlobalBounds().intersects(shape.getGlobalBounds());
}