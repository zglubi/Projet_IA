#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Enemy.hpp"
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Jeu SFML - IA Ennemis");
    window.setFramerateLimit(60);

    Player player(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    std::vector<Enemy> enemies = { Enemy(100, 100), Enemy(700, 100) };

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.update(deltaTime);
        for (auto& enemy : enemies) {
            enemy.update(deltaTime);
        }

        window.clear();
        window.draw(player.shape);
        for (const auto& enemy : enemies)
            window.draw(enemy.shape);
        window.display();
    }
    return 0;
}
