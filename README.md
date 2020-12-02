# ECE 4122 Fall 2020 Final Project - Pac-Man Game

**Author** Jackson McMath
`jmcmath3` | `903300636`

## Introduction

This project aims to recreate the classic Pac-Man game in 3D with C++ utilizing OpenGL.

## Building and Running Code Locally

This project was developed on macOS Catalina (10.15.7). No third party libraries were used, only OpenGL and standard libraries.

### Compiling

The code is compiled using C++11, OpenGL and GLUT frameworks, and lpthread for threading.
```bash
g++ -std=c++11 *.cpp -o a.out -framework GLUT -framework OpenGL -framework Cocoa -lpthread
```
The `-Wno-deprecated-declarations` flag can also be included to avoid GLUT deprecation warnings (since 10.9).

### Running

The program is run using the command
```bash
./a.out
```
or by substituting `a.out` with the out file from the compile command.

### Controls & Gameplay

Pac-man is controlled using the arrow keys.
The game starts when any key is pressed.
The game can be paused by pressing `p` or the space bar.

Pac-man has three lives to eat all the pellets on the map.
Pac-man loses a life by touching a ghost, unless a power pellet has been eaten in the last 7 seconds.
If Pac-man eats a power pellet, he can eat the ghosts for 7 seconds, forcing them to respawn in the ghost house.
If Pac-man loses all three lives or eats all the pellets, the game starts over.
