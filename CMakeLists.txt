cmake_minimum_required(VERSION 4.0)
project(COMP426A1)

set(CMAKE_CXX_STANDARD 20)

add_executable(COMP426A1 main.cpp
        Cell.cpp
        Cell.h
        Grid.cpp
        Grid.h
        Worker.cpp
        Worker.h
        Control.cpp
        Control.h
        OpenGLHandler.cpp
        OpenGLHandler.h
)

# Include Homebrew headers
include_directories(/opt/homebrew/include)

# Link Homebrew libraries
link_directories(/opt/homebrew/lib)

# Link OpenGL, GLUT (framework), and GLEW (library)
target_link_libraries(COMP426A1
        "-framework OpenGL"
        "-framework GLUT"
        /opt/homebrew/lib/libGLEW.dylib
)
