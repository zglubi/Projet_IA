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
const int WINDOW_HEIGHT = 600;

int main() {
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Jeu SFML - IA Ennemis");
    window.setFramerateLimit(60);

    Blackboard blackboard;
    Grid grid;

    shared_ptr<Player> player = make_shared<Player>(200, 400);
    vector<shared_ptr<EnemyBT>> enemies = {
        make_shared<EnemyBT>(400, 400, blackboard, grid, player)/*,
        make_shared<EnemyBT>(700, 100, blackboard, grid, player)*/
    };
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
        /*for (auto& GOAPenemy : GOAPenemies) {
            GOAPenemy.update(deltaTime, grid,player);
        }*/
        
        window.clear();
        grid.draw(window);
        window.draw(player->shape);
        for (const auto& enemy : enemies)
            window.draw(enemy->shape);
        window.display();
    }
    return 0;
}
