# Line

According to basic geometry, only one unique line passes through two distinct points. This class takes five parameters: the coordinates of the two endpoints and the desired line color.

```cpp
size_t x1, y1, x2, y2;
uint32_t color;
```

The algorithm draws the line using fundamental geometric logic by calculating the differences between adjacent points along both axes. To ensure precision, an error value is considered when stepping across the grid.

This method is based on Bresenham’s line algorithm—an efficient way to render lines on a pixel canvas:

```cpp
int dx = abs((int)x2 - (int)x1);
int dy = abs((int)y2 - (int)y1);
int sx = x1 < x2 ? 1 : -1;
int sy = y1 < y2 ? 1 : -1;
int err = dx - dy;

while (x1 != x2 || y1 != y2) {
    pixels[y1 * width + x1] = color;

    int e2 = err * 2;
    if (e2 > -dy) {
        err -= dy;
        x1 += sx;
    }
    if (e2 < dx) {
        err += dx;
        y1 += sy;
    }
}
```

This technique delivers crisp, accurate lines with minimal computational overhead—perfect for drawing in raster graphics environments.