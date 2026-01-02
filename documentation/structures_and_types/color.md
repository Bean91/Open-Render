```cpp
struct Color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};
```
---
## Basic Info
The structure that stores color data in absolute space before being converted to raw bytes.
### Variables
| Variable | Type      | Use                  | Optional |
|----------|-----------|----------------------|----------|
| `r`      | `uint8_t` | Red value of color   | ❌        |
| `g`      | `uint8_t` | Green value of color | ❌        |
| `b`      | `uint8_t` | Blue value of color  | ❌        |
| `a`      | `uint8_t` | Alpha value of color | ❌        |
### Functions
```cpp
Color operator*(const double bc) const
```
#### Basic Info
Returns a `Color` with the `a` value being scaled by parameter `bc`

```cpp
bool operator==(const Color other) const
```
#### Basic Info
Returns true if all corresponding values are the same and false if not.

---
## Example
```cpp
// Using Color structure
const openrender::Color BLUE = {0, 0, 255, 255};
const openrender::Color TRANSPARENT_BLUE = BLUE * 0.5;
```