```cpp
Point getDistance(const std::array<Point, 8> viewFrame,
	const double x1, const double y1, const double z1);
```
---
## Basic Info
Gets the distance from a point to the line along the view frame.
### Return Type
returns a [`Point`](../structures_and_types/point.md)

### Parameters
| Parameter   | Type                                                       | Use                                | Optional |
|-------------|------------------------------------------------------------|------------------------------------|----------|
| `viewFrame` | [`std::array<Point, 8>`](../structures_and_types/point.md) | Verteces of rectangle being viewed | ❌        |
| `x1`        | `double`                                                   | Y Coordinate of point              | ❌        |
| `y1`        | `double`                                                   | Z Coordinate of point              | ❌        |
| `z1`        | `double`                                                   | Z Coordinate of point              | ❌        |
---
## Example

```cpp
std::array<openrender::Point, 8> viewFrame = {
	{0, 0, 0},
	{0, 42, 0},
	{0, 0, 42},
	{0, 42, 42},
	{42, 0, 0},
	{42, 42, 0},
	{42, 0, 42},
	{42, 42, 42},
};
openrender::Point pnt = {35, 21, 12};
// Calling the getDistance() function
openrender::Point p = getDistance(viewFrame, pnt.x, pnt.y, pnt.z);
```