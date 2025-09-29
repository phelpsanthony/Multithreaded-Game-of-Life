//
// Created by Anthony Phelps on 2025-09-27.
//

#include "Grid.h"

Grid::Grid(int width, int height) {
    this->width = width;
    this->height = height;

    // Initialize the grid with invalid species temporarily
    grid = vector<vector<Cell> >(this->height, vector<Cell>(this->width, Cell(-1, false)));
}

Grid::Grid(const Grid& grid) {
    this->width = grid.width;
    this->height = grid.height;
    this->grid = grid.grid;
}

Grid::~Grid() {
    //stuff
}

int Grid::getWidth() {
    return width;
}

int Grid::getHeight() {
    return height;
}

Cell& Grid::getCell(int row, int col) {
    return grid[row][col];
}

void Grid::setWidth(int width) {
    this->width = width;
}

void Grid::setHeight(int height) {
    this->height = height;
}

void Grid::setCell(int row, int col, int species, bool alive) {
    grid[row][col].setSpeciesId(species);
    grid[row][col].setAlive(alive);
}

vector<vector<Cell>>& Grid::getGrid() {
    return grid;
}
