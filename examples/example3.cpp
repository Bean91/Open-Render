#define AA
#include "../open_render.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main() {
    //List of all objects
    std::map<std::string, openrender::object> objectList;
    objectList.insert({"recFill0", openrender::drawRecPrFill(200.0, 200.0, 200.0, 200.0, 200.0, 200.0, {255, 255, 255, 255})});
    objectList.insert({"recFill1", openrender::drawRecPrFill(600.0, 200.0, 200.0, 100.0, 100.0, 100.0, {0, 0, 255, 255}, 30, 20, 30)});

    openrender::projToFlat(-200.0, 200.0, -200.0, &objectList, 45, 45, 45);
    //Your own image writer
    makeImage("example3.png", WIDTH, HEIGHT, 4, openrender::convToBytes(), 0);

    return 0;
}