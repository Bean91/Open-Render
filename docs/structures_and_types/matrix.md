```cpp
typedef std::array<std::array<double, 3>, 3> Matrix;
```
---
## Basic Info
A 3 by 3 `double` array. Used to apply rotations.

---
## Example
```cpp
// Using Matrix type
const openrender::Matrix rollMat = {{
		{1.0, 0.0, 0.0},
		{0.0, cos(roll), -sin(roll)},
		{0.0, sin(roll), cos(roll)}
	}};
```