#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "EnemyBT.hpp"
#include "Grid.hpp"
#include "GOAPEnemy.h"
#include "BT.hpp"
#include <vector>


using namespace sf;
using namespace std;


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

int resizeCoord(int coord) {
	return coord * 40;
}

int main() {
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Jeu SFML - IA Ennemis");
    window.setFramerateLimit(60);

    Blackboard blackboard;
    Grid grid;

    vector<Vector2f> patrolPath1,patrolPath2;
    patrolPath1.push_back(Vector2f(540, 540));
    patrolPath1.push_back(Vector2f(680, 540));
    patrolPath1.push_back(Vector2f(680, 700));
    patrolPath1.push_back(Vector2f(540, 700));
    patrolPath1.push_back(Vector2f(540, 540));

    patrolPath2.push_back(Vector2f(100, 520));
    patrolPath2.push_back(Vector2f(240, 520));
    patrolPath2.push_back(Vector2f(240, 720));
    patrolPath2.push_back(Vector2f(100, 720));
    patrolPath2.push_back(Vector2f(100, 520));

    shared_ptr<Player> player = make_shared<Player>(400, 200);
    vector<shared_ptr<EnemyBT>> enemies = {
        make_shared<EnemyBT>(520, 520,150,50, blackboard, grid, player, patrolPath1),
        make_shared<EnemyBT>(100, 600,150,50, blackboard, grid, player,patrolPath2)
    };
    vector<GOAPEnemy> GOAPenemies = { GOAPEnemy(200,100,100,50), GOAPEnemy(680,200,150,50) };
    grid.loadFromFile("map.txt");

    Clock clock;

    while (window.isOpen()) {
        Time dt = clock.restart();
        float deltaTime = dt.asSeconds();

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        player->update(deltaTime, grid);
        for (auto& enemy : enemies) {
            enemy->update(deltaTime, grid);
        }
        for (auto& GOAPenemy : GOAPenemies) {
            GOAPenemy.update(grid,player);
        }
        
        window.clear();
        grid.draw(window);
        window.draw(player->shape);
        for (const auto& enemy : enemies) {
            window.draw(enemy->shape);
        }
        
        for (const auto& enemy : GOAPenemies) {
            window.draw(enemy.shape);
        }

        window.display();
    }
    return 0;
}
