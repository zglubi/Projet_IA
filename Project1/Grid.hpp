#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include <string>
using namespace std;
using namespace sf;

const int GRID_WIDTH = 20;
const int GRID_HEIGHT = 20;
const int CELL_SIZE = 40;

struct Cell {
    bool walkable,Ewalkable;
    sf::Vector2f position;
    sf::RectangleShape shape;
};

class Grid {
public:
    std::vector<std::vector<int>> grid;

    Grid();
    void loadFromFile(const std::string& filename);
    void draw(sf::RenderWindow& window);
    Cell& getCell(int x, int y);
    vector<vector<bool>> getWalkable();
private:
    std::vector<std::vector<Cell>> cells;
};

#endif