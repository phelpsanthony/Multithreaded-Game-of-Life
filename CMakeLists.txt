cmake_minimum_required(VERSION 3.16)
project(COMP426A1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Find TBB (Intel oneAPI Threading Building Blocks) ---
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /opt/homebrew)
find_package(TBB QUIET)

# Fallback: manually set include and library paths if find_package didn't populate them
if (NOT TBB_FOUND)
    set(TBB_INCLUDE_DIRS /opt/homebrew/include)
    set(TBB_LIBRARIES /opt/homebrew/lib/libtbb.dylib)
else()
    set(TBB_INCLUDE_DIRS ${TBB_INCLUDE_DIRS})
    set(TBB_LIBRARIES TBB::tbb)
endif()

# Include directories (Homebrew + TBB)
include_directories(
        /opt/homebrew/include
        ${TBB_INCLUDE_DIRS}
)

# Link directories (Homebrew)
link_directories(/opt/homebrew/lib)

# Define executable and source files
add_executable(COMP426A1
        main.cpp
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

# Link libraries: OpenGL, GLUT, GLEW, TBB
target_link_libraries(COMP426A1
        "-framework OpenGL"
        "-framework GLUT"
        /opt/homebrew/lib/libGLEW.dylib
        ${TBB_LIBRARIES}
)

