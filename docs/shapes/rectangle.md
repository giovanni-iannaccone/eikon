# Rectangle

This class accepts five parameters:

```cpp
size_t x1, y1, h, b;
uint32_t color;
```

- `x1` and `y1` represent the coordinates of the top-left vertex of the rectangle
- `h` and `b` define the height and width (base) of the rectangle
- `color` specifies the rectangle's fill color

The drawing algorithm is straightforward—it iterates from the starting point to the opposite corner, updating each pixel in the range with the given color:

```cpp
for (size_t y = y1; y < y1 + h; y++)
    for (size_t x = x1; x < x1 + b; x++)
        pixels[y * width + x] = get_alpha_blend_color(pixels[y][x], color);
```

Adjust your shape's transparency via the alpha component of the ARGB color value.

To actually draw your shape, instantiate a new shape object (using its constructor) and pass it to the `draw` method:
```cpp
Rectangle rec {150, 200, 100, 200, 0xFFA1FF15};

canvas->fill(0xFF000000)
    ->draw(rec);
```