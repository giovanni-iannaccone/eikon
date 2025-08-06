# Circle

This class accepts four required values and one optional:
```cpp
float radius;
uint32_t color;
size_t xc, yc;
float opaqueness;
```
- `radius` defines the circle's radius
- `color` sets the circle's color
- `xc` and `yc` specify the center coordinates of the circle
- `opaqueness` controls the circle's transparency; the algorithm uses this value to determine the final color when the circle overlaps other shapes

The algorithm checks whether a point lies within the circle by applying the Pythagorean theorem: if the squared distance between the center and the current point is less than or equal to the squared radius, the corresponding value in the pixels array is updated with the appropriate color.

To optimize performance, the algorithm limits its execution to a defined portion of the canvas:
- Along the Y-axis: from the topmost point of the circle (yc - radius) to the bottommost point (yc + radius)
- Along the X-axis: from the first point within the circle to its symmetric counterpart (2 * center - distance)

It's just basic geometry:

```cpp
float radius_squared {radius * radius};
size_t dist {};

for (size_t y = yc - radius; y <= yc + radius; y++) {
    
    dist = xc - radius;
    while(radius_squared < (y - yc) * (y - yc) + (dist - xc) * (dist - xc))
        dist++;
    
    for (size_t x = dist; x <= 2*xc - dist; x++) 
        pixels[y * width + x] = opaqueness != 1
            ? mix_colors(pixels[y * width + x], color, opaqueness)
            : color;
}
```