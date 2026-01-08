```cpp
void drawAbsolute(Point pnt, Color rgba);
```
---
## Basic Info
Draws a pixel in absolute space. Combines colors if pixel already has a color.

### Parameters
| Parameter | Type                              | Use                  | Optional |
|-----------|-----------------------------------|----------------------|----------|
| `pnt`     | [`Point`](../structures_and_types/point.md) | Coordinates of pixel | ❌        |
| `rgba`    | [`Color`](../structures_and_types/color.md) | Color of pixel       | ❌        |
---
## Example

```cpp
openrender::Point pnt = {42, 21, 9};
openrender::Color rgba0 = {35, 255, 150, 135};
openrender::Color rgba1 = {28, 32, 255, 100};
// Calling the drawAbsolute() function
drawAbsolute(pnt, rgba0);
// Combines colors
drawAbsolute(pnt, rgba1);
```