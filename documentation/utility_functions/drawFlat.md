```cpp
void drawFlat(int x, int y, Color rgba);
```
---
## Basic Info
Draws a pixel in the flattened 2D view.

### Parameters
| Parameter | Type                              | Use                   | Optional |
|-----------|-----------------------------------|-----------------------|----------|
| `x`       | `int`                             | X Coordinate of pixel | ❌        |
| `y`       | `int`                             | Y Coordinate of pixel | ❌        |
| `rgba`    | [`Color`](../structures_and_types/color.md) | Color of pixel        | ❌        |
---
## Example

```cpp
const int x = 42;
const int y = 350;
openrender::Color rgba = {35, 255, 150, 135};
// Calling the drawFlat() function
drawFlat(x, y, rgba);
```