```cpp
struct Color {
	std::vector<Point> points;
	Color rgba;
	bool rendered;
};
```
---
## Basic Info
The structure that stores objects as points and a color before being flattened into 2D.
### Variables
| Variable   | Type                               | Use                  | Optional |
|------------|------------------------------------|----------------------|----------|
| `points`   | [`std::vector<Point>`](./point.md) | Red value of color   | ❌        |
| `rgba`     | [`Color`](./color.md)              | Green value of color | ❌        |
| `rendered` | `bool`                             | Blue value of color  | ❌        |
---
## Example
```cpp
const openrender::Object CIRLCE = openrender::drawCircle(42.0, 42.0, 42.0, 42.0, {255, 255, 255, 255}, 42.0, 42.0, 42.0);
```