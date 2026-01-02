```cpp
void* convToBytes();
```
---
## Basic Info
Turns the [`Color`](../structures_and_types/color.md) array into a `uint8_t` array for binary streaming.
### Return Type
returns a `void *`
---
## Example

```cpp
// Defining object list
std::map<std::string, openrender::Object> objectList;

objectList.insert({"circle", openrender::drawCircle(42.0, 42.0, 42.0, 42.0, {255, 255, 255, 255}, 42.0, 42.0, 42.0)});

// Converting from 3D to 2D
openrender::projToFlat(0.0, 0.0, 0.0, &objectList, 0, 0, 0);

// Calling the convToBytes() function
void *data = openrender::convToBytes();
```