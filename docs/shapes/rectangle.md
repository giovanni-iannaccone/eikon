# Rectangle

This class accepts five parameters, plus one optional:

```cpp
size_t x1, y1, h, b;
uint32_t color;
float opaqueness;
```

- `x1` and `y1` represent the coordinates of the top-left vertex of the rectangle
- `h` and `b` define the height and width (base) of the rectangle
- `color` specifies the rectangle's fill color
- `opaqueness` determines its transparency level; the algorithm uses this value to calculate the final color when the rectangle overlaps other shapes

The drawing algorithm is straightforward—it iterates from the starting point to the opposite corner, updating each pixel in the range with the given color:

```cpp
for (size_t y = y1; y < y1 + h; y++)
    for (size_t x = x1; x < x1 + b; x++)
        pixels[y * width + x] = opaqueness != 1
            ? mix_colors(pixels[y][x], color, opaqueness)
            : color;
```

To actually draw your shape, instantiate a new shape object (using its constructor) and pass it to the `draw` method:
```cpp
Rectangle rec {150, 200, 100, 200, 0xFFA1FF15};

canvas->fill(0xFF000000)
    ->draw(rec);
```