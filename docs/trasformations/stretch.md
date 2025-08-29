# Stretch

The stretch function takes one parameter and one optional:
- `size` is an `uint` that determines how many times each pixel is duplicated along the x-axis.
- `pixels` is a pointer to pointer to pointer that is useful to prevent memory leaks (see the WARNING section below).

To perform the operation, the function creates a new pixel array with expanded dimensions. After stretching, you may free the original pixel array if needed.

```cpp
EikonCanvas *stretch(uint size, uint32_t ***pixels = nullptr) {
    uint32_t *new_pixels {};

    for (uint y = 0; y < this->height; y++) {
        new_pixels = new uint32_t[this->width * size];
        
        for (uint x = 0; x < this->width; x++)
            for (uint i = 0; i < size; i++)
                new_pixels[x * size + i] = this->pixels[y][x];

        this->pixels[y] = new_pixels;
    }

    this->width *= size;
    new_pixels = nullptr;
    
    if (pixels != nullptr)
        *pixels = this->pixels;

    return this;
}
```

>[!WARNING]
> When invoking `stretch`, the canvas dynamically allocates a large number of rows to accommodate the expanded structure. These rows may not be directly accessible through standard iteration or indexing.
> To avoid memory leaks or excessive resource usage, it's strongly recommended to declare a pointer-to-pointer and pass its address to stretch. This allows you to properly free the allocated memory afterward.
> This ensures that all allocated rows—visible or hidden—are properly deallocated, keeping your application efficient and clean.
> ```cpp
> uint32_t **new_pixels {};
> 
> canvas->draw(circle)
>        ->stretch(3, &new_pixels)
>        ->save(fd, FileType::PPM);
> 
> free_pixels(new_pixels, HEIGHT);
> ```