//
// Created by Anthony Phelps on 2025-09-27.
//

#ifndef COMP426A1_GRID_H
#define COMP426A1_GRID_H

using namespace std;
#include <vector>
#include "Cell.h"


class Grid {
    private:
    int width;
    int height;
    vector<vector<Cell>> grid;

    public:
    Grid(int, int);
    Grid(const Grid&);
    ~Grid();

    int getWidth();
    int getHeight();
    Cell& getCell(int, int);
    vector<vector<Cell>>& getGrid();

    void setWidth(int);
    void setHeight(int);
    void setCell(int, int, int, bool);

};


#endif //COMP426A1_GRID_H