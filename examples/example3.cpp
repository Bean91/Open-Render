#define AA
#include "../open_render.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main() {
    //List of all objects
    std::map<std::string, object> objectList;
    objectList.insert({"recFill0", drawRecPrFill(200.0, 200.0, 200.0, 200.0, 200.0, 200.0, {255, 255, 255, 255})});
    objectList.insert({"recFill1", drawRecPrFill(600.0, 200.0, 200.0, 100.0, 100.0, 100.0, {0, 0, 255, 255}, 30, 20, 30)});

    projToFlat(-200.0, 200.0, -200.0, &objectList, 45, 45, 45);
    stbi_write_png("example3.png", WIDTH, HEIGHT, 4, convToBytes(), 0);

    return 0;
}