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

The algorithm fills the ellipse by drawing horizontal scanlines across its width. For each vertical offset from the center, it calculates the horizontal extent of the ellipse at that height using the canonical ellipse equation:
x2a2+y2b2=1\frac{x^2}{a^2} + \frac{y^2}{b^2} = 1

Rearranged to solve for x:
x=a2(1−(y−cy)2b2)x = \sqrt{a^2 \left(1 - \frac{(y - cy)^2}{b^2}\right)}

To optimize performance, the algorithm:
- Precomputes `a²` and `b²` to avoid repeated multiplication
- Uses vertical symmetry to mirror the top half of the ellipse onto the bottom half
- Avoids floating-point division inside the inner loop

It's just basic geometry:

```cpp
uint a2 = a * a;
uint b2 = b * b;
uint y = cy - b;

for (; y < cy; y++) {
    uint dy = y - cy;

    uint x1 = std::sqrt(a2 - (dy * dy) * a2 / b2);

    for (uint x = cx - x1; x < cx + x1; x++) {
        pixels[y][x] = get_alpha_blend_color(pixels[y][x], color);
        pixels[2*cy - y][x] = get_alpha_blend_color(pixels[2*cy - y][x], color);
    }
}
```

And then another execution to draw the central row: 
```cpp
uint dy = y - cy;
uint x1 = std::sqrt(a2 - (dy * dy) * a2 / b2);

for (uint x = cx - x1; x < cx + x1; x++)
    pixels[y][x] = get_alpha_blend_color(pixels[y][x], color);
```

To actually draw your shape, instantiate a new shape object (using its constructor) and pass it to the `draw` method:
```cpp
Ellipse ellipse {400, 400, 200, 100, 0xFF00FF00};

canvas->fill(0xFF000000)
    ->draw(ellipse);
```