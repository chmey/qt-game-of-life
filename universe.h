#ifndef UNIVERSE_H
#define UNIVERSE_H
#include <vector>

class Universe
{
public:
    Universe();
    Universe(int num_row, int num_col);
    int getCols(); // Return number of cells per row
    int getRows();
    bool getState(int row, int col); // Return state of cell e.g alive | dead
    void setState(int row, int col, bool val); // Set the state of a cell to val
    void setState(int row, int col, bool val, int len); // Set the state of len cells to val

    using iterator = std::vector<bool>::iterator;
    iterator begin() { return this->map.begin(); }
    iterator end() { return this->map.end(); }

private:
   int columnsPerRow; // Number of cells per row
   int numRows; // Number of Rows
   std::vector<bool> map;
};

#endif // UNIVERSE_H
