//#define NO_PROP_A
#include "open_render.hpp"

int main() {
    drawRecPrFill(400.0, 200.0, 200.0, 100.0, 100.0, 100.0, 30.0, 40.0, 60.0, {255, 255, 255, 255});
    drawRecPrOut(400.0, 200.0, 200.0, 300.0, 200.0, 200.0, 70.0, 190.0, 10.0, {244, 67, 41, 255});
    drawLine(10.0, 352.0, 0.0, 500.0, 0.0, 250.0, {255, 255, 255, 255});

    projAbsToFlat(0.0, 0.0, 0.0);

    char const *filename = "filename.png";
    stbi_write_png(filename, WIDTH, HEIGHT, 4, convToBytes(), 0);
    return 0;
}