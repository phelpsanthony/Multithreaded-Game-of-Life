//
// Created by Anthony Phelps on 2025-09-27.
//

#include "Cell.h"

Cell::Cell(int species_id, bool alive) {
    this->species_id = species_id;
    this->alive = alive;
}

Cell::Cell(const Cell &obj) {
    this->species_id = obj.species_id;
    this->alive = obj.alive;
}

Cell::~Cell() {
    // stuff
}

int Cell::getSpeciesId() const {
    return species_id;
}

bool Cell::getAlive() const {
    return alive;
}

void Cell::setAlive(bool alive) {
    this->alive = alive;
}

void Cell::setSpeciesId(int species_id) {
    this->species_id = species_id;
}