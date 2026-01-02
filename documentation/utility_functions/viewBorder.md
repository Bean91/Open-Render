```cpp
std::array<Point, 8> viewBorder(const double x, const double y, const double z,
	double roll = 0.0, double pitch = 0.0, double yaw = 0.0);
```
---
## Basic Info
Returns the vertices of the view frame.
### Return Type
returns a [`std::array<Point, 8>`](../structures_and_types/point.md)

### Parameters
| Parameter | Type     | Use                    | Optional |
|-----------|----------|------------------------|----------|
| `x`       | `double` | X Coordinate of center | ❌        |
| `y`       | `double` | Y Coordinate of center | ❌        |
| `z`       | `double` | Z Coordinate of center | ❌        |
| `roll`    | `double` | Roll of view frame     | ✅        |
| `pitch`   | `double` | Pitch of view frame    | ✅        |
| `yaw`     | `double` | Yaw of view frame      | ✅        |
---
## Example

```cpp
const double x = 0;
const double y = 12;
const double z = 42;
const double roll = 36;
const double pitch = 89;
const double yaw = 359;
// Calling the viewBorder() function
std::array<Point, 8> viewFrame = viewBorder(x, y, z, roll, pitch, yaw);
```