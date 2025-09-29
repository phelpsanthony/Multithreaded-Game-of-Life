//
// Created by Anthony Phelps on 2025-09-28.
//

#include "OpenGLHandler.h"

#include <GL/freeglut_std.h>

void OpenGLHandler::initWindow(int argc, char **argv, const char *title, int height, int width) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow(title);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);    // Match window size
}

void OpenGLHandler::drawFrame(const std::vector<std::vector<Cell>>& grid) {
    int numRows = grid.size();        // height
    int numCols = grid[0].size();     // width

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Calculate cell width and height in normalized OpenGL coordinates [-1, 1]
    float cellWidth = 2.0f / numCols;
    float cellHeight = 2.0f / numRows;

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            const Cell& cell = grid[row][col];

            if (!cell.getAlive()) {
                glColor3f(0.0f, 0.0f, 0.0f); // Dead → black
            } else {
                switch (cell.getSpeciesId()) {
                    case 0: glColor3f(1.0f, 0.5f, 0.5f); break;   // Light Red
                    case 1: glColor3f(0.5f, 1.0f, 0.5f); break;   // Light Green
                    case 2: glColor3f(0.5f, 0.5f, 1.0f); break;   // Light Blue
                    case 3: glColor3f(1.0f, 1.0f, 0.5f); break;   // Light Yellow
                    case 4: glColor3f(1.0f, 0.5f, 1.0f); break;   // Light Magenta
                    case 5: glColor3f(0.5f, 1.0f, 1.0f); break;   // Light Cyan
                    case 6: glColor3f(1.0f, 0.7f, 0.5f); break;   // Light Orange
                    case 7: glColor3f(0.7f, 0.5f, 1.0f); break;   // Light Purple
                    case 8: glColor3f(0.8f, 0.8f, 0.8f); break;   // Light Gray
                    case 9: glColor3f(0.5f, 1.0f, 0.7f); break;   // Light Lime
                    default: glColor3f(1.0f, 1.0f, 1.0f); break;  // White
                }
            }

            // Draw a quad for each cell
            float xLeft   = -1.0f + col * cellWidth;
            float xRight  = -1.0f + (col + 1) * cellWidth;
            float yTop    =  1.0f - row * cellHeight;
            float yBottom =  1.0f - (row + 1) * cellHeight;

            glBegin(GL_QUADS);
                glVertex2f(xLeft, yTop);
                glVertex2f(xRight, yTop);
                glVertex2f(xRight, yBottom);
                glVertex2f(xLeft, yBottom);
            glEnd();
        }
    }

    // Swap buffers to display the frame
    glutSwapBuffers();
}

