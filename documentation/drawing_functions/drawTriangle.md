```cpp
Object drawTriangle(const double x0, const double y0, const double z0,
	const double x1, const double y1, const double z1,
	const double x2, const double y2, const double z2, const Color rgba);
```
---
## Basic Info
Draws a line in absolute space
### Return Type
returns an [`Object`](../structures_and_types/object.md)

### Parameters
| Parameter | Type                              | Use                     | Optional |
|-----------|-----------------------------------|-------------------------|----------|
| `x0`      | `double`                          | X Coordinate of point 1 | ❌        |
| `y0`      | `double`                          | Y Coordinate of point 1 | ❌        |
| `z0`      | `double`                          | Z Coordinate of point 1 | ❌        |
| `x1`      | `double`                          | X Coordinate of point 2 | ❌        |
| `y1`      | `double`                          | Y Coordinate of point 2 | ❌        |
| `z1`      | `double`                          | Z Coordinate of point 2 | ❌        |
| `x2`      | `double`                          | X Coordinate of point 3 | ❌        |
| `y2`      | `double`                          | Y Coordinate of point 3 | ❌        |
| `z2`      | `double`                          | Z Coordinate of point 3 | ❌        |
| `rgba`    | [`Color`](../structures_and_types/color.md) | Color of triangle       | ❌        |
---
## Example

```cpp
// Defining object list
std::map<std::string, openrender::Object> objectList;
// Calling the drawTriangle() function
objectList.insert({"triangle", openrender::drawTriangle(0.0, 0.0, 0.0, 42.0, 42.0, 42.0, 91.0, 91.0, 91.0 {255, 255, 255, 255})});

// Converting from 3D to 2D
openrender::projToFlat(0.0, 0.0, 0.0, &objectList, 0, 0, 0);

void *data = openrender::convToBytes();
```