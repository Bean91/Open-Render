#include <cstdint>
#include <vector>
#include <array>
#include <iostream>
#include <cmath>
using std::cout, std::cin, std::endl;

//Defining Demensions
const uint64_t WIDTH = 1920;
const uint64_t HEIGHT = 1080;
const uint64_t DEPTH = 1080;

//For 2D
const uint64_t FLATTENED = HEIGHT * WIDTH;
//For 3D
const uint64_t ABSOLUTE = HEIGHT * WIDTH * DEPTH;

//Color struct
struct color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

//Point struct
struct point {
    int x;
    int y;
    int z;
};

//3 by 3 Matrix type definition
typedef std::array<std::array<double, 3>, 3> matrix;

//For 2D
std::array<color, FLATTENED> flattenedPixels = {{255, 255, 255, 255}};
//For 3D
std::array<color, ABSOLUTE> absolutePixels = {{255, 255, 255, 255}};

//Smart indexing for 1D arrays.
//For 2D
uint64_t flattenedIndex(int x, int y) {
    return y * WIDTH + x;
}
//For 3D
uint64_t absoluteIndex(int x, int y, int z) {
    return z * HEIGHT * WIDTH + y * WIDTH + x;
}

//Converts four uint8_t's into a color struct
color rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return {r, g, b, a};
}

//Draws individual pixel in 3D space
void drawAbsolute(int x, int y, int z, color rgba) {
    absolutePixels[absoluteIndex(x, y, z)] = rgba;
}

//Draws individual pixel in 2D space
void drawFlat(int x, int y, color rgba) {
    flattenedPixels[flattenedIndex(x, y)] = rgba;
}

//
void drawRecPr(const int x, const int y, const int z, const int l, const int w, const int h, const int roll, const int pitch, const int yaw, const color rgba) {
    std::array<point, 8> localPointList = {
        {0, 0, 0},
        {0, y, 0},
        {0, 0, z},
        {0, y, z},
        {x, 0, 0},
        {x, y, 0},
        {x, 0, z},
        {x, y, z}
    };

    matrix rMat = {
        {1.0, 0.0, 0.0},
        {0.0, cos(roll), -1.0 * sin(roll)},
        {0.0, sin(roll), cos(roll)}
    };

    matrix pMat = {
        {cos(pitch), 0.0, sin(pitch)},
        {0.0, 1.0, 0.0},
        {-1.0 * sin(pitch), 0.0, cos(pitch)}
    };

    matrix yMat = {
        {cos(yaw), -1.0 * sin(yaw), 0.0},
        {sin(yaw), cos(yaw), 0.0},
        {0.0, 0.0, 1.0}
    };
}