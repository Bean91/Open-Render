#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <cstdint>
#include <vector>
#include <array>
#include <iostream>
#include <cmath>
#include <memory>
using std::cout, std::cin, std::endl;

//Defining Demensions
const uint64_t WIDTH = 852;
const uint64_t HEIGHT = 480;
const uint64_t DEPTH = 480;

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

    color operator*(const double bc) const {
        return {r, g, b, static_cast<uint8_t>(a * bc)};
    }

    bool operator==(const color other) const {
        return (r == other.r && g == other.g && b == other.b && a == other.a);
    }
};

const color EMPTY_COLOR = {0, 0, 0, 0};

//Point struct
struct point {
    double x;
    double y;
    double z;
};

//3 by 3 Matrix type definition
typedef std::array<std::array<double, 3>, 3> matrix;

point matMult(point pnt, matrix mat) {
    double tempX = pnt.x; double tempY = pnt.y; double tempZ = pnt.z;
    tempX = pnt.x * mat[0][0] + pnt.y * mat[0][1] + pnt.z * mat[0][2];
    tempY = pnt.x * mat[1][0] + pnt.y * mat[1][1] + pnt.z * mat[1][2];
    tempZ = pnt.x * mat[2][0] + pnt.y * mat[2][1] + pnt.z * mat[2][2];
    return {tempX, tempY, tempZ};
}

//For 2D
std::array<color, FLATTENED> flattenedPixels = {{EMPTY_COLOR}};
//For displaying data
std::array<uint8_t, FLATTENED*4> flattenedBytes = {{0}};
//For 3D
std::array<color, ABSOLUTE> absolutePixels = {{EMPTY_COLOR}};

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
    if ((x > 0 && y > 0 && z > 0) && (x < WIDTH && y < HEIGHT && z < DEPTH)) absolutePixels[absoluteIndex(x, y, z)] = rgba;
}

//Draws individual pixel in 2D space
void drawFlat(int x, int y, color rgba) {
    if ((x > 0 && y > 0) && (x < WIDTH && y < HEIGHT)) flattenedPixels[flattenedIndex(x, y)] = rgba;
}

//Draws an anti-aliased line starting at (x0, y0, z0) to (x1, y1, z1)
std::pair<std::vector<point>, std::array<int, 3>> drawLine(const double x0, const double y0, const double z0, const double x1, const double y1, const double z1, const color rgba) {
    double dx = std::abs(x1 - x0);
    double dy = std::abs(y1 - y0);
    double dz = std::abs(z1 - z0);

    //Getting sign of vector
    int xs = 1;
    int ys = 1;
    int zs = 1;
    if ((x1 - x0) < 0) {
        xs = -1;
    }
    if ((y1 - y0) < 0) {
        ys = -1;
    }
    if ((z1 - z0) < 0) {
        zs = -1;
    }

    //Total numbers of points (largest distance value)
    int np;
    if (dx > dy && dx > dz) np = (int) dx;
    else if (dy > dx && dy > dz) np = (int) dy;
    else np = (int) dz;

    //Distances between points
    double vx = dx / np * xs;
    double vy = dy / np * ys;
    double vz = dz / np * zs;

    //Get each point
    std::vector<point> points(np);
    for (int i = 0; i < np; i++) {
        points[i] = {x0 + i*vx, y0 + i*vy, z0 + i*vz};
    }

    for (point pnt : points) {
        //Integer parts of point
        int ix = (int) pnt.x;
        int iy = (int) pnt.y;
        int iz = (int) pnt.z;
        //Float parts of point
        double fx = pnt.x - ix;
        double fy = pnt.y - iy;
        double fz = pnt.z - iz;
        //Brightness co-efficients
        double cx0, cx1, cy0, cy1, cz0, cz1;
        if (fx > 0.5) {
            cx0 = fx;
            cx1 = 1 - fx;
        } else {
            cx0 = 1 - fx;
            cx1 = fx;
        }
        if (fy > 0.5) {
            cy0 = fy;
            cy1 = 1 - fy;
        } else {
            cy0 = 1 - fy;
            cy1 = fy;
        }
        if (fz > 0.5) {
            cz0 = fz;
            cz1 = 1 - fz;
        } else {
            cz0 = 1 - fz;
            cz1 = fz;
        }
        drawAbsolute(ix, iy, iz, rgba * ((cx0 + cy0 + cz0) / 3.0));
        drawAbsolute(ix, iy + 1 * ys, iz, rgba * ((cx0 + cy1 + cz0) / 3.0));
        drawAbsolute(ix, iy, iz + 1, rgba * ((cx0 + cy0 + cz1) / 3.0));
        drawAbsolute(ix, iy + 1 * ys, iz + 1 * zs, rgba * ((cx0 + cy1 + cz1) / 3.0));
        drawAbsolute(ix + 1 * xs, iy, iz, rgba * ((cx1 + cy0 + cz0) / 3.0));
        drawAbsolute(ix + 1 * xs, iy + 1 * ys, iz, rgba * ((cx1 + cy1 + cz0) / 3.0));
        drawAbsolute(ix + 1 * xs, iy, iz + 1 * zs, rgba * ((cx1 + cy0 + cz1) / 3.0));
        drawAbsolute(ix + 1 * xs, iy + 1 * ys, iz + 1 * zs, rgba * ((cx1 + cy1 + cz1) / 3.0));
    }
    return {points, {xs, ys, zs}};
}

//Draws a rectangle centered at (x, y, z) with size l x w x h and angles (roll, pitch, yaw)
void drawRecPr(const double x, const double y, const double z, const double l, const double w, const double h, const double roll, const double pitch, const double yaw, const color rgba) {
    std::array<point, 8> localPointList = {{
        {0.0, 0.0, 0.0},
        {0.0, w, 0.0},
        {0.0, 0.0, h},
        {0.0, w, h},
        {l, 0.0, 0.0},
        {l, w, 0.0},
        {l, 0.0, h},
        {l, w, h}
    }};

    std::array<point, 8> vertexList;

    //Matrix for roll
    matrix rMat = {{
        {1.0, 0.0, 0.0},
        {0.0, cos(roll), -sin(roll)},
        {0.0, sin(roll), cos(roll)}
    }};
    //Matrix for pitch
    matrix pMat = {{
        {cos(pitch), 0.0, sin(pitch)},
        {0.0, 1.0, 0.0},
        {-sin(pitch), 0.0, cos(pitch)}
    }};
    //Matrix for yaw
    matrix yMat = {{
        {cos(yaw), -sin(yaw), 0.0},
        {sin(yaw), cos(yaw), 0.0},
        {0.0, 0.0, 1.0}
    }};
    //Array of matrices
    std::array<matrix, 3> matArr = {rMat, pMat, yMat};

    for (int i = 0; i < size(localPointList); i++) {
        point pnt = localPointList[i];
        if (i < 4) {
            pnt.x += x - l/2.0; pnt.y += y - w/2.0; pnt.z += z - h/2.0;
        } else {
            pnt.x += x + l/2.0; pnt.y += y + w/2.0; pnt.z += z + h/2.0;
        }
        for (matrix mat : matArr) {
            pnt = matMult(pnt, mat);
        }
        vertexList[i] = pnt;
        cout << pnt.x << " " << pnt.y << " " << pnt.z<< endl;
    }

    std::array<std::pair<std::vector<point>, std::array<int, 3>>, 12> edgeDataList = {{
        drawLine(vertexList[0].x, vertexList[0].y, vertexList[0].z, vertexList[1].x, vertexList[1].y, vertexList[1].z, rgba),
        drawLine(vertexList[0].x, vertexList[0].y, vertexList[0].z, vertexList[4].x, vertexList[4].y, vertexList[4].z, rgba),
        drawLine(vertexList[1].x, vertexList[1].y, vertexList[1].z, vertexList[5].x, vertexList[5].y, vertexList[5].z, rgba),
        drawLine(vertexList[4].x, vertexList[4].y, vertexList[4].z, vertexList[5].x, vertexList[5].y, vertexList[5].z, rgba),
        drawLine(vertexList[2].x, vertexList[2].y, vertexList[2].z, vertexList[3].x, vertexList[3].y, vertexList[3].z, rgba),
        drawLine(vertexList[2].x, vertexList[2].y, vertexList[2].z, vertexList[6].x, vertexList[6].y, vertexList[6].z, rgba),
        drawLine(vertexList[3].x, vertexList[3].y, vertexList[3].z, vertexList[7].x, vertexList[5].y, vertexList[7].z, rgba),
        drawLine(vertexList[6].x, vertexList[6].y, vertexList[6].z, vertexList[7].x, vertexList[7].y, vertexList[7].z, rgba),
        drawLine(vertexList[0].x, vertexList[0].y, vertexList[0].z, vertexList[2].x, vertexList[2].y, vertexList[2].z, rgba),
        drawLine(vertexList[1].x, vertexList[1].y, vertexList[1].z, vertexList[3].x, vertexList[3].y, vertexList[3].z, rgba),
        drawLine(vertexList[4].x, vertexList[4].y, vertexList[4].z, vertexList[6].x, vertexList[6].y, vertexList[6].z, rgba),
        drawLine(vertexList[5].x, vertexList[5].y, vertexList[5].z, vertexList[7].x, vertexList[7].y, vertexList[7].z, rgba)
    }};
}

//Convert the 3D data to 2D
void projAbsToFlat(double r, double p, double y) {
    for (int i = 0; i < DEPTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            for (int k = 0; k < WIDTH; k++) {
                if (flattenedPixels[flattenedIndex(k, j)] == EMPTY_COLOR) {
                    flattenedPixels[flattenedIndex(k, j)] = absolutePixels[absoluteIndex(k, j, i)];
                } else {
                    continue;
                }
            }
        }
    }
}

void* convToBytes() {
    for (int i = 0; i < FLATTENED; i++) {
        flattenedBytes[i*4] = flattenedPixels[i].r;
        flattenedBytes[i*4+1] = flattenedPixels[i].g;
        flattenedBytes[i*4+2] = flattenedPixels[i].b;
        flattenedBytes[i*4+3] = flattenedPixels[i].a;
    }
    void* ptr = &flattenedBytes[0];
    return ptr;
}

int main() {
    drawRecPr(400.0, 200.0, 0.0, 100.0, 100.0, 100.0, 0.0, 0.0, 0.0, {255, 255, 255, 255});
//    drawLine(10.0, 352.0, 0.0, 500.0, 0.0, 0.0, {255, 255, 255, 255});

    projAbsToFlat(0.0, 0.0, 0.0);

    char const *filename = "filename.png";
    stbi_write_png(filename, WIDTH, HEIGHT, 4, convToBytes(), 0);
    return 0;
}