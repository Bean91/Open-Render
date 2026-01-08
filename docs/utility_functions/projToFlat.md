```cpp
void projToFlat(const double x, const double y, const double z,
	std::map<std::string, Object> *objectList,
	double roll = 0.0, double pitch = 0.0, double yaw = 0.0);
```
---
## Basic Info
Draws the `objectList` into absolute space based on a rectangular view area with rotations `roll`, `pitch`, and `yaw`, and center of `(x, y, z)`

### Parameters
| Parameter    | Type                                                                   | Use                                                   | Optional |
|--------------|------------------------------------------------------------------------|-------------------------------------------------------|----------|
| `x`          | `double`                                                               | X Coordinate of center                                | ❌        |
| `y`          | `double`                                                               | Y Coordinate of center                                | ❌        |
| `z`          | `double`                                                               | Z Coordinate of center                                | ❌        |
| `objectList` | [`std::map<std::string, Object> *`](../structures_and_types/object.md) | List of [`Object`](../structures_and_types/object.md) | ❌        |
| `roll`       | `double`                                                               | Roll of view frame                                    | ✅        |
| `pitch`      | `double`                                                               | Pitch of view frame                                   | ✅        |
| `yaw`        | `double`                                                               | Yaw of view frame                                     | ✅        |
---
## Example

```cpp
// Defining object list
std::map<std::string, openrender::Object> objectList;

objectList.insert({"circle", openrender::drawCircle(42.0, 42.0, 42.0, 42.0, {255, 255, 255, 255}, 42.0, 42.0, 42.0)});

// Calling the projToFlat() function
openrender::projToFlat(0.0, 0.0, 0.0, &objectList, 0, 0, 0);

void *data = openrender::convToBytes();
```