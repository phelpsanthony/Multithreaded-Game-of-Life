#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <OpenGL/glu.h>
#include <vector>
#include "Grid.h"
#include "Cell.h"
#include "Worker.h"
#include "Control.h"

using namespace std;

// Window size
const int WIDTH = 1024;
const int HEIGHT = 768;

mutex frame_mutex;
condition_variable frame_cv;
bool frame_ready = false;
Control *game_of_life_ptr = nullptr;
OpenGLHandler *opengl_handler_ptr = nullptr;

// GLUT display callback
void display() {
    unique_lock<mutex> lock(frame_mutex);
    if (!frame_ready) return;   // Only draw if a new frame is ready

    opengl_handler_ptr->drawFrame(game_of_life_ptr->getCurrentGrid()->getGrid());
    frame_ready = false;        // Reset flag
}

// GLUT idle callback
void idle() {
    unique_lock<mutex> lock(frame_mutex);
    frame_cv.wait(lock, [](){ return frame_ready; });  // Wait until Control thread signals
    glutPostRedisplay();       // Ask GLUT to call display()
}

int main(int argc, char** argv) {
    // Initialize OpenGLHandler
    OpenGLHandler opengl_handler;
    opengl_handler.initWindow(argc, argv, "Game of Life", HEIGHT, WIDTH);

    // Pass pointers for GLUT callbacks
    opengl_handler_ptr = &opengl_handler;

    // Initialize Control thread
    Control game_of_life(HEIGHT, WIDTH, 5, &opengl_handler, &frame_mutex, &frame_cv, &frame_ready);
    game_of_life_ptr = &game_of_life;
    game_of_life.start();

    // Set GLUT callbacks
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    // Start GLUT main loop (blocks here, runs on main thread)
    glutMainLoop();

    // Join Control thread if ever needed (never reached in GLUT main loop)
    game_of_life.join();

    return 0;
}