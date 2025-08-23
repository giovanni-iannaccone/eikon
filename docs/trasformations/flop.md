# Flop

Flop is a basic utility that swaps symmetric points within the same row of the canvas.
```cpp
EikonCanvas* flop() {
    for (size_t y = 0; y < this->height; y++)
        for (size_t x = 0; x < this->width / 2; x++)
            std::swap(
                this->pixels[y][x],
                this->pixels[y][this->width - x - 1]
            );
    
    return this;
}
```

Using it is straightforward—you simply call `->flop()` on the canvas object.