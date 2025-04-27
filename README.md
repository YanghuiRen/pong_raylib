# Pong Game

This is a simple Pong game implemented in C++ using the [Raylib](https://www.raylib.com/) graphics library.  
The game features basic AI, keyboard controls, scoring, and a start screen.

## Features

- Two-player Pong gameplay (Player vs. AI)
- Score tracking and win condition
- Start screen with button interaction
- Cross-platform support via CMake
- Basic color and shape rendering using Raylib

## Requirements

- C++11-compatible compiler
- Raylib 5.0 or higher
- CMake 3.11 or higher

## Build Instructions

### Linux/macOS

```bash
git clone git@github.com:YanghuiRen/pong_raylib.git
cd pong_raylib
mkdir build && cd build
cmake ..
make
./pong
```

### Windows (Cross-compilation)

```bash
mkdir build-win && cd build-win
cmake .. -DCMAKE_TOOLCHAIN_FILE=../win_toolchain.cmake
make
```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

© 2025 Yanghui Ren

