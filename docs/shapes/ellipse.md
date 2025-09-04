# Ellipse

This class accepts five parameters:

```cpp
uint cx, cy;
uint a, b;
uint32_t color;
```

- `cx` and `cy` are the coordinates of the ellipse's center
- `a` and `b` are the semi-axis lengths along the X and Y directions, respectively
- `color` is an ARGB value that defines the ellipse's color

It's `draw` method uses basic analytical geometry to draw a filled ellipse. The general equation of an ellipse centered at (cx, cy) is:
```
x² * b² + y² * a² = a² * b²
```

We iterate vertically along the Y-axis from the top to the bottom of the ellipse. For each Y-coordinate, we compute the corresponding horizontal extent (X-axis range) using the ellipse equation. The leftmost point is calculated directly, and the rightmost point is its symmetric counterpart. Each pixel within this horizontal span is filled using alpha blending.

```cpp
uint a2 = a * a;
uint b2 = b * b;

for (uint y = cy - b; y < cy + b; y++) {
    uint dy = y - cy;

    uint x1 = std::sqrt(a2 - (dy * dy) * a2 / b2);

    for (uint x = cx - x1; x < cx + x1; x++)
        pixels[y][x] = get_alpha_blend_color(pixels[y][x], color);
}
```

To actually draw your shape, instantiate a new shape object (using its constructor) and pass it to the `draw` method:
```cpp
Ellipse ellipse {400, 400, 200, 100, 0xFF00FF00};

canvas->fill(0xFF000000)
    ->draw(ellipse);
```