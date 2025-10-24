#include "open_render.hpp"
#include <cstdint>
#include <fstream>

void displaySTL(std::string filepath, color rgba) {
    std::ifstream input(filepath, std::ios::binary);
    input.seekg(80);
    uint32_t triangleNum;
    input >> triangleNum;
    for (int i = 0; i < triangleNum; i++) {
        input.seekg(3, std::ifstream::cur);
        std::array<float, 9> vertexList;
        input >> vertexList[0] >> vertexList[1] >> vertexList[2]
         >> vertexList[3] >> vertexList[4] >> vertexList[5]
         >> vertexList[6] >> vertexList[7] >> vertexList[8];
        drawTriangle(vertexList[0], vertexList[1], vertexList[2],
                     vertexList[3], vertexList[4], vertexList[5],
                     vertexList[6], vertexList[7], vertexList[8], rgba);
        input.seekg(2, std::ifstream::cur);
    }
}