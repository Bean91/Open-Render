#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <map>
#include <set>
#include <cstdlib>
#include <cmath>

using std::cout, std::cin, std::endl;

//CONFIG
constexpr int WIDTH = 320;
constexpr int HEIGHT = 240;
std::vector<uint32_t> framebuffer(WIDTH * HEIGHT, 0xFF000000); // ARGB: black

void draw_pixel(int x, int y, uint32_t color) {
    if (x > WIDTH || x < 0 || y < 0 || y > HEIGHT)
        return;
    framebuffer[y * WIDTH + x] = color;
}
void draw_filled_rect(int x, int y, int w, int h, uint32_t color) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            draw_pixel(x + j, y + i, color);
        }
    }
}
void draw_rect(int x, int y, int w, int h, int lW = 1, uint32_t color) {
    for (int i = 0; i < h; i++) {
        if (i == 0 || i == h - 1) {
            for (int j = 0; j < w; j++) {
                draw_pixel(x + j, y + i, color);
            }
        } else {
            for (int j = 0; j < lW; j++) {
                draw_pixel(x + j, y + i, color);
                draw_pixel(x + w - j - 1, y + i, color);
            }
        }
    }
}
void draw_line(int xS, int yS, int xE, int yE, int lW = 1, uint32_t color) {
    int dx = abs(xE - xS);
    int dy = abs(yE - yS);
    int sx = xS < xE ? 1 : -1;
    int sy = yS < yE ? 1 : -1;
    int err = dx - dy;

    while (true) {
        draw_pixel(xS, yS, color);
        if (xS == xE && yS == yE) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; xS += sx; }
        if (e2 < dx)  { err += dx; yS += sy; }
    }
}
void draw_filled_circle(int x, int y, int r, uint32_t color) {
    for (int i = -r; i < r; i++) {
        if ((x - i) > 0 && (x + i) < WIDTH) {
            int mY = std::sqrt((r * r) - (i * i));
            for (int j = -mY; j < mY; j++) {
                if ((y - j) > 0 && (y + j) < HEIGHT) {
                    draw_pixel(x + i, y + j, color);
                }
            }
        }
    }
}