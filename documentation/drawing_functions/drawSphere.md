```cpp
Object drawSphere(const double x, const double y, const double z, const double r,
	const Color rgba);
```
---
## Basic Info
Draws a line in absolute space
### Return Type
returns an [`Object`](../structures_and_types/object.md)

### Parameters
| Parameter | Type                              | Use                    | Optional |
|-----------|-----------------------------------|------------------------|----------|
| `x`       | `double`                          | X Coordinate of center | ❌        |
| `y`       | `double`                          | Y Coordinate of center | ❌        |
| `z`       | `double`                          | Z Coordinate of center | ❌        |
| `r`       | `double`                          | Radius of sphere       | ❌        |
| `rgba`    | [`Color`](../structures_and_types/color.md) | Color of sphere        | ❌        |
---
## Example

```cpp
// Defining object list
std::map<std::string, openrender::Object> objectList;
// Calling the drawSphere() function
objectList.insert({"sphere", openrender::drawSphere(0.0, 0.0, 0.0, 42.0, {255, 255, 255, 255})});

// Converting from 3D to 2D
openrender::projToFlat(0.0, 0.0, 0.0, &objectList, 0, 0, 0);

void *data = openrender::convToBytes();
```