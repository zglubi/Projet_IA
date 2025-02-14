#include "grid.hpp"
#include <fstream>
#include <iostream>


Grid::Grid() {
    cells.resize(GRID_HEIGHT, std::vector<Cell>(GRID_WIDTH, { true, {0, 0}, sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE)) }));
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            cells[y][x].position = sf::Vector2f(x * CELL_SIZE, y * CELL_SIZE);
            cells[y][x].shape.setPosition(cells[y][x].position);
            cells[y][x].shape.setFillColor(sf::Color::Transparent);
            cells[y][x].shape.setOutlineThickness(1);
            cells[y][x].shape.setOutlineColor(sf::Color(50, 50, 50));
        }
    }
}

void Grid::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << std::endl;
        return;
    }

    for (int y = 0; y < GRID_HEIGHT; ++y) {
        std::string line;
        if (!std::getline(file, line)) break;
        for (int x = 0; x < GRID_WIDTH && x < line.size(); ++x) {
            cells[y][x].walkable = (line[x] == '0');
            if (!cells[y][x].walkable) {
                cells[y][x].shape.setFillColor(sf::Color::White);
            }
        }
    }
}

void Grid::draw(sf::RenderWindow& window) {
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            window.draw(cells[y][x].shape);
        }
    }
}

Cell& Grid::getCell(int x, int y) {
    return cells[y][x];
}