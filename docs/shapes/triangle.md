# Triangle

This class accepts seven required values:
```cpp
uint x1, y1;
uint x2, y2;
uint x3, y3;
uint32_t color;
```

- `x1`, `y1`, `x2`, `y2`, `x3`, `y3` define the three vertices of the triangle
- `color` sets the fill color of the triangle

The algorithm fills the triangle by iterating over the bounding box that contains it, and checking whether each pixel lies inside the triangle using barycentric coordinates via the cross product method.

To determine if a point lies inside the triangle, the algorithm computes the sign of the cross product between each edge and the point in question. If the point lies on the same side of all three edges, it is considered inside.

```cpp
int cross_product(int px, int py, int qx, int qy, int rx, int ry) const {
    return (qx - px) * (ry - py) - (qy - py) * (rx - px);
}

bool is_inside(int px, int py) const {
    int d1 = cross_product(x1, y1, x2, y2, px, py);
    int d2 = cross_product(x2, y2, x3, y3, px, py);
    int d3 = cross_product(x3, y3, x1, y1, px, py);

    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}
```

To optimize performance, the algorithm:
- Computes the bounding box of the triangle to limit the area of iteration
- Avoids floating-point arithmetic by using integer cross products
- Checks pixel bounds before drawing to avoid memory access violations

Here’s the implementation:
```cpp

int minX = utils::min(x1, x2, x3);
int maxX = utils::max(x1, x2, x3);
int minY = utils::min(y1, y2, y3);
int maxY = utils::max(y1, y2, y3);

for (int y = minY; y <= maxY; ++y)
    for (int x = minX; x <= maxX; ++x)
        if (x >= 0 && x < width && y >= 0 && y < height)
            if (is_inside(x, y))
                pixels[y][x] = utils::get_alpha_blend_color(pixels[y][x], color);
```

The `get_alpha_blend_color` function blends the new color with the existing pixel value, allowing for transparency effects via the alpha component of the ARGB color.

To draw your shape, instantiate a Triangle and pass it to the canvas:
```cpp
eikon::Triangle triangle {100, 100, 300, 150, 200, 300, 0xFFAA33CC};

canvas.fill(0xFF000000)
    .draw(triangle);
```

This approach ensures your triangle is filled accurately and efficiently, using integer math and geometric principles to achieve pixel-perfect rendering.
