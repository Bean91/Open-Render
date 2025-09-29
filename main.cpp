#include <cstdint>
#include <vector>
#include <array>
#include <iostream>
#include <cmath>
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
        return {r * bc, g * bc, b * bc, a};
    }
};

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
std::array<color, FLATTENED> flattenedPixels = {{{255, 255, 255, 255}}};
//For 3D
std::array<color, ABSOLUTE> absolutePixels = {{{255, 255, 255, 255}}};

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

//Draws an anti-aliased line starting at (x0, y0, z0) to (x1, y1, z1)
void drawLine(const double x0, const double y0, const double z0, const double x1, const double y1, const double z1, const color rgba) {
    double dx = std::abs(x0 - x1);
    double dy = std::abs(y0 - y1);
    double dz = std::abs(z0 - z1);

    //Total numbers of points (largest distance value)
    int np;
    if (dx > dy && dx > dz) np = (int) dx;
    else if (dy > dx && dy > dz) np = (int) dx;
    else np = (int) dz;

    //Distances between points
    double vx = dx / np;
    double vy = dy / np;
    double vz = dz / np;

    //Get each point
    std::array<point, np> points;
    for (int i = 0; i < np; i++) {
        points[i] = {x0 + i*dx, y0 + i*dy, z0 + i*dz};
    }

    for (point pnt : points) {
        //Integer parts of point
        int ix = (int) pnt.x;
        int iy = (int) pnt.y;
        int iz = (int) pnt.z;
        //Float parts of point
        int fx = pnt.x - ix;
        int fy = pnt.y - iy;
        int fz = pnt.z - iz;
        //Brightness co-efficients
        double cx0, cx1, cy0, cy1, cz0, cz1;
        if (fx > 0.5) {
            cx0 = 1 - fx;
            cx1 = fx;
        } else {
            cx0 = fx;
            cx1 = 1 - fx;
        }
        if (fy > 0.5) {
            cy0 = 1 - fy;
            cy1 = fy;
        } else {
            cy0 = fy;
            cy1 = 1 - fy;
        }
        if (fz > 0.5) {
            cz0 = 1 - fz;
            cz1 = fz;
        } else {
            cz0 = fz;
            cz1 = 1 - fz;
        }
        drawAbsolute(ix, iy, iz, color * (cx0 * cy0 * cz0 / 3));
        drawAbsolute(ix, iy + 1, iz, color * (cx0 * cy1 * cz0 / 3));
        drawAbsolute(ix, iy, iz + 1, color * (cx0 * cy0 * cz1 / 3));
        drawAbsolute(ix, iy + 1, iz + 1, color * (cx0 * cy1 * cz1 / 3));
        drawAbsolute(ix + 1, iy, iz, color * (cx1 * cy0 * cz0 / 3));
        drawAbsolute(ix + 1, iy + 1, iz, color * (cx1 * cy1 * cz0 / 3));
        drawAbsolute(ix + 1, iy, iz + 1, color * (cx1 * cy0 * cz1 / 3));
        drawAbsolute(ix + 1, iy + 1, iz + 1, color * (cx1 * cy1 * cz1 / 3));
    }
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
        cout << vertexList[i].x << " " << vertexList[i].y << " " << vertexList[i].z << endl;
    }
}

int main() {
    drawRecPr(10.0, 31.0, 5.0, 13.0, 64.0, 42.0, 24.0, 21.0, 63.0, {255, 255, 255, 255});
    return 0;
}