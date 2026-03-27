# AlgoToolKit

A C++ toolkit for algorithm analysis and simulation, built with OpenGL and Dear ImGui.

![AlgoToolKit Demo](demo.gif)
## Build and Run

This project uses CMake for building.

### 1. Configure
```bash
cmake -B build
```

### 2. Build
```bash
cmake --build build
```

### 3. Run
*   **macOS / Linux**:
    ```bash
    ./build/AlgoToolkit
    ```
*   **Windows**:
    ```powershell
    .\build\Debug\AlgoToolkit.exe
    ```


## Requirements:
- CMake (3.12+)
- GLFW 3.3+
- OpenGL 3.3+

## Structure:
- `src/main.cpp`: Entry point and main loop.
- `src/Menu/`: UI Logic.
- `src/ImGui/`: Dear ImGui sources and backend implementations.
