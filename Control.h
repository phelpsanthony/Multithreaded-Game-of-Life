//
// Created by Anthony Phelps on 2025-09-28.
//

#ifndef COMP426A1_CONTROL_H
#define COMP426A1_CONTROL_H

#include <random>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <barrier>
#include <vector>
#include "Grid.h"
#include "Worker.h"
#include "OpenGLHandler.h"
#include <random>
#include <iostream>

using namespace std;

class Control {
    private:
    int height;
    int width;
    int num_of_species;

    Grid *current_grid;
    Grid *next_grid;

    thread control_thread;

    // Synchronization with main.cpp because on MacOs FREEGLUT function can only be called in main which
    // would have been nice to know before I made this whole program
    mutex& frame_mutex;
    condition_variable& frame_cv;
    bool& frame_ready;

    // Synchronization with worker threads
    mutex mtx;
    condition_variable cv;
    bool ready;
    barrier<> sync;

    vector<unique_ptr<Worker>> worker_threads;

    OpenGLHandler* opengl_handler;

    void run(); // Main loop for game of life

    public:
    Control(int, int, int, OpenGLHandler*, mutex*, condition_variable*, bool*);
    ~Control();
    void start();
    void join();
    Grid* getCurrentGrid();
};


#endif //COMP426A1_CONTROL_H