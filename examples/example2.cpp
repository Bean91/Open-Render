#include "../open_render.hpp"

int main() {
    //List of all objects
    std::map<std::string, openrender::Object> objectList;
    objectList.insert({"recFill", openrender::drawRecPrFill(200.0, 200.0, 200.0, 200.0, 200.0, 200.0, {255, 255, 255, 255})});

    openrender::projToFlat(0.0, 0.0, 0.0, &objectList, 45, 45, 45);
    //Your own image writer
    makeImage("example2.png", WIDTH, HEIGHT, 4, openrender::convToBytes(), 0);

    return 0;
}