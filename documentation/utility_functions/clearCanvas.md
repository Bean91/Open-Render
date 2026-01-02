```cpp
void clearCanvas();
```
---
## Basic Info
Clears the stored view pixels for re-rendering

---
## Example

```cpp
// Defining object list
std::map<std::string, openrender::Object> objectList;

objectList.insert({"circle", openrender::drawCircle(42.0, 42.0, 42.0, 42.0, {255, 255, 255, 255}, 42.0, 42.0, 42.0)});

// Converting from 3D to 2D
openrender::projToFlat(0.0, 0.0, 0.0, &objectList, 0, 0, 0);

void *data = openrender::convToBytes();

// Calling the clearCanvas() function
openrender::clearCanvas();

//Re-render
openrender::projToFlat(42.0, 42.0, .0, &objectList, 42, 42, 42);

void *data = openrender::convToBytes();
```