#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "Grid.hpp"
#include "Node.hpp"
#include <vector>

class Pathfinding {
public:
	Pathfinding(Grid& grid);
    vector<Vector2i> findPath(Grid& grid, Vector2i start, Vector2i end);
private:
    vector<vector<bool>> gridB;
};

#endif