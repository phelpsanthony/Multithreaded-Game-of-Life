//
// Created by Anthony Phelps on 2025-09-27.
//

#ifndef COMP426A1_CELL_H
#define COMP426A1_CELL_H


class Cell {
    private:
    int species_id;
    bool alive;

    public:
    Cell(int, bool);    // constructor
    Cell(const Cell&); // copy constructor
    ~Cell();            // destructor

    int getSpeciesId() const;
    bool getAlive() const;

    void setAlive(bool);
    void setSpeciesId(int);

};


#endif //COMP426A1_CELL_H