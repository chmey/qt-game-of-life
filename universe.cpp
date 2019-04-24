#include "universe.h"

Universe::Universe()
{
    Universe(16,16);
}

Universe::Universe(int num_row, int num_col) {
    this->columnsPerRow = num_col;
    this->numRows = num_row;
    this->map.resize(num_row * num_col);
    std::fill(this->map.begin(), this->map.end(), false);
}

bool Universe::getState(int row, int col) {
    return this->map.at(row * this->columnsPerRow + col);
}

void Universe::setState(int row, int col, bool val) {
    this->map.at(row * this->columnsPerRow + col) = val;
}
void Universe::setState(int row, int col, bool val, int len) {
    for (int i = 0; i < len; ++i) {
        this->setState(row, col+i, val);
    }
}

int Universe::getCols() {
    return this->columnsPerRow;
}
int Universe::getRows() {
    return this->numRows;
}


