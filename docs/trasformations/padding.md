# Padding

This methd takes five parameters and one optional:
- `top`, `bottom`, `left`, `right` are uints that define how many pixels of padding to add on each side of the canvas.
- `color` is a `uint32_t` used to fill the padding area.
- `pixels` is an `uint32_t ***`, useful for memory management (see the WARNING section below).

To perform the operation, the function allocates a new pixel matrix with expanded dimensions. The original image is copied into the center, and the surrounding areas are filled with the specified color.

```cpp
EikonCanvas *padding(uint top, uint bottom, uint left, uint right, uint32_t color, uint32_t ***pixels = nullptr) {
    uint32_t **new_pixels = new uint32_t*[this->height + top + bottom];
    
    for (uint i = 0; i < this->height + top + bottom; i++)
        new_pixels[i] = new uint32_t[this->width + left + right];
```

- Top padding: fills the first top rows entirely with color.
```cpp
for (uint i = 0; i < top; i++)
    for (uint j = 0; j < this->width + left + right; j++)
        new_pixels[i][j] = color;
```

- Left padding, Image placement and Right padding:
```cpp
for (uint i = top; i < this->height + top; i++) {
    // fills the first left columns of each middle row.
    for (uint j = 0; j < left; j++)
        new_pixels[i][j] = color;

    // copies the original image into the center of the new matrix.
    for (uint j = 0; j < this->width; j++)
        new_pixels[i][j + left] = this->pixels[i - top][j];

    // fills the last right columns of each middle row.
    for (uint j = this->width + left; j < this->width + left + right; j++)
        new_pixels[i][j] = color; 
}
```

- Bottom padding: fills the final bottom rows entirely with color.
```cpp
for (uint i = this->height + top; i < this->height + top + bottom; i++)
    for (uint j = 0; j < this->width + left + right; j++)
        new_pixels[i][j] = color;
```

After padding, the canvas dimensions are updated and the original pixel matrix is replaced.
```cpp
    this->width  += left + right;
    this->height += top + bottom;

    delete[] this->pixels;
    this->pixels = new_pixels;

    new_pixels = nullptr;
    if (pixels != nullptr)
        *pixels = this->pixels;

    return this;
}
```

> [!WARNING] When invoking `padding`, the canvas allocates a new matrix with additional rows and columns to accommodate the padding.
> These new regions may not be accessible through standard iteration unless properly tracked. To avoid memory leaks, it's recommended to
> declare a pointer-to-pointer and pass its address to pad. This ensures that all allocated memory—including the padded regions—is 
> properly freed when no longer needed.
> ```cpp
> uint32_t **new_pixels {};
> canvas->draw(rectangle)
>       ->pad(10, 10, 20, 20, 0xFFFFFFFF, &new_pixels)
>       ->save(fd, FileType::PPM);
>
> free_pixels(new_pixels, canvas->height);
> ```