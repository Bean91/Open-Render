#include <cstdint>
#include <vector>
#include <array>
#include <iostream>
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
struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

//For 2D
std::array<Color, FLATTENED> flattenedPixels;
//For 3D
std::array<Color, ABSOLUTE> absolutePixels;

//Smart indexing for 1D arrays.
//For 2D
uint64_t flattenedIndex(int x, int y) {
    return y * WIDTH + x;
}
//For 3D
uint64_t absoluteIndex(int x, int y, int z) {
    return z * HEIGHT * WIDTH + y * WIDTH + x;
}

//Converts four uint8_t's into a Color struct
Color rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    Color temp;
    temp.r = r;
    temp.g = g;
    temp.b = b;
    temp.a = a;
    return temp;
}

//Draws individual pixel in 3D space
void drawAbsolute(int x, int y, int z, Color rgba) {
    absolutePixels[absoluteIndex(x, y, z)] = rgba;
}

//Draws individual pixel in 2D space
void drawFlat(int x, int y, Color rgba) {
    flattenedPixels[flattenedIndex(x, y)] = rgba;
}

void drawRecPr(int x0, int y0, int z0, int x1, int y1, int z1, int width, int length, double angle, Color rgba) {

}