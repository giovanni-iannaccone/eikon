# Brightness

This method decomposes each pixel into its RGB channels, scales them by the `inc` factor, and reassembles the modified values into new pixels. The overall effect is an increase in image brightness.

```cpp
eikon::Canvas &Canvas::brightness(float inc) {
    uint8_t r {}, g {}, b {};

    this->map([&] (uint32_t &pixel) {
        eikon::get_rgb(pixel, r, g, b);
        
        r = std::min(255.0f, r * inc);
        g = std::min(255.0f, g * inc);
        b = std::min(255.0f, b * inc);

        pixel = get_hex(r, g, b);
    });
    
    return *this;
}
```
