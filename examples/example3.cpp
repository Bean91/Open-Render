#define AA
#include "../open_render.hpp"

int main() {
    //List of all objects
    std::map<std::string, openrender::Object> objectList;
    objectList.insert({"recFill0", openrender::drawRecPrFill(200.0, 200.0, 200.0, 200.0, 200.0, 200.0, {255, 255, 255, 255})});
    objectList.insert({"recFill1", openrender::drawRecPrFill(600.0, 200.0, 200.0, 100.0, 100.0, 100.0, {0, 0, 255, 255}, 30, 20, 30)});

    openrender::projToFlat(-200.0, 200.0, -200.0, &objectList, 45, 45, 45);
    //Your own image writer
    makeImage("example3.png", WIDTH, HEIGHT, 4, openrender::convToBytes(), 0);

    return 0;
}