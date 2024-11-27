#include "Grid.h"
#include <stdexcept>
#include "ppgso.h"

// Constructor
Grid::Grid(int rows, int cols, float cellSize, const glm::vec3& origin)
        : rows(rows), cols(cols), cellSize(cellSize), origin(origin) {
    // Initialize the grid
    cells.resize(rows, std::vector<GridCell>(cols));
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cells[r][c].worldPosition = origin + glm::vec3(c * cellSize, 0.0f, r * cellSize);
        }
    }
}

// Get the world position of a cell
glm::vec3 Grid::getCellPosition(int row, int col) const {
    if (!isWithinBounds(row, col)) {
        throw std::out_of_range("Grid cell out of bounds");
    }
    // Ensure Y remains constant (e.g., height is 0), and Z is used for the forward direction.
    return origin + glm::vec3(col * cellSize, 0.0f, row * cellSize);
}

// Check if a cell is occupied
bool Grid::isCellOccupied(int row, int col) const {
    if (!isWithinBounds(row, col)) {
        throw std::out_of_range("Grid cell out of bounds");
    }
    return cells[row][col].occupied;
}

// Occupy a cell with an object
void Grid::occupyCell(int row, int col, const std::string& objectType) {
    if (!isWithinBounds(row, col)) {
        throw std::out_of_range("Grid cell out of bounds");
    }
    cells[row][col].occupied = true;
    cells[row][col].objectType = objectType;
}

// Free a cell
void Grid::freeCell(int row, int col) {
    if (!isWithinBounds(row, col)) {
        throw std::out_of_range("Grid cell out of bounds");
    }
    cells[row][col].occupied = false;
    cells[row][col].objectType = "";
}

// Check if the cell is within bounds
bool Grid::isWithinBounds(int row, int col) const {
    return row >= 0 && row < rows && col >= 0 && col < cols;
}
