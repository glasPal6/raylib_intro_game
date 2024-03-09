# rayPong

The purpose of this is to get a basic understanding of raylib. \
Most of this is based off of: https://github.com/raysan5/raylib-intro-course, definitley check that out.

## The Game

A basic, classic pong game. \
There is a memory leak, but I think that it is a problem on Raylib's side. \
The game has only been tested on Ubuntu 22.04.

## Building

```
mkdir build ; cd build
cp -r ../src/assets .
cmake ..
make
./rayPong
```

Note that `raylib` as a libraray will be fetched by the CMake build if it is not on your system.

## Controls

The player controls can be changed at the definitions in `main.c`
- Player 1:
    - "M" up
    - "N" down
- Player 2:
    - "Q" up
    - "E" down

The game can ge paused with "p"
