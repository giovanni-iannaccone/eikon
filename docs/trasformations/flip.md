# Flip

Flip is a basic utility that swaps symmetric points within the same column of the canvas.
```cpp
RaccoonCanvas* flip() {
    for (size_t y = 0; y < this->height / 2; y++)
        for (size_t x = 0; x < this->width; x++)
            std::swap(
                this->pixels[y][x],
                this->pixels[this->height - y - 1][x]
            );
    
    return this;
}
```

Using it is straightforward—you simply call `->flip()` on the canvas object.