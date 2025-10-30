//#define NO_PROP_A
#include "open_render_with_STL.hpp"

int main() {
    //List of all objects
    std::map<std::string, object> objectList;
    objectList.insert({"recFill", drawRecPrFill(400.0, 200.0, 200.0, 300.0, 200.0, 200.0, {244, 67, 41, 255}, 70.0, 190.0, 10.0)});
    objectList.insert({"line", drawLine(10.0, 352.0, 0.0, 500.0, 0.0, 250.0, {255, 255, 255, 255})});
    objectList.insert({"sphere", drawSphere(500, 200, 200, 100, {255, 255, 255, 255})});
    objectList.insert({"triangle", drawTriangle(600, 200, 0, 250, 350, 200, 234, 100, 150, {50, 0, 255, 255})});
    char const *filename = "filename.png";
    projToFlat(0.0, 0.0, 0.0, &objectList, 0, 0, 35);
    stbi_write_png(filename, WIDTH, HEIGHT, 4, convToBytes(), 0);

    return 0;
}