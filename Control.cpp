//
// Created by Anthony Phelps on 2025-09-28.
//

#include "Control.h"
#include <chrono>
using namespace chrono;

Control::Control(int height, int width, int num_of_species, OpenGLHandler* opengl_handler, mutex* m, condition_variable* c,
    bool* frame_ready)
:height(height), width(width), num_of_species(num_of_species), opengl_handler(opengl_handler),
frame_mutex(*m), frame_cv(*c), frame_ready(*frame_ready),
sync(num_of_species+1), ready(false)
{
    current_grid = new Grid(width, height);
    next_grid = new Grid(width, height);

    // Make num_of_species amount of worker threads and store them in the worker_threads vector
    for (int i = 0; i < num_of_species; i++) {
        worker_threads.push_back(
            make_unique<Worker>(i, current_grid, next_grid, mtx, cv, ready, sync));
    }
}

Control::~Control() {
    delete current_grid;
    delete next_grid;
}

// Start Control thread
void Control::start() {
    // Start all worker threads first
    for (auto& w : worker_threads) {
        w->start(); // launches the worker thread
    }

    control_thread = thread(&Control::run, this);
}

void Control::join() {
    if (control_thread.joinable()) {
        control_thread.join();
    }
}

// The function that the Control Thread executes
void Control::run() {

    // Set up random number generation for random initialization at the beginning
    random_device rd;
    default_random_engine generator(rd());
    uniform_int_distribution<int> species_id_generator(0, num_of_species-1);

    // Use random number generators to randomly initialize the current grid
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            int species_id = species_id_generator(generator);
            current_grid->setCell(i, j, species_id, true);          // All cells should be alive at init
        }
    }

    // variables to calculate the average performance
    int total=0;
    int counter=0;
    float average=0;

    // At this point the initialization is finished and the control thread can enter it's main loop
    while (true) {
        //Start timer to be able to measure performance
        auto start = high_resolution_clock::now(); // start timing

        // Change ready flag to true so the worker threads can se that they can proceed
        {
            lock_guard<mutex> lock(mtx);
            ready = true;
        }
        cv.notify_all();    // Notify all threads that we are ready

        // Should wait here for all the worker threads to finish computing next states for their species
        sync.arrive_and_wait();

        // Make the ready flag false again because all the workers shouldn't continue
        {
            lock_guard<mutex> lock(mtx);
            ready = false;
        }

        // The next_grid holds the next states of all the cells, swap the pointers
        // This swap should be visible in the worker class as well because I passed the pointers by reference
        // in the Worker constructor. Hopefully it works
        swap(current_grid, next_grid);

        // signal to main.cpp that the next frame is ready because apparently OpenGL needs to be on the main thread
        {
            lock_guard<mutex> lock(frame_mutex);
            frame_ready = true;
        }
        frame_cv.notify_one();

        // Finish timing the iteration for performance measurement
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start).count();
        total += duration;
        counter++;
        average = total/counter;

        if (counter == 100) {
            cout << "Average Iteration  after the first 100 iterations took " << average << " ms" << endl;
        }


        // Pause main thread for (1/30) seconds
        this_thread::sleep_for(33ms);
    }

}

Grid *Control::getCurrentGrid() {
    return current_grid;
}
