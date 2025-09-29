//
// Created by Anthony Phelps on 2025-09-27.
//

#include "Worker.h"

Worker::Worker(int sid, Grid*& curr, Grid*& next,
               mutex& m, condition_variable& cv,
               bool& rdy, barrier<>& b)
    : species_id(sid),
      current_grid(curr),
      next_grid(next),
      mtx(m),
      cv(cv),
      ready(rdy),
      sync(b)
{ }

Worker::~Worker() {
    if (worker.joinable()) {
        worker.join();
    }
}

// Public function that allows starting of threads
void Worker::start() {
    worker = thread(&Worker::run, this);
}

void Worker::join() {
    if (worker.joinable()) {
        worker.join();
    }
}

/*
 * This function needs to check the neighboring cells for living members of the same species while making
 * sure that the neighboring cells are valid positions within the grid. If the position is valid, and the
 * neighboring cell is the same species as the thread's species and it's alive then the living_neighbors
 * counter is incremented. The counter is then used to return the next state of the cell after applying the
 * game rules.
 */
bool Worker::computeNextState(int row, int col) {

    int living_neighbors = 0;
    bool alive = current_grid->getCell(row, col).getAlive();

    // Check the all the surrounding cells for valid position in the grid, species type and life
    for (int i=-1; i<=1; i++)           // Check the neighboring rows
        for (int j=-1; j<=1; j++) {     // Check the neighboring cols

            // If it's the cell itself skip
            if (i==0 && j==0) continue;

            // Calculate neighboring rows and cols
            int neighbor_row = row + i;
            int neighbor_col = col + j;

            bool valid_row = (neighbor_row >=0 && neighbor_row < current_grid->getHeight());
            bool valid_col = (neighbor_col >= 0 && neighbor_col < current_grid->getWidth());

            /* If rows and cols are valid and the cell is the same species as the thread's species and it's alive
             * increment counter
             */
            if (valid_row && valid_col &&
                species_id == current_grid->getCell(neighbor_row, neighbor_col).getSpeciesId()
                && current_grid->getCell(neighbor_row, neighbor_col).getAlive())
            {
                living_neighbors++;
            }
        }

    // Apply the rules of the game
    if (alive) {
        return (living_neighbors == 2 || living_neighbors == 3);
    }
    return (living_neighbors == 3);
}

// this is what each thread should execute during the execution of the program
void Worker::run() {
    while (true) {

        // Wait to be triggered by the main thread using a condition variable
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&] {return ready;});
        lock.unlock();

        // Iterate over the current grid to compute the next state of each cell for the species of the thread
        for (int row=0; row < current_grid->getHeight(); row++) {
            for (int col=0; col < current_grid->getWidth(); col++) {

                Cell& cell = current_grid->getCell(row, col);

                if (cell.getSpeciesId() != species_id) continue;

                bool next_alive = computeNextState(row, col);
                next_grid->setCell(row, col, species_id, next_alive);

            }
        }

        sync.arrive_and_wait();
    }
}