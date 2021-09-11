# OscillatorGrid

## Description

Grid of oscillators solved with Euler method implemented in C++/OpenGL.

## Demo

Hexagonal grid

![alt text](https://github.com/EQUINOX24/OscillatorGrid/blob/master/demo_hex.gif)

Square grid

![alt text](https://github.com/EQUINOX24/OscillatorGrid/blob/master/demo_aquare.gif)

## "Documentation"

Python is used to generate `mesh.hpp`. For now `main.cpp` file is a mess, but you can control simulation from the `int main()` function.

## Dependencies

- OpenGL 4.5

#### Dependencies to generate a mesh

- Python 3.9.6
- Numpy 1.21.0

## TODO

- Make the simulation interactive
- Abstract out differential equation solving
- Add higher order methods (e.g. Runge-Kutta)
