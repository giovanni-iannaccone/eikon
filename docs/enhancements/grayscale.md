# Gray Scale

This method iterates over every pixel in the matrix and converts it to its grayscale equivalent. A pixel is considered grayscale when its red, green, and blue components are equal. To achieve this, the method uses perceptual weights based on how the human eye responds to color: red is weighted by 0.30, green by 0.59, and blue by 0.11. By multiplying each component by its respective weight and summing the result, you get the grayscale intensity, which is then assigned to all three channels of the pixel.

This method makes use of the `map` method, taking advantage of its caching mechanism to accelerate repeated computations.

```cpp
eikon::Canvas &Canvas::gray_scale() {
    uint8_t r {}, g {}, b {};

    this->map([&] (uint32_t &pixel) {
        get_rgb(pixel, r, g, b);
        uint8_t gray = 0.30 * r + 0.59 * g + 0.11 * b;
        pixel = get_hex(gray, gray, gray);
    });

    return *this;
}
```
