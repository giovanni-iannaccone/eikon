# Flip

Flip is a basic utility that swaps symmetric points within the same row of the canvas.
```cpp
RaccoonCanvas* flip() {
    for (size_t y = 0; y < this->height; y++)
        for (size_t x = 0; x < this->width / 2; x++)
            std::swap(
                this->pixels[y*this->width + x],
                this->pixels[(y + 1)*this->width - x - 1]
            );
    
    return this;
}
```

Using it is straightforward—you simply call `->flip()` on the canvas object.