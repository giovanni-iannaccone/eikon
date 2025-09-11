# Circle

This class accepts four required values:
```cpp
float radius;
uint32_t color;
size_t cx, cy;
```
- `radius` defines the circle's radius
- `color` sets the circle's color
- `cx` and `cy` specify the center coordinates of the circle

The algorithm fills a circle by drawing horizontal scanlines across its diameter. It uses the Pythagorean theorem to determine the horizontal bounds of each scanline: for a given vertical offset from the center, it calculates the minimum x such that the point lies within the circle. Then, it draws a horizontal line from that point to its symmetric counterpart.

To optimize performance, the algorithm:
- Precomputes the squared radius to avoid repeated multiplication
- Limits its vertical range to the top half of the circle, using symmetry to mirror the bottom half
- Avoids square roots by comparing squared distances

Here’s the math in motion:

```cpp
float radius_squared = radius * radius;
    
uint y  = cy - radius;
uint dist {};

for (; y < cy; y++) {
    
    dist = cx - radius;
    uint y2cy2 = (y - cy) * (y - cy);
    while(radius_squared < y2cy2 + (dist - cx) * (dist - cx))
        dist++;
        
    for (uint x = dist; x <= 2*cx - dist; x++) {
        pixels[y][x] = get_alpha_blend_color(pixels[y][x], color);
        pixels[2*cy - y][x] = get_alpha_blend_color(pixels[2*cy - y][x], color);
    }
}
```

And then another execution to draw the central row:
```cpp
dist = cx - radius;
uint y2cy2 = (y - cy) * (y - cy);
while(radius_squared < y2cy2 + (dist - cx) * (dist - cx))
    dist++;

for (uint x = dist; x <= 2*cx - dist; x++)
    pixels[y][x] = get_alpha_blend_color(pixels[y][x], color);
```

Adjust your shape's transparency via the alpha component of the ARGB color value.

To actually draw your shape, instantiate a new shape object (using its constructor) and pass it to the `draw` method:
```cpp
Circle circle {100.0, 400, 400, 0xFF0000FF};

canvas->fill(0xFF000000)
    ->draw(circle);
```