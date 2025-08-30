# Gray Scale

This method iterates over every pixel in the matrix and converts it to its grayscale equivalent. A pixel is considered grayscale when its red, green, and blue components are equal. To achieve this, the method uses perceptual weights based on how the human eye responds to color: red is weighted by 0.30, green by 0.59, and blue by 0.11. By multiplying each component by its respective weight and summing the result, you get the grayscale intensity, which is then assigned to all three channels of the pixel.

```cpp
EikonCanvas *gray_scale() {
    uint8_t r {}, g {}, b {};
    uint32_t pixel;

    for (uint y = 0; y < this->height; y++)
        for (uint x = 0; x < this->width; x++) {
            get_rgb(this->pixels[y][x], &r, &g, &b);

            pixel = 0.30 * r + 0.59 * g + 0.11 * b;

            this->pixels[y][x] = get_hex(
                pixel,
                pixel,
                pixel
            );
        }

    return this;
}
```