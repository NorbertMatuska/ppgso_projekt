#ifndef GRID_H
#define GRID_H

#include <vector>
#include <glm/vec3.hpp>
#include <string>

struct GridCell {
    glm::vec3 worldPosition;
    bool occupied = false;
    std::string objectType;
};

class Grid {
private:
    int rows, cols;
    float cellSize;
    glm::vec3 origin;
    std::vector<std::vector<GridCell>> cells;

public:
    Grid(int rows, int cols, float cellSize, const glm::vec3& origin);

    glm::vec3 getCellPosition(int row, int col) const;
    bool isCellOccupied(int row, int col) const;
    void occupyCell(int row, int col, const std::string& objectType = "");
    void freeCell(int row, int col);
    bool isWithinBounds(int row, int col) const;
};

#endif // GRID_H
