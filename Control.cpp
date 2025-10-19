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
    long long comp_sum = 0;
    long long loop_sum = 0;
    int frame_count = 0;
    const int avg_interval = 100; // print averages every 100 frames

    // At this point the initialization is finished and the control thread can enter it's main loop
    while (true) {
        //Start timer to be able to measure performance
        auto loop_start = high_resolution_clock::now(); // start timing

        // Change ready flag to true so the worker threads can se that they can proceed
        {
            lock_guard<mutex> lock(mtx);
            ready = true;
        }
        cv.notify_all();    // Notify all threads that we are ready

        // Should wait here for all the worker threads to finish computing next states for their species
        sync.arrive_and_wait();

        auto comp_end = high_resolution_clock::now();
        auto computation_time = duration_cast<milliseconds>(comp_end - loop_start).count();


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
        frame_cv.notify_all();

        // End total loop timer
        auto loop_end = high_resolution_clock::now();
        auto loop_time = duration_cast<milliseconds>(loop_end - loop_start).count();

        // Accumulate for averaging
        comp_sum += computation_time;
        loop_sum += loop_time;
        frame_count++;

        if (frame_count % avg_interval == 0) {
            std::cout << "Average of last " << avg_interval << " frames"
                      << " | computation: " << (comp_sum / avg_interval) << "ms"
                      << " | total loop: " << (loop_sum / avg_interval) << "ms"
                      << std::endl;
            comp_sum = 0;
            loop_sum = 0;
        }


        // Pause main thread for (1/30) seconds
        // this_thread::sleep_for(33ms);
    }

}

Grid *Control::getCurrentGrid() {
    return current_grid;
}
