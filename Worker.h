//
// Created by Anthony Phelps on 2025-09-27.
//

#ifndef COMP426A1_WORKER_H
#define COMP426A1_WORKER_H

#include <barrier>
#include <thread>
#include <condition_variable>
#include <mutex>
#include "Grid.h"

class Worker {
    private:
    int species_id;
    Grid*& current_grid;
    Grid*& next_grid;
    thread worker;

    mutex& mtx;             // Shared with main thread for synchronization
    condition_variable& cv; // Shared with main thread for synchronization
    bool& ready;            // Shared with main thread for synchronization

    barrier<>& sync;        // Shared with main thread for synchronization

    // Private functions that shouldn't be accessed from outside this class
    void run();
    bool computeNextState(int, int);

    public:
    Worker(int, Grid*&, Grid*&, mutex&, condition_variable&, bool&, barrier<>&);  // Constructor
    ~Worker();

    void start();
    void join();

};


#endif //COMP426A1_WORKER_H