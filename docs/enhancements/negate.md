# Negate

This method iterates over every pixel in the matrix and sets it to its negative. 

To compute a pixel's negative, subtract each of its color components from 255. For example, given a pixel (255, 140, 50), its negative would be: (255 - 255, 255 - 140, 255 - 50) -> (0, 115, 205)

This method makes use of the `map` method, taking advantage of its caching mechanism to accelerate repeated computations.

```cpp
eikon::Canvas &Canvas::negate() {    
    uint8_t r {}, g {}, b {};

    this->map([&] (uint32_t &pixel) {
        get_rgb(pixel, r, g, b);
        
        r = 255 - r;
        g = 255 - g;
        b = 255 - b;
        pixel = get_hex(r, g, b);
    });
    
    return *this;
}
```
