# Multi-Species Game of Life

A **C++** implementation of Conway’s Game of Life extended to support **multiple species** on a 2D grid, with **real-time OpenGL visualization**. Each species is represented by a different color, and the simulation runs efficiently using **multithreading**.
This implementation uses CPU multithreading. This is an assignment for COMP 436 Multicore programming.
---

## Features

- Supports **up to 10 species** of cells.
- Each cell can be **alive or dead**.
- Implements the classic Game of Life rules:
  - Any live cell with fewer than two live neighbors dies (under-population).
  - Any live cell with two or three live neighbors lives on.
  - Any live cell with more than three live neighbors dies (over-population).
  - Any dead cell with exactly three live neighbors becomes alive (reproduction).
- **Multithreaded computation**:
  - Separate threads handle the computation of next states for each species.
  - Efficient synchronization using `std::barrier` and `std::condition_variable`.
- Real-time rendering using **OpenGL (FreeGLUT)**.

---

## Requirements

- **C++17** or higher
- **OpenGL**, **GLEW**, and **FreeGLUT**
- CMake (for building)

On macOS, you can install dependencies via Homebrew:

```bash
brew install glew freeglut

git clone https://github.com/<your-username>/game-of-life-multispecies.git
cd game-of-life-multispecies


mkdir build
cd build
cmake ..
make


./GameOfLife


Author: Anthony Phelps
