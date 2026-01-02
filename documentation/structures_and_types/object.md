```cpp
struct Object {
	std::vector<Point> points;
	Color rgba;
	bool rendered;
};
```
---
## Basic Info
The structure that stores objects as points and a color before being flattened into 2D.
### Variables
| Variable   | Type                               | Use                                     | Optional |
|------------|------------------------------------|-----------------------------------------|----------|
| `points`   | [`std::vector<Point>`](./point.md) | List of every point in object           | ❌        |
| `rgba`     | [`Color`](./color.md)              | Color of all the points                 | ❌        |
| `rendered` | `bool`                             | Flag determining if it will be rendered | ❌        |
---
## Example
```cpp
// Using Object structure
const openrender::Object CIRLCE = openrender::drawCircle(42.0, 42.0, 42.0, 42.0, {255, 255, 255, 255}, 42.0, 42.0, 42.0);
```