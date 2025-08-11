#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <fstream>
#include "fonts.hpp"

using std::cout, std::endl;

// CONFIG
constexpr int WIDTH = 1920;
constexpr int HEIGHT = 1080;
std::vector<uint32_t> frameBuffer(WIDTH * HEIGHT, 0xFF000000);
struct UnicodeRange {
    uint32_t start;
    uint32_t end;
    uint16_t indexOffset;
};

const UnicodeRange fontRanges[] = {
    {0x0000, 0x00FF, 0},
    {0x0390, 0x03C9, 256},
    {0x2500, 0x259F, 320},
    {0x3040, 0x309F, 384},
    {0xE541, 0xE55A, 448}
};
const int numRanges = sizeof(fontRanges) / sizeof(fontRanges[0]);

void save_ppm(const std::string& filename, const std::vector<uint32_t>& framebuffer, int width, int height) {
    std::ofstream ofs(filename, std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (int i = 0; i < width * height; i++) {
        uint32_t pixel = framebuffer[i];
        uint8_t r = (pixel >> 16) & 0xFF;
        uint8_t g = (pixel >> 8) & 0xFF;
        uint8_t b = pixel & 0xFF;
        ofs.put(r);
        ofs.put(g);
        ofs.put(b);
    }
    ofs.close();
}

uint32_t blend_colors(uint32_t dst, uint32_t src) {
    uint8_t src_a = (src >> 24) & 0xFF;
    uint8_t src_r = (src >> 16) & 0xFF;
    uint8_t src_g = (src >> 8) & 0xFF;
    uint8_t src_b = src & 0xFF;
    uint8_t dst_r = (dst >> 16) & 0xFF;
    uint8_t dst_g = (dst >> 8) & 0xFF;
    uint8_t dst_b = dst & 0xFF;
    float alpha = src_a / 255.0f;
    uint8_t out_r = static_cast<uint8_t>(src_r * alpha + dst_r * (1 - alpha));
    uint8_t out_g = static_cast<uint8_t>(src_g * alpha + dst_g * (1 - alpha));
    uint8_t out_b = static_cast<uint8_t>(src_b * alpha + dst_b * (1 - alpha));
    uint8_t out_a = 255;
    return (out_a << 24) | (out_r << 16) | (out_g << 8) | out_b;
}

void draw_pixel(int x, int y, uint32_t color, float alpha = 1.0f) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    uint8_t src_a = (color >> 24) & 0xFF;
    uint32_t mod_color = ((uint8_t)(src_a * alpha) << 24) | (color & 0x00FFFFFF);
    frameBuffer[y * WIDTH + x] = blend_colors(frameBuffer[y * WIDTH + x], mod_color);
}

void draw_filled_rect(int x, int y, int w, int h, uint32_t color) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            draw_pixel(x + j, y + i, color);
        }
    }
}

void draw_rect(int x, int y, int w, int h, int lW = 1, uint32_t color = 0xFF000000) {
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

void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
    int x = x0;
    int y = y0;
    float err = 0;
    uint32_t alphaColor;

    const float slope = (y1 - y0) / (x1 - x0);
    const int dx = std::abs(x0 - x1);
    const int dy = std::abs(y0 - y1);

    while (true) {
        if (x == x1 && y = y1) return;

        alphaColor |= static_cast<uint32_t>(color << 0) << 0;
        alphaColor |= static_cast<uint32_t>(color << 8) << 8;
        alphaColor |= static_cast<uint32_t>(color << 16) << 16;
        alphaColor |= static_cast<uint32_t>(alpha) << 24;
    }
}

void draw_circle(int cx, int cy, float r, int lW, uint32_t color) {
    int minX = std::max(0, static_cast<int>(cx - r - 1));
    int maxX = std::min(WIDTH - 1, static_cast<int>(cx + r + 1));
    int minY = std::max(0, static_cast<int>(cy - r - 1));
    int maxY = std::min(HEIGHT - 1, static_cast<int>(cy + r + 1));
    float outer_r = r + lW / 2.0f;
    float inner_r = r - lW / 2.0f;
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            float dist = std::hypot(x - cx, y - cy);
            if (dist <= outer_r && dist >= inner_r) {
                float alpha = 1.0f - std::min(std::abs(dist - r) / (lW / 2.0f), 1.0f);
                draw_pixel(x, y, color, alpha);
            }
        }
    }
}

void draw_filled_circle(int cx, int cy, int r, uint32_t color) {
    if (r <= 0) return;
    int x = r;
    int y = 0;
    int err = 1 - x;
    while (x >= y) {
        if (cy + y >= 0 && cy + y < HEIGHT) {
            int x_start = std::max(cx - x, 0);
            int x_end = std::min(cx + x, WIDTH - 1);
            for (int px = x_start; px <= x_end; px++) {
                draw_pixel(px, cy + y, color);
            }
        }
        if (cy - y >= 0 && cy - y < HEIGHT) {
            int x_start = std::max(cx - x, 0);
            int x_end = std::min(cx + x, WIDTH - 1);
            for (int px = x_start; px <= x_end; px++) {
                draw_pixel(px, cy - y, color);
            }
        }
        if (cy + x >= 0 && cy + x < HEIGHT) {
            int x_start = std::max(cx - y, 0);
            int x_end = std::min(cx + y, WIDTH - 1);
            for (int px = x_start; px <= x_end; px++) {
                draw_pixel(px, cy + x, color);
            }
        }
        if (cy - x >= 0 && cy - x < HEIGHT) {
            int x_start = std::max(cx - y, 0);
            int x_end = std::min(cx + y, WIDTH - 1);
            for (int px = x_start; px <= x_end; px++) {
                draw_pixel(px, cy - x, color);
            }
        }
        y++;
        if (err < 0) {
            err += 2 * y + 1;
        } else {
            x--;
            err += 2 * (y - x + 1);
        }
    }
}

void draw_polygon(std::vector<std::pair<int, int>> coordList, int lW, uint32_t color) {
    for (int i = 0; i < (int)coordList.size() - 1; i++) {
        draw_line(coordList[i].first, coordList[i].second, coordList[i + 1].first, coordList[i + 1].second, color);
    }
    if (!coordList.empty()) {
        draw_line(coordList.back().first, coordList.back().second, coordList[0].first, coordList[0].second, color);
    }
}

void draw_filled_polygon(const std::vector<std::pair<int,int>>& points, uint32_t color) {
    if (points.size() < 3) return;
    int minY = HEIGHT, maxY = 0;
    for (auto& p : points) {
        minY = std::min(minY, p.second);
        maxY = std::max(maxY, p.second);
    }
    minY = std::max(minY, 0);
    maxY = std::min(maxY, HEIGHT-1);
    for (int y = minY; y <= maxY; ++y) {
        std::vector<int> intersections;
        for (size_t i = 0; i < points.size(); ++i) {
            auto p1 = points[i];
            auto p2 = points[(i+1) % points.size()];
            if ((p1.second <= y && p2.second > y) || (p2.second <= y && p1.second > y)) {
                int x = p1.first + (y - p1.second) * (p2.first - p1.first) / (p2.second - p1.second);
                intersections.push_back(x);
            }
        }
        std::sort(intersections.begin(), intersections.end());
        for (size_t i = 0; i + 1 < intersections.size(); i += 2) {
            int xStart = std::max(intersections[i], 0);
            int xEnd = std::min(intersections[i + 1], WIDTH - 1);
            for (int x = xStart; x <= xEnd; ++x) {
                draw_pixel(x, y, color);
            }
        }
    }
}

int unicode_to_font_index(uint32_t codepoint) {
    for (int i = 0; i < numRanges; ++i) {
        if (codepoint >= fontRanges[i].start && codepoint <= fontRanges[i].end) {
            return fontRanges[i].indexOffset + (codepoint - fontRanges[i].start);
        }
    }
    return -1;
}

uint32_t utf8_to_codepoint(const char* utf8) {
    const uint8_t* bytes = (const uint8_t*)utf8;
    if (bytes[0] < 0x80) return bytes[0];
    else if ((bytes[0] & 0xE0) == 0xC0) return ((bytes[0] & 0x1F) << 6) | (bytes[1] & 0x3F);
    else if ((bytes[0] & 0xF0) == 0xE0) return ((bytes[0] & 0x0F) << 12) | ((bytes[1] & 0x3F) << 6) | (bytes[2] & 0x3F);
    else if ((bytes[0] & 0xF8) == 0xF0) return ((bytes[0] & 0x07) << 18) | ((bytes[1] & 0x3F) << 12)
                                                | ((bytes[2] & 0x3F) << 6) | (bytes[3] & 0x3F);
    return 0xFFFD;
}

void draw_char(int x, int y, const uint8_t* bitmap, uint32_t color, int scale = 1) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            bool pixelOn = (bitmap[row] & (1 << (col))) != 0;
            if (pixelOn) {
                for (int dy = 0; dy < scale; ++dy) {
                    for (int dx = 0; dx < scale; ++dx) {
                        draw_pixel(x + col * scale + dx, y + row * scale + dy, color);
                    }
                }
            }
        }
    }
}

void draw_text(int x, int y, const std::string& text, uint32_t color, int scale = 1, int spacing = 2, int lineHeight = 10) {
    int cursor_x = x;
    int cursor_y = y;
    const char* ptr = text.c_str();
    while (*ptr) {
        if (*ptr == '\n') {
            cursor_x = x;
            cursor_y += (8 * scale) + lineHeight;
            ++ptr;
            continue;
        }
        uint32_t codepoint = utf8_to_codepoint(ptr);
        int index = unicode_to_font_index(codepoint);
        if (index >= 0) {
            draw_char(cursor_x, cursor_y, fonts[index], color, scale); // note: passing bitmap directly
            cursor_x += 8 * scale + spacing;
        }
        uint8_t firstByte = static_cast<uint8_t>(*ptr);
        if (firstByte < 0x80) ptr += 1;
        else if ((firstByte & 0xE0) == 0xC0) ptr += 2;
        else if ((firstByte & 0xF0) == 0xE0) ptr += 3;
        else if ((firstByte & 0xF8) == 0xF0) ptr += 4;
        else ptr += 1;
    }
}


int main() {
    // Clear framebuffer to white
    std::fill(frameBuffer.begin(), frameBuffer.end(), 0xFFFFFFFF);

    // Draw a big filled red circle in center
    draw_filled_circle(WIDTH / 2, HEIGHT / 2, 200, 0xFFFF4444);

    // Add black anti-aliased circle outline
    draw_circle_aa(WIDTH / 2, HEIGHT / 2, 200.0f, 4, 0xFF000000);

    // Draw a green rectangle left of the circle
    draw_filled_rect(200, 400, 150, 300, 0xFF00DD00);
    draw_rect(200, 400, 150, 300, 3, 0xFF003300);

    // Draw a magenta triangle overlapping the circle
    std::vector<std::pair<int, int>> triangle = {
        {WIDTH / 2, HEIGHT / 2 - 150},
        {WIDTH / 2 - 130, HEIGHT / 2 + 150},
        {WIDTH / 2 + 130, HEIGHT / 2 + 150}
    };
    draw_filled_polygon(triangle, 0xFFFF00FF);
    draw_polygon(triangle, 2, 0xFF000000);

    // Diagonal anti-aliased line across screen
    draw_line(0, 0, WIDTH - 1, HEIGHT - 1, 0xFF2222FF);

    // Draw some circles across the top with varying sizes
    for (int i = 0; i < 10; ++i) {
        int r = 10 + i * 5;
        int cx = 100 + i * 150;
        draw_filled_circle(cx, 80, r, 0xFF000000 + (i * 0x00112233));
        draw_circle_aa(cx, 80, r, 2, 0xFFFFFFFF);
    }

    std::vector<std::pair<int, int>> pentagon = {
        {300, 100},
        {350, 150},
        {325, 210},
        {275, 210},
        {250, 150}
    };
    draw_filled_polygon(pentagon, 0xFF00AAFF); // blue fill
    draw_polygon(pentagon, 2, 0xFF000000);     // black outline

    // Hardcoded Dodecagon (12 sides, roughly circular)
    std::vector<std::pair<int, int>> dodecagon = {
        {800, 100}, {820, 115}, {835, 135},
        {840, 160}, {835, 185}, {820, 205},
        {800, 220}, {780, 205}, {765, 185},
        {760, 160}, {765, 135}, {780, 115}
    };
    draw_filled_polygon(dodecagon, 0xFFFFAA00); // orange fill
    draw_polygon(dodecagon, 2, 0xFF000000);

    draw_text(50, 50, "Hello, World!", 0xFF000000, 1); // normal size
    draw_text(50, 80, "Big Text!", 0xFF0000FF, 3);
    draw_text(500, 500, "Really Big Text!", 0xFF0000FF, 10);

    // Save to file
    save_ppm("output.ppm", frameBuffer, WIDTH, HEIGHT);
    std::cout << "Image written to output.ppm" << std::endl;

    return 0;
}
