# Negate

This method iterates over every pixel in the matrix and sets it to its negative. 

To compute a pixel's negative, subtract each of its color components from 255. For example, given a pixel (255, 140, 50), its negative would be: (255 - 255, 255 - 140, 255 - 50) -> (0, 115, 205)

```cpp
EikonCanvas *negate() {
    uint8_t r {}, g {}, b {};

    for (size_t y = 0; y < this->height; y++) {
        for (size_t x = 0; x < this->width; x++) {
            get_rgb(this->pixels[y][x], &r, &g, &b);

            r = 255 - r;
            g = 255 - g;
            b = 255 - b;

            this->pixels[y][x] = get_hex(r, g, b);
        }
    }

    return this;
}
```
