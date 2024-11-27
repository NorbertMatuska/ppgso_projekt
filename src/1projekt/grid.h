#ifndef GRID_H
#define GRID_H

#include <vector>
#include <glm/vec3.hpp>
#include <string>

struct GridCell {
    glm::vec3 worldPosition;
    bool occupied = false;        // Whether the cell is occupied
    std::string objectType = "";  // Optional: Type of object in the cell
};

class Grid {
private:
    int rows, cols;             // Grid dimensions
    float cellSize;             // Size of each cell in world units
    glm::vec3 origin;           // Bottom-left corner of the grid in world space
    std::vector<std::vector<GridCell>> cells;  // 2D vector to store the grid cells

public:
    // Constructor
    Grid(int rows, int cols, float cellSize, const glm::vec3& origin);

    // Get the world position of a cell
    glm::vec3 getCellPosition(int row, int col) const;

    // Check if a cell is occupied
    bool isCellOccupied(int row, int col) const;

    // Occupy a cell with an object
    void occupyCell(int row, int col, const std::string& objectType = "");

    // Free a cell
    void freeCell(int row, int col);

    // Check grid bounds
    bool isWithinBounds(int row, int col) const;
};

#endif // GRID_H
