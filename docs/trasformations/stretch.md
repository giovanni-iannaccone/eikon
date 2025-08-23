# Stretch

The stretch function accepts one parameter: size. This determines how many times each pixel is duplicated along the x-axis.

To perform the operation, the function creates a new pixel array with expanded dimensions. After using it, you can _feel free to free_ the old pixels array—yes, I know, pun intended, and I humbly apologize for the low-tier wordplay... but I couldn’t resist.

```cpp
EikonCanvas *stretch(uint size) {
    uint32_t *new_pixels {};

    for (size_t y = 0; y < this->height; y++) {
        new_pixels = new uint32_t[this->width * size];
        
        for (size_t x = 0; x < this->width; x++)
            for (uint i = 0; i < size; i++)
                new_pixels[x * size + i] = this->pixels[y][x];

        this->pixels[y] = new_pixels;
    }

    this->width *= size;
    new_pixels = nullptr;
    return this;
}
```

>[!WARNING]
> When you invoke the `stretch` method on the canvas, it dynamically generates a large number of rows in memory to accommodate the expanded structure. However, these rows cannot be directly accessible through standard iteration or indexing.
> To avoid memory leaks or unnecessary resource consumption, it's strongly recommended to call the `delete_all` method once you're finished working with the canvas. 
> This ensures that all allocated rows—visible or hidden—are properly deallocated, keeping your application efficient and clean.