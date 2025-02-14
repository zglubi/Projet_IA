#include "GOAPEnemy.h"

GOAPEnemy::GOAPEnemy(int x, int y, float radius) : detectionRadius(radius), Enemy(x, y) { shape.setOutlineColor(Color::Yellow); shape.setOutlineThickness(2); }

void GOAPEnemy::update(float deltaTime, Grid& grid, Player& player){
	if (player.shape.getGlobalBounds().intersects(shape.getGlobalBounds())) {
		if (damageClock.getElapsedTime().asSeconds() > 1) {
			state.decreaseHp(); // si rien dans () alors decreaseHP(1);;
			damageClock.restart();
			cout << state.getHp();
		}
	}
}

bool GOAPEnemy::isColliding(Player& player) {
	return player.shape.getGlobalBounds().intersects(shape.getGlobalBounds());
}