```cpp
struct Point {
	double x;
	double y;
	double z;
};
```
---
## Basic Info
The structure that stores objects as points and a color before being flattened into 2D.
### Variables
| Variable | Type     | Use              | Optional |
|----------|----------|------------------|----------|
| `x`      | `double` | X value of point | ❌        |
| `y`      | `double` | Y value of point | ❌        |
| `z`      | `double` | Z value of point | ❌        |
---
## Example
```cpp
const openrender::Point ORIGIN = {0.0, 0.0, 0.0};
```