#include <cstdint>
#include <vector>
#include <array>
#include <iostream>
#include <cmath>
#include <memory>
#include <numbers>
#include <map>

//Defining Demensions
#ifndef WIDTH
    #define WIDTH 852
#endif
#ifndef HEIGHT
    #define HEIGHT 480
#endif
#ifndef DEPTH
    #define DEPTH 480
#endif

#define PI std::numbers::pi

namespace openrender {
    //For 2D
    const uint64_t FLATTENED = HEIGHT * WIDTH;
    //For 3D
    const uint64_t ABSOLUTE = HEIGHT * WIDTH * DEPTH;

    //For converting degrees to radians
    const double convRate = PI / 180;

    //Color struct
    struct Color {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        Color operator*(const double bc) const {
            return {r, g, b, static_cast<uint8_t>(a * bc)};
        }

        bool operator==(const Color other) const {
            return (r == other.r && g == other.g && b == other.b && a == other.a);
        }
    };

    const Color EMPTY_COLOR = {0, 0, 0, 0};

    //Point struct
    struct Point {
        double x;
        double y;
        double z;
    };

    //Object data storing struct
    struct Object {
        std::vector<Point> points;
        Color rgba;
        bool rendered;
    };

    //3 by 3 Matrix type definition
    typedef std::array<std::array<double, 3>, 3> Matrix;

    //3 by 1 Matrix times a 3 by 3 Matrix
    Point matMult(Point pnt, Matrix mat) {
        double tempX = pnt.x; double tempY = pnt.y; double tempZ = pnt.z;
        tempX = pnt.x * mat[0][0] + pnt.y * mat[0][1] + pnt.z * mat[0][2];
        tempY = pnt.x * mat[1][0] + pnt.y * mat[1][1] + pnt.z * mat[1][2];
        tempZ = pnt.x * mat[2][0] + pnt.y * mat[2][1] + pnt.z * mat[2][2];
        return {tempX, tempY, tempZ};
    }

    //For 2D
    std::array<Color, FLATTENED> flattenedPixels = {{EMPTY_COLOR}};
    //For displaying data
    std::array<uint8_t, FLATTENED*4> flattenedBytes = {{0}};
    //For 3D
    std::array<Color, ABSOLUTE> absolutePixels = {{EMPTY_COLOR}};

    //Smart indexing for 1D arrays.
    //For 2D
    uint64_t flattenedIndex(int x, int y) {
        return y * WIDTH + x;
    }

    //For 3D
    uint64_t absoluteIndex(int x, int y, int z) {
        return z * HEIGHT * WIDTH + y * WIDTH + x;
    }

    //Draws individual pixel in 3D space
    void drawAbsolute(Point pnt, Color rgba) {
        if ((pnt.x > 0) && (pnt.y > 0) && (pnt.z > 0) && (pnt.x < WIDTH) && (pnt.y < HEIGHT) && (pnt.z < DEPTH)) {
            if (absolutePixels[absoluteIndex(pnt.x, pnt.y, pnt.z)] == EMPTY_COLOR) {
                absolutePixels[absoluteIndex(pnt.x, pnt.y, pnt.z)] = rgba;
            } else {
                Color temp = absolutePixels[absoluteIndex(pnt.x, pnt.y, pnt.z)];

                uint8_t a = 255-((255-temp.a)*(255-rgba.a)/255);
                uint8_t r = (temp.r*(255-rgba.a)+rgba.r*rgba.a)/255;
                uint8_t g = (temp.g*(255-rgba.a)+rgba.g*rgba.a)/255;
                uint8_t b = (temp.b*(255-rgba.a)+rgba.b*rgba.a)/255;

                absolutePixels[absoluteIndex(pnt.x, pnt.y, pnt.z)] = {r, g, b, a};
            }
        }
    }

    //Draws individual pixel in 2D space
    void drawFlat(int x, int y, Color rgba) {
        if ((x > 0) && (y > 0) && (x < WIDTH) && (y < HEIGHT))
            flattenedPixels[flattenedIndex(x, y)] = rgba;
    }

    //Draws a line starting at (x0, y0, z0) to (x1, y1, z1)
    Object drawLine(const double x0, const double y0, const double z0, const double x1, const double y1, const double z1, const Color rgba) {
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
        if (dx >= dy && dx >= dz) np = (int) dx;
        else if (dy >= dx && dy >= dz) np = (int) dy;
        else np = (int) dz;

        //Distances between points
        double vx = dx / np * xs;
        double vy = dy / np * ys;
        double vz = dz / np * zs;

        //Get each point
        std::vector<Point> points(np);
        for (int i = 0; i < np; i++) {
            points[i] = {x0 + i*vx, y0 + i*vy, z0 + i*vz};
        }

        Object obj(points, rgba, true);
        return obj;
    }

    //Draws a triangle with verteces (x0, y0, z0), (x1, y1, z1), (x2, y3, z4)
    Object drawTriangle(const double x0, const double y0, const double z0, const double x1, const double y1, const double z1, const double x2, const double y2, const double z2, const Color rgba) {
        std::array<std::vector<Point>, 3> pointData;
        pointData[0] = drawLine(x0, y0, z0, x1, y1, z1, rgba).points;
        pointData[1] = drawLine(x1, y1, z1, x2, y2, z2, rgba).points;
        pointData[2] = drawLine(x0, y0, z0, x2, y2, z2, rgba).points;
        int l0 = pointData[0].size();
        int l1 = pointData[1].size();
        int l2 = pointData[2].size();

        std::vector<Point> pointsList;

        if (l0 <= l1) {
            for (int i = 0; i < l0-1; i++) {
                std::vector<Point> temp = drawLine(pointData[0][i].x, pointData[0][i].y, pointData[0][i].z, pointData[1][l1-1-i].x, pointData[1][l1-1-i].y, pointData[1][l1-1-i].z, rgba).points;
                pointsList.insert(pointsList.end(), temp.begin(), temp.end());
            }
        } else {
            for (int i = 0; i < l1-1; i++) {
                std::vector<Point> temp = drawLine(pointData[0][i].x, pointData[0][i].y, pointData[0][i].z, pointData[1][l1-1-i].x, pointData[1][l1-1-i].y, pointData[1][l1-1-i].z, rgba).points;
                pointsList.insert(pointsList.end(), temp.begin(), temp.end());
            }
        }

        if (l1 <= l2) {
            for (int i = 0; i < l1-1; i++) {
                std::vector<Point> temp = drawLine(pointData[1][i].x, pointData[1][i].y, pointData[1][i].z, pointData[2][i].x, pointData[2][i].y, pointData[2][i].z, rgba).points;
                pointsList.insert(pointsList.end(), temp.begin(), temp.end());
            }
        } else {
            for (int i = 0; i < l2-1; i++) {
                std::vector<Point> temp = drawLine(pointData[1][i].x, pointData[1][i].y, pointData[1][i].z, pointData[2][i].x, pointData[2][i].y, pointData[2][i].z, rgba).points;
                pointsList.insert(pointsList.end(), temp.begin(), temp.end());
            }
        }

        if (l0 <= l2) {
            for (int i = 0; i < l0-1; i++) {
                std::vector<Point> temp = drawLine(pointData[0][i].x, pointData[0][i].y, pointData[0][i].z, pointData[2][i].x, pointData[2][i].y, pointData[2][i].z, rgba).points;
                pointsList.insert(pointsList.end(), temp.begin(), temp.end());
            }
        } else {
            for (int i = 0; i < l2-1; i++) {
                std::vector<Point> temp = drawLine(pointData[0][i].x, pointData[0][i].y, pointData[0][i].z, pointData[2][i].x, pointData[2][i].y, pointData[2][i].z, rgba).points;
                pointsList.insert(pointsList.end(), temp.begin(), temp.end());
            }
        }
        Object obj(pointsList, rgba, true);
        return obj;
    }

    //Draws a rectangle outline centered at (x, y, z) with size l x w x h and angles (roll, pitch, yaw)
    Object drawRecPrOut(const double x, const double y, const double z, const double l, const double w, const double h, const Color rgba, double roll = 0.0, double pitch = 0.0, double yaw = 0.0) {
        std::array<Point, 8> localPointList = {{
            {-l/2, -w/2, -h/2},
            {-l/2, w/2, -h/2},
            {-l/2, -w/2, h/2},
            {-l/2, w/2, h/2},
            {l/2, -w/2, -h/2},
            {l/2, w/2, -h/2},
            {l/2, -w/2, h/2},
            {l/2, w/2, h/2}
        }};

        //Converting to radians
        roll *= convRate;
        pitch *= convRate;
        yaw *= convRate;

        std::array<Point, 8> vertexList;

        //Matrix for roll
        Matrix rMat = {{
            {1.0, 0.0, 0.0},
            {0.0, cos(roll), -sin(roll)},
            {0.0, sin(roll), cos(roll)}
        }};
        //Matrix for pitch
        Matrix pMat = {{
            {cos(pitch), 0.0, sin(pitch)},
            {0.0, 1.0, 0.0},
            {-sin(pitch), 0.0, cos(pitch)}
        }};
        //Matrix for yaw
        Matrix yMat = {{
            {cos(yaw), -sin(yaw), 0.0},
            {sin(yaw), cos(yaw), 0.0},
            {0.0, 0.0, 1.0}
        }};
        //Array of matrices
        std::array<Matrix, 3> matArr = {rMat, pMat, yMat};

        for (int i = 0; i < size(localPointList); i++) {
            Point pnt = localPointList[i];
            for (Matrix mat : matArr) {
                pnt = matMult(pnt, mat);
            }
            pnt.x += x;
            pnt.y += y;
            pnt.z += z;
            vertexList[i] = {round(pnt.x), round(pnt.y), round(pnt.z)};
        }

        std::vector<Point> pointsList;
        std::vector<Point> temp;

        temp = drawLine(vertexList[0].x, vertexList[0].y, vertexList[0].z, vertexList[1].x, vertexList[1].y, vertexList[1].z, rgba).points;
        pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        temp = drawLine(vertexList[0].x, vertexList[0].y, vertexList[0].z, vertexList[4].x, vertexList[4].y, vertexList[4].z, rgba).points;
        pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        temp = drawLine(vertexList[1].x, vertexList[1].y, vertexList[1].z, vertexList[5].x, vertexList[5].y, vertexList[5].z, rgba).points;
        pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        temp = drawLine(vertexList[4].x, vertexList[4].y, vertexList[4].z, vertexList[5].x, vertexList[5].y, vertexList[5].z, rgba).points;
        pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        temp = drawLine(vertexList[2].x, vertexList[2].y, vertexList[2].z, vertexList[3].x, vertexList[3].y, vertexList[3].z, rgba).points;
        pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        temp = drawLine(vertexList[2].x, vertexList[2].y, vertexList[2].z, vertexList[6].x, vertexList[6].y, vertexList[6].z, rgba).points;
        pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        temp = drawLine(vertexList[3].x, vertexList[3].y, vertexList[3].z, vertexList[7].x, vertexList[7].y, vertexList[7].z, rgba).points;
        pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        temp = drawLine(vertexList[6].x, vertexList[6].y, vertexList[6].z, vertexList[7].x, vertexList[7].y, vertexList[7].z, rgba).points;
        pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        temp = drawLine(vertexList[0].x, vertexList[0].y, vertexList[0].z, vertexList[2].x, vertexList[2].y, vertexList[2].z, rgba).points;
        pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        temp = drawLine(vertexList[1].x, vertexList[1].y, vertexList[1].z, vertexList[3].x, vertexList[3].y, vertexList[3].z, rgba).points;
        pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        temp = drawLine(vertexList[4].x, vertexList[4].y, vertexList[4].z, vertexList[6].x, vertexList[6].y, vertexList[6].z, rgba).points;
        pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        temp = drawLine(vertexList[5].x, vertexList[5].y, vertexList[5].z, vertexList[7].x, vertexList[7].y, vertexList[7].z, rgba).points;
        pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        Object obj(pointsList, rgba, true);
        return obj;
    }

    //Draws a filled rectangle centered at (x, y, z) with size l x w x h and angles (roll, pitch, yaw)
    Object drawRecPrFill(const double x, const double y, const double z, const double l, const double w, const double h, const Color rgba, double roll = 0.0, double pitch = 0.0, double yaw = 0.0) {
        std::array<Point, 8> localPointList = {{
            {-l/2, -w/2, -h/2},
            {-l/2, w/2, -h/2},
            {-l/2, -w/2, h/2},
            {-l/2, w/2, h/2},
            {l/2, -w/2, -h/2},
            {l/2, w/2, -h/2},
            {l/2, -w/2, h/2},
            {l/2, w/2, h/2}
        }};
        std::vector<Point> pointsList;
        std::vector<Point> temp;

        //Converting to radians
        roll *= convRate;
        pitch *= convRate;
        yaw *= convRate;

        std::array<Point, 8> vertexList;

        //Matrix for roll
        Matrix rMat = {{
            {1.0, 0.0, 0.0},
            {0.0, cos(roll), -sin(roll)},
            {0.0, sin(roll), cos(roll)}
        }};
        //Matrix for pitch
        Matrix pMat = {{
            {cos(pitch), 0.0, sin(pitch)},
            {0.0, 1.0, 0.0},
            {-sin(pitch), 0.0, cos(pitch)}
        }};
        //Matrix for yaw
        Matrix yMat = {{
            {cos(yaw), -sin(yaw), 0.0},
            {sin(yaw), cos(yaw), 0.0},
            {0.0, 0.0, 1.0}
        }};
        //Array of matrices
        std::array<Matrix, 3> matArr = {rMat, pMat, yMat};

        for (int i = 0; i < size(localPointList); i++) {
            Point pnt = localPointList[i];
            for (Matrix mat : matArr) {
                pnt = matMult(pnt, mat);
            }
            pnt.x += x;
            pnt.y += y;
            pnt.z += z;
            vertexList[i] = {round(pnt.x), round(pnt.y), round(pnt.z)};
        }

        std::array<std::vector<Point>, 12> edgeDataList = {{
            drawLine(vertexList[0].x, vertexList[0].y, vertexList[0].z, vertexList[1].x, vertexList[1].y, vertexList[1].z, rgba).points,
            drawLine(vertexList[0].x, vertexList[0].y, vertexList[0].z, vertexList[4].x, vertexList[4].y, vertexList[4].z, rgba).points,
            drawLine(vertexList[1].x, vertexList[1].y, vertexList[1].z, vertexList[5].x, vertexList[5].y, vertexList[5].z, rgba).points,
            drawLine(vertexList[4].x, vertexList[4].y, vertexList[4].z, vertexList[5].x, vertexList[5].y, vertexList[5].z, rgba).points,
            drawLine(vertexList[2].x, vertexList[2].y, vertexList[2].z, vertexList[3].x, vertexList[3].y, vertexList[3].z, rgba).points,
            drawLine(vertexList[2].x, vertexList[2].y, vertexList[2].z, vertexList[6].x, vertexList[6].y, vertexList[6].z, rgba).points,
            drawLine(vertexList[3].x, vertexList[3].y, vertexList[3].z, vertexList[7].x, vertexList[7].y, vertexList[7].z, rgba).points,
            drawLine(vertexList[6].x, vertexList[6].y, vertexList[6].z, vertexList[7].x, vertexList[7].y, vertexList[7].z, rgba).points,
            drawLine(vertexList[0].x, vertexList[0].y, vertexList[0].z, vertexList[2].x, vertexList[2].y, vertexList[2].z, rgba).points,
            drawLine(vertexList[1].x, vertexList[1].y, vertexList[1].z, vertexList[3].x, vertexList[3].y, vertexList[3].z, rgba).points,
            drawLine(vertexList[4].x, vertexList[4].y, vertexList[4].z, vertexList[6].x, vertexList[6].y, vertexList[6].z, rgba).points,
            drawLine(vertexList[5].x, vertexList[5].y, vertexList[5].z, vertexList[7].x, vertexList[7].y, vertexList[7].z, rgba).points
        }};

        for (int i = 0; i < edgeDataList[0].size(); i++) {
            temp = drawLine(edgeDataList[0][i].x, edgeDataList[0][i].y, edgeDataList[0][i].z, edgeDataList[3][i].x, edgeDataList[3][i].y, edgeDataList[3][i].z, rgba).points;
            pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        }
        for (int i = 0; i < edgeDataList[0].size(); i++) {
            temp = drawLine(edgeDataList[0][i].x, edgeDataList[0][i].y, edgeDataList[0][i].z, edgeDataList[4][i].x, edgeDataList[4][i].y, edgeDataList[4][i].z, rgba).points;
            pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        }
        for (int i = 0; i < edgeDataList[3].size(); i++) {
            temp = drawLine(edgeDataList[3][i].x, edgeDataList[3][i].y, edgeDataList[3][i].z, edgeDataList[7][i].x, edgeDataList[7][i].y, edgeDataList[7][i].z, rgba).points;
            pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        }
        for (int i = 0; i < edgeDataList[4].size(); i++) {
            temp = drawLine(edgeDataList[4][i].x, edgeDataList[4][i].y, edgeDataList[4][i].z, edgeDataList[7][i].x, edgeDataList[7][i].y, edgeDataList[7][i].z, rgba).points;
            pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        }
        for (int i = 0; i < edgeDataList[8].size(); i++) {
            temp = drawLine(edgeDataList[8][i].x, edgeDataList[8][i].y, edgeDataList[8][i].z, edgeDataList[10][i].x, edgeDataList[10][i].y, edgeDataList[10][i].z, rgba).points;
            pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        }
        for (int i = 0; i < edgeDataList[9].size(); i++) {
            temp = drawLine(edgeDataList[9][i].x, edgeDataList[9][i].y, edgeDataList[9][i].z, edgeDataList[11][i].x, edgeDataList[11][i].y, edgeDataList[11][i].z, rgba).points;
            pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        }
        Object obj(pointsList, rgba, true);
        return obj;
    }

    //Draws a circle centered at (x, y, z) with radius r and angles (roll, pitch, yaw)
    Object drawCircle(const double x, const double y, const double z, const double r, const Color rgba, double roll = 0.0, double pitch = 0.0, double yaw = 0.0) {
        std::vector<Point> points;
        std::vector<Point> pointsList;
        std::vector<Point> temp;

        int tnp = 2*PI*r;
        int onp = tnp/8;
        double interval = 1/r;
        double theta = 0;

        for (int i = 0; i < onp; i++){
            theta += interval;
            points.push_back({cos(theta)*r, sin(theta)*r, 0});
        }

        int len = points.size();

        std::vector<Point> fPoints;
        for (int i = 0; i < len; i++) {
            Point pnt = points[i];
            fPoints.push_back({pnt.x, pnt.y, 0});
            fPoints.push_back({pnt.x, -pnt.y, 0});
            fPoints.push_back({-pnt.x, pnt.y, 0});
            fPoints.push_back({-pnt.x, -pnt.y, 0});
            fPoints.push_back({pnt.y, pnt.x, 0});
            fPoints.push_back({-pnt.y, pnt.x, 0});
            fPoints.push_back({pnt.y, -pnt.x, 0});
            fPoints.push_back({-pnt.y, -pnt.x, 0});
        }

        //Converts from degrees to radians
        roll *= convRate;
        pitch *= convRate;
        yaw *= convRate;

        //Matrix for roll
        Matrix rMat = {{
            {1.0, 0.0, 0.0},
            {0.0, cos(roll), -sin(roll)},
            {0.0, sin(roll), cos(roll)}
        }};
        //Matrix for pitch
        Matrix pMat = {{
            {cos(pitch), 0.0, sin(pitch)},
            {0.0, 1.0, 0.0},
            {-sin(pitch), 0.0, cos(pitch)}
        }};
        //Matrix for yaw
        Matrix yMat = {{
            {cos(yaw), -sin(yaw), 0.0},
            {sin(yaw), cos(yaw), 0.0},
            {0.0, 0.0, 1.0}
        }};
        //Array of matrices
        std::array<Matrix, 3> matArr = {rMat, pMat, yMat};

        for (int i = 0; i < fPoints.size(); i++) {
            Point pnt = fPoints[i];
            for (Matrix mat : matArr) {
                pnt = matMult(pnt, mat);
            }
            pnt.x += x;
            pnt.y += y;
            pnt.z += z;
            fPoints[i] = {round(pnt.x), round(pnt.y), round(pnt.z)};
        }

        for (int i = 0; i < (fPoints.size()/8)-1; i++) {
            temp = drawLine(fPoints[(i*8)].x, fPoints[(i*8)].y, fPoints[(i*8)].z, fPoints[((i+1)*8)].x+1, fPoints[((i+1)*8)].y+1, fPoints[((i+1)*8)].z+1, rgba).points;
            pointsList.insert(pointsList.end(), temp.begin(), temp.end());
            temp = drawLine(fPoints[(i*8)+1].x, fPoints[(i*8)+1].y, fPoints[(i*8)+1].z, fPoints[((i+1)*8)+1].x+1, fPoints[((i+1)*8)+1].y+1, fPoints[((i+1)*8)+1].z+1, rgba).points;
            pointsList.insert(pointsList.end(), temp.begin(), temp.end());
            temp = drawLine(fPoints[(i*8)+2].x, fPoints[(i*8)+2].y, fPoints[(i*8)+2].z, fPoints[((i+1)*8)+2].x+1, fPoints[((i+1)*8)+2].y+1, fPoints[((i+1)*8)+2].z+1, rgba).points;
            pointsList.insert(pointsList.end(), temp.begin(), temp.end());
            temp = drawLine(fPoints[(i*8)+3].x, fPoints[(i*8)+3].y, fPoints[(i*8)+3].z, fPoints[((i+1)*8)+3].x+1, fPoints[((i+1)*8)+3].y+1, fPoints[((i+1)*8)+3].z+1, rgba).points;
            pointsList.insert(pointsList.end(), temp.begin(), temp.end());
            temp = drawLine(fPoints[(i*8)+4].x, fPoints[(i*8)+4].y, fPoints[(i*8)+4].z, fPoints[((i+1)*8)+4].x+1, fPoints[((i+1)*8)+4].y+1, fPoints[((i+1)*8)+4].z+1, rgba).points;
            pointsList.insert(pointsList.end(), temp.begin(), temp.end());
            temp = drawLine(fPoints[(i*8)+5].x, fPoints[(i*8)+5].y, fPoints[(i*8)+5].z, fPoints[((i+1)*8)+5].x+1, fPoints[((i+1)*8)+5].y+1, fPoints[((i+1)*8)+5].z+1, rgba).points;
            pointsList.insert(pointsList.end(), temp.begin(), temp.end());
            temp = drawLine(fPoints[(i*8)+6].x, fPoints[(i*8)+6].y, fPoints[(i*8)+6].z, fPoints[((i+1)*8)+6].x+1, fPoints[((i+1)*8)+6].y+1, fPoints[((i+1)*8)+6].z+1, rgba).points;
            pointsList.insert(pointsList.end(), temp.begin(), temp.end());
            temp = drawLine(fPoints[(i*8)+7].x, fPoints[(i*8)+7].y, fPoints[(i*8)+7].z, fPoints[((i+1)*8)+7].x+1, fPoints[((i+1)*8)+7].y+1, fPoints[((i+1)*8)+7].z+1, rgba).points;
            pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        }
        Object obj(pointsList, rgba, true);
        return obj;
    }

    //Draws a sphere centered at (x, y, z) with radius r
    Object drawSphere(const double x, const double y, const double z, const double r, const Color rgba) {
        int tnp = 2*PI*r;
        double interval = 180/(r*PI);
        double theta = 0;
        std::vector<Point> pointsList;
        std::vector<Point> temp;

        for (int i = 0; i < tnp/2; i++){
            theta += interval;
            temp = drawCircle(x, y, z, r, rgba, theta).points;
            pointsList.insert(pointsList.end(), temp.begin(), temp.end());
        }
        Object obj(pointsList, rgba, true);
        return obj;
    }

    //Returns values for a line
    std::vector<Point> drawBorderLine(const double x0, const double y0, const double z0, const double x1, const double y1, const double z1) {
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
        if (dx >= dy && dx >= dz) np = (int) dx;
        else if (dy >= dx && dy >= dz) np = (int) dy;
        else np = (int) dz;

        //Distances between points
        double vx = dx / np * xs;
        double vy = dy / np * ys;
        double vz = dz / np * zs;


        //Get each point
        std::vector<Point> points(np);
        for (int i = 0; i < np; i++) {
            points[i] = {x0 + i*vx, y0 + i*vy, z0 + i*vz};
        }

        return points;
    }

    //Returns border values
    std::array<Point, 8> viewBorder(const double x, const double y, const double z, double roll = 0.0, double pitch = 0.0, double yaw = 0.0) {
        std::array<Point, 8> localPointList = {{
            {-WIDTH/2, -HEIGHT/2, -DEPTH/2},
            {-WIDTH/2, HEIGHT/2, -DEPTH/2},
            {-WIDTH/2, -HEIGHT/2, DEPTH/2},
            {-WIDTH/2, HEIGHT/2, DEPTH/2},
            {WIDTH/2, -HEIGHT/2, -DEPTH/2},
            {WIDTH/2, HEIGHT/2, -DEPTH/2},
            {WIDTH/2, -HEIGHT/2, DEPTH/2},
            {WIDTH/2, HEIGHT/2, DEPTH/2}
        }};

        //Converting to radians
        roll *= convRate;
        pitch *= convRate;
        yaw *= convRate;

        std::array<Point, 8> vertexList;

        //Matrix for roll
        Matrix rMat = {{
            {1.0, 0.0, 0.0},
            {0.0, cos(roll), -sin(roll)},
            {0.0, sin(roll), cos(roll)}
        }};
        //Matrix for pitch
        Matrix pMat = {{
            {cos(pitch), 0.0, sin(pitch)},
            {0.0, 1.0, 0.0},
            {-sin(pitch), 0.0, cos(pitch)}
        }};
        //Matrix for yaw
        Matrix yMat = {{
            {cos(yaw), -sin(yaw), 0.0},
            {sin(yaw), cos(yaw), 0.0},
            {0.0, 0.0, 1.0}
        }};
        //Array of matrices
        std::array<Matrix, 3> matArr = {rMat, pMat, yMat};

        for (int i = 0; i < size(localPointList); i++) {
            Point pnt = localPointList[i];
            for (Matrix mat : matArr) {
                pnt = matMult(pnt, mat);
            }
            pnt.x += x + WIDTH/2;
            pnt.y += y + HEIGHT/2;
            pnt.z += z + DEPTH/2;
            vertexList[i] = {round(pnt.x), round(pnt.y), round(pnt.z)};
        }
        return vertexList;
    }

    //Gets the distance from a point to the line along the view frame
    Point getDistance(const std::array<Point, 8> viewFrame, const double x1, const double y1, const double z1) {
        double x, y, z;
        const double mx = (viewFrame[0].z - viewFrame[4].z)/(viewFrame[0].x - viewFrame[4].x);
        const double my = (viewFrame[0].x - viewFrame[1].x)/(viewFrame[0].y - viewFrame[1].y);
        const double mz = (viewFrame[0].y - viewFrame[2].y)/(viewFrame[0].z - viewFrame[2].z);

        y = -(my*x1-y1+viewFrame[0].y-my*viewFrame[0].x) / std::sqrt(my*my+1);
        z = -(mz*y1-z1+viewFrame[0].z-mz*viewFrame[0].y) / std::sqrt(mz*mz+1);
        x = -(mx*z1-x1+viewFrame[0].x-mx*viewFrame[0].z) / std::sqrt(mx*mx+1);
        return {x, y, z};
    }

    //Convert the 3D data to 2D
    void projToFlat(const double x, const double y, const double z, std::map<std::string, Object> *objectList, double roll = 0.0, double pitch = 0.0, double yaw = 0.0) {
        std::array<Point, 8> viewFrame = viewBorder(x, y, z, roll, pitch, yaw);
        for(const auto& [key, obj] : *objectList) {
            if (obj.rendered) {
                for (Point pnt : obj.points) {
                    Point p = getDistance(viewFrame, pnt.x, pnt.y, pnt.z);
                    double ix = (int) p.x;
                    double iy = (int) p.y;
                    double iz = (int) p.z;
                    //Float parts of point
                    double fx = p.x - ix;
                    double fy = p.y - iy;
                    double fz = p.z - iz;
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
                    #ifndef AA
                        drawAbsolute({ix-1, iy-1, iz-1}, obj.rgba);
                        drawAbsolute({ix, iy-1, iz-1}, obj.rgba);
                        drawAbsolute({ix+1, iy-1, iz-1}, obj.rgba);
                        drawAbsolute({ix-1, iy, iz-1}, obj.rgba);
                        drawAbsolute({ix, iy, iz-1}, obj.rgba);
                        drawAbsolute({ix+1, iy, iz-1}, obj.rgba);
                        drawAbsolute({ix-1, iy+1, iz-1}, obj.rgba);
                        drawAbsolute({ix, iy+1, iz-1}, obj.rgba);
                        drawAbsolute({ix+1, iy+1, iz-1}, obj.rgba);
                        drawAbsolute({ix-1, iy-1, iz}, obj.rgba);
                        drawAbsolute({ix, iy-1, iz}, obj.rgba);
                        drawAbsolute({ix+1, iy-1, iz}, obj.rgba);
                        drawAbsolute({ix-1, iy, iz}, obj.rgba);
                        drawAbsolute({ix, iy, iz}, obj.rgba);
                        drawAbsolute({ix+1, iy, iz}, obj.rgba);
                        drawAbsolute({ix-1, iy+1, iz}, obj.rgba);
                        drawAbsolute({ix, iy+1, iz}, obj.rgba);
                        drawAbsolute({ix+1, iy+1, iz}, obj.rgba);
                        drawAbsolute({ix-1, iy-1, iz+1}, obj.rgba);
                        drawAbsolute({ix, iy-1, iz+1}, obj.rgba);
                        drawAbsolute({ix+1, iy-1, iz+1}, obj.rgba);
                        drawAbsolute({ix-1, iy, iz+1}, obj.rgba);
                        drawAbsolute({ix, iy, iz+1}, obj.rgba);
                        drawAbsolute({ix+1, iy, iz+1}, obj.rgba);
                        drawAbsolute({ix-1, iy+1, iz+1}, obj.rgba);
                        drawAbsolute({ix, iy+1, iz+1}, obj.rgba);
                        drawAbsolute({ix+1, iy+1, iz+1}, obj.rgba);
                    #else
                        drawAbsolute({ix-1, iy-1, iz-1}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix, iy-1, iz-1}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix+1, iy-1, iz-1}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix-1, iy, iz-1}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix, iy, iz-1}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix+1, iy, iz-1}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix-1, iy+1, iz-1}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix, iy+1, iz-1}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix+1, iy+1, iz-1}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix-1, iy-1, iz}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix, iy-1, iz}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix+1, iy-1, iz}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix-1, iy, iz}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix, iy, iz}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix+1, iy, iz}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix-1, iy+1, iz}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix, iy+1, iz}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix+1, iy+1, iz}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix-1, iy-1, iz+1}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix, iy-1, iz+1}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix+1, iy-1, iz+1}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix-1, iy, iz+1}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix, iy, iz+1}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix+1, iy, iz+1}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix-1, iy+1, iz+1}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix, iy+1, iz+1}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                        drawAbsolute({ix+1, iy+1, iz+1}, obj.rgba * ((cx0 + cy0 + cz0) / 3.0));
                    #endif
                }
            } else {
                continue;
            }
        }

        for (int i = 0; i < DEPTH; i++) {
            for (int j = 0; j < HEIGHT; j++) {
                for (int k = 0; k < WIDTH; k++) {
                    if (flattenedPixels[flattenedIndex(k, j)] == EMPTY_COLOR) {
                        #ifndef NO_PROP_A
                            drawFlat(k, j, absolutePixels[absoluteIndex(k, j, i)] * (1 - (static_cast<double>(i)/DEPTH)));
                        #else
                            drawFlat(k, j, absolutePixels[absoluteIndex(k, j, i)]);
                        #endif
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
        return &flattenedBytes[0];
    }
}