#include "../open_render.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main() {
    //List of all objects
    std::map<std::string, object> objectList;
    objectList.insert({"recFill", drawRecPrFill(200.0, 200.0, 200.0, 200.0, 200.0, 200.0, {255, 255, 255, 255}, 45, 45, 45)});

    projToFlat(0.0, 0.0, 0.0, &objectList, 0, 0, 0);
    //Your own image writer
    makeImage("example1.png", WIDTH, HEIGHT, 4, convToBytes(), 0);

    return 0;
}