#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "Grid.hpp"
#include "Node.hpp"
#include <vector>

class Pathfinding {
public:
	Pathfinding(Grid& grid);
    static std::vector<sf::Vector2i> findPath(Grid& grid, sf::Vector2i start, sf::Vector2i end);
private:
    vector<vector<bool>> gridB;
};

#endif