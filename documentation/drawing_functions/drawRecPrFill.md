```cpp
Object drawRecPrFill(const double x, const double y, const double z,
	const double l, const double w, const double h,
	const Color rgba,
	double roll = 0.0, double pitch = 0.0, double yaw = 0.0);
```
---
## Basic Info
Draws a filled rectangular prizm in absolute space
### Return Type
returns an [`Object`](../structures_and_types/object.md)

### Parameters
| Parameter | Type                              | Use                    | Optional |
|-----------|-----------------------------------|------------------------|----------|
| `x`       | `double`                          | X Coordinate of center | ❌        |
| `y`       | `double`                          | Y Coordinate of center | ❌        |
| `z`       | `double`                          | Z Coordinate of center | ❌        |
| `l`       | `double`                          | Length of prizm        | ❌        |
| `w`       | `double`                          | Width of prizm         | ❌        |
| `h`       | `double`                          | Height of prizm        | ❌        |
| `rgba`    | [`Color`](../structures_and_types/color.md) | Color of prizm         | ❌        |
| `roll`    | `double`                          | Roll of circle         | ✅        |
| `pitch`   | `double`                          | Pitch of prizm         | ✅        |
| `yaw`     | `double`                          | Yaw of prizm           | ✅        |
---
## Example

```cpp
// Defining object list
std::map<std::string, openrender::Object> objectList;
// Calling the drawRecPrFill() function
objectList.insert({"recFill", openrender::drawRecPrFill(200.0, 200.0, 200.0, 200.0, 200.0, 200.0, {255, 255, 255, 255})});

// Converting from 3D to 2D
openrender::projToFlat(0.0, 0.0, 0.0, &objectList, 0, 0, 0);

void *data = openrender::convToBytes();
```