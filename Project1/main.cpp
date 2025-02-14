#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Grid.hpp"
#include "GOAP.h"
#include "GOAPEnemy.h"
#include "BT.hpp"
#include "EnemyBT.hpp"
#include <vector>

using namespace sf;
using namespace std;


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Jeu SFML - IA Ennemis");
    window.setFramerateLimit(60);

    Player player(200, 400);
    std::vector<Enemy> enemies = { Enemy(100, 100), Enemy(700, 100) };
    Grid grid;
    grid.loadFromFile("map.txt");

    sf::Clock clock;

    GOAPAgent agent;

    cout << "Etat initial de l'agent:\n\n";
    agent.PrintState();
    

    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.update(deltaTime, grid);
        for (auto& enemy : enemies) {
            enemy.update(deltaTime, grid);
        }

        window.clear();
        grid.draw(window);
        window.draw(player.shape);
        for (const auto& enemy : enemies)
            window.draw(enemy.shape);
        window.display();


        //cout << "\nL'agent commence ses actions...\n\n";
        //agent.PerformActions();

        //cout << "\nEtat de l'agent apres avoir effectue les actions:\n\n";
        //agent.PrintState();
    }
    return 0;
}