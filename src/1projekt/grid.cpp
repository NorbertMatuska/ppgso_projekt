#include "Grid.h"
#include <stdexcept>
#include "ppgso.h"

Grid::Grid(int rows, int cols, float cellSize, const glm::vec3& origin)
        : rows(rows), cols(cols), cellSize(cellSize), origin(origin) {
    cells.resize(rows, std::vector<GridCell>(cols));
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cells[r][c].worldPosition = origin + glm::vec3(c * cellSize, 0.0f, r * cellSize);
        }
    }
}

glm::vec3 Grid::getCellPosition(int row, int col) const {
    if (!isWithinBounds(row, col)) {
        throw std::out_of_range("Grid cell out of bounds");
    }
    return origin + glm::vec3(col * cellSize, 0.0f, row * cellSize);
}

bool Grid::isCellOccupied(int row, int col) const {
    if (!isWithinBounds(row, col)) {
        throw std::out_of_range("Grid cell out of bounds");
    }
    return cells[row][col].occupied;
}

void Grid::occupyCell(int row, int col, const std::string& objectType) {
    if (!isWithinBounds(row, col)) {
        throw std::out_of_range("Grid cell out of bounds");
    }
    cells[row][col].occupied = true;
    cells[row][col].objectType = objectType;
}

void Grid::freeCell(int row, int col) {
    if (!isWithinBounds(row, col)) {
        throw std::out_of_range("Grid cell out of bounds");
    }
    cells[row][col].occupied = false;
    cells[row][col].objectType = "";
}

bool Grid::isWithinBounds(int row, int col) const {
    return row >= 0 && row < rows && col >= 0 && col < cols;
}
