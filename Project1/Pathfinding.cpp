#include "Pathfinding.hpp"
#include <queue>
#include <algorithm>

Pathfinding::Pathfinding(Grid& grid) 
{
    gridB = grid.getWalkable();
}

std::vector<sf::Vector2i> Pathfinding::findPath(Grid& grid, sf::Vector2i start, sf::Vector2i end) {
    std::vector<std::vector<bool>> visited(GRID_HEIGHT, std::vector<bool>(GRID_WIDTH, false));
    std::vector<Node*> openList;
    std::vector<Node*> allNodes;

    Node* startNode = new Node(start);
    Node* endNode = new Node(end);
    openList.push_back(startNode);
    allNodes.push_back(startNode);

    while (!openList.empty()) {
        std::sort(openList.begin(), openList.end(), [](Node* a, Node* b) { return a->fCost < b->fCost; });
        Node* current = openList.front();
        openList.erase(openList.begin());

        if (current->position == end) {
            std::vector<sf::Vector2i> path;
            while (current) {
                path.push_back(current->position);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        visited[current->position.y][current->position.x] = true;

        std::vector<sf::Vector2i> neighbors = {
            {current->position.x + 1, current->position.y},
            {current->position.x - 1, current->position.y},
            {current->position.x, current->position.y + 1},
            {current->position.x, current->position.y - 1}
        };

        for (sf::Vector2i& neighborPos : neighbors) {
            if (neighborPos.x < 0 || neighborPos.x >= GRID_WIDTH || neighborPos.y < 0 || neighborPos.y >= GRID_HEIGHT)
                continue;
            if (grid.grid[neighborPos.y][neighborPos.x] == 1 || visited[neighborPos.y][neighborPos.x])
                continue;

            Node* neighbor = new Node(neighborPos);
            neighbor->parent = current;
            neighbor->calculateCosts(endNode, current->gCost + 1);
            openList.push_back(neighbor);
            allNodes.push_back(neighbor);
        }
    }

    for (Node* node : allNodes)
        delete node;

    return {};
}