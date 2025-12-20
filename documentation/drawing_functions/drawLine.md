```cpp
Object drawLine(const double x0, const double y0, const double z0,
	const double x1, const double y1, const double z1,
	const Color rgba)
```
---
## Basic Info
Draws a line in absolute space
### Return Type
returns an [`Object`](../structures/object.md)

### Parameters
| Parameter | Type                              | Use                     | Optional |
|-----------|-----------------------------------|-------------------------|----------|
| `x0`      | `double`                          | X Coordinate of point 1 | No       |
| `y0`      | `double`                          | Y Coordinate of point 1 | No       |
| `z0`      | `double`                          | Z Coordinate of point 1 | No       |
| `x1`      | `double`                          | X Coordinate of point 2 | No       |
| `y1`      | `double`                          | Y Coordinate of point 2 | No       |
| `z1`      | `double`                          | Z Coordinate of point 2 | No       |
| `rgba`    | [`Color`](../structures/color.md) | Color of line           | No       |

## Example

```cpp
// Defining object list
std::map<std::string, openrender::Object> objectList;
// Calling the drawLine() function
objectList.insert({"line", openrender::drawLine(0.0, 0.0, 0.0, 42.0, 42.0, 42.0, 42.0, {255, 255, 255, 255})});

// Converting from 3D to 2D
openrender::projToFlat(0.0, 0.0, 0.0, &objectList, 0, 0, 0);

void *data = openrender::convToBytes();
```