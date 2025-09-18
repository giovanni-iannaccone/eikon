# Solarize

Solarization is a visual effect that partially inverts the colors of an image. The process involves iterating through each pixel of the image and evaluating its color components—red (R), green (G), and blue (B). If any of these components exceed a specified threshold—determined by a percentage input—the component is replaced by its inverted value, calculated as `255 - original_value`. This selective inversion creates a surreal, high-contrast look that mimics the photographic solarization technique.

```cpp
EikonCanvas *solarize(float perc) {
    uint8_t limit = 2.55f * perc;
    uint8_t r {}, g {}, b {};

    for (uint y = 0; y < this->height; y++)
        for (uint x = 0; x < this->width; x++) {
            get_rgb(this->pixels[y][x], &r, &g, &b);

            r = (r > limit) ? (255 - r) : r;
            g = (g > limit) ? (255 - g) : g;
            b = (b > limit) ? (255 - b) : b;

            this->pixels[y][x] = get_hex(r, g, b);
        }
    
    return this;
}
```