//
// Created by Anthony Phelps on 2025-09-28.
//

#ifndef COMP426A1_OPENGLHANDLER_H
#define COMP426A1_OPENGLHANDLER_H

#include <vector>
#include "Cell.h"

using namespace std;

class OpenGLHandler {
public:
    OpenGLHandler() = default;
    void initWindow(int argc, char**argv, const char* title, int height, int width);
    void drawFrame(const vector<vector <Cell>>& grid);
};


#endif //COMP426A1_OPENGLHANDLER_H