```cpp
Object drawCircle(const double x, const double y, const double z, const double r,
	const Color rgba,
	double roll = 0.0, double pitch = 0.0, double yaw = 0.0);
```
---
## Basic Info
Draws a circle in absolute space
### Return Type
returns an [`Object`](../structures/object.md)

### Parameters
| Parameter | Type                              | Use                    | Optional |
|-----------|-----------------------------------|------------------------|----------|
| `x`       | `double`                          | X Coordinate of center | ❌        |
| `y`       | `double`                          | Y Coordinate of center | ❌        |
| `z`       | `double`                          | Z Coordinate of center | ❌        |
| `r`       | `double`                          | Radius of circle       | ❌        |
| `rgba`    | [`Color`](../structures/color.md) | Color of circle        | ❌        |
| `roll`    | `double`                          | Roll of circle         | ✅        |
| `pitch`   | `double`                          | Pitch of circle        | ✅        |
| `yaw`     | `double`                          | Yaw of circle          | ✅        |
---
## Example

```cpp
// Defining object list
std::map<std::string, openrender::Object> objectList;
// Calling the drawCircle() function
objectList.insert({"circle", openrender::drawCircle(42.0, 42.0, 42.0, 42.0, {255, 255, 255, 255}, 42.0, 42.0, 42.0)});

// Converting from 3D to 2D
openrender::projToFlat(0.0, 0.0, 0.0, &objectList, 0, 0, 0);

void *data = openrender::convToBytes();
```