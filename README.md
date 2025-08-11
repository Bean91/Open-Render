# OpenRender

**OpenRender** is a minimal, zero-dependency software rasterizer written in C++. It's designed for learning, tinkering, or just rendering cool stuff on the CPU like it's 1997.

No OpenGL. No SDL. No DirectX.
Just C++ and raw pixel-pushing.

---

## File Structure

```
.
├── main.cpp    # The whole shebang: rendering logic, geometry, and demo
```

---

## Requirements
- A C++ compiler (G++ or Clang++ recommended)

C++17 or later

No external dependencies

## Build & Run
### Compile
```bash
g++ -std=c++17 -O2 main.cpp -o openrender
```

## Run
```bash
./openrender
```

This generates a `test_output.ppm` file in the same directory. You can view it with most image viewers or convert it using `ImageMagick`:

```bash
convert test_output.ppm test_output.png
```

---

## Features (So Far)
- [x] Anti-aliased lines (Wu's algorithm)
- [x] Filled & outlined polygons
- [x] Filled & outlined circles
- [x] Axis-aligned rectangles
- [x] Solid color rendering
- [x] 32-bit RGBA pixel buffer
- [x] Writes directly to .ppm image format

## Roadmap
- [ ] Text rendering

- [ ] Gradient fills

- [ ] Alpha blending

- [ ] Transform pipelines

- [ ] Z-buffer / layering

- [ ] Texture mapping

- [ ] Vector-to-raster SVG-style rendering

## Why?
Because modern graphics APIs are massive and sometimes you just want to draw a triangle without summoning the GPU gods. This project is great for:

- Learning 2D rendering algorithms

- Tinkering with geometry

- Making old-school demos

- Gaining low-level graphics understanding

## Contributing
Right now it's just main.cpp, but feel free to fork, hack, and share ideas. A future modular structure is planned as features grow.

## License
This project is licensed under the Apache License 2.0 — see the full LICENSE file for details.

TL;DR: You can use, modify, and distribute it freely. Just don’t remove our names or sue us if it breaks.

## Credits
Inspired by Bresenham, Wu, and every nerd who ever plotted a pixel with love.
Fonts from [this repo](https://github.com/dhepper/font8x8)