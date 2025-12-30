# Padding

This methd takes five parameters:
- `top`, `bottom`, `left`, `right` are uints that define how many pixels of padding to add on each side of the canvas.
- `color` is a `uint32_t` used to fill the padding area.

To perform the operation, the function allocates a new pixel matrix with expanded dimensions. The original image is copied into the center, and the surrounding areas are filled with the specified color.

```cpp
eikon::Canvas &padding(uint top, uint bottom, uint left, uint right, uint32_t color) {  
    PixelBuffer new_pixels {this->height() + top + bottom, this->width() + left + right};
```

- Top padding: fills the first top rows entirely with color.
```cpp
for (uint i = 0; i < top; i++)
    for (uint j = 0; j < this->width() + left + right; j++)
        new_pixels[i][j] = color;
```

- Left padding, Image placement and Right padding:
```cpp
for (uint i = top; i < this->height() + top; i++) {
    // fills the first left columns of each middle row.
    for (uint j = 0; j < left; j++)
        new_pixels[i][j] = color;

    // copies the original image into the center of the new matrix.
    for (uint j = 0; j < this->width(); j++)
        new_pixels[i][j + left] = this->pixels[i - top][j];

    // fills the last right columns of each middle row.
    for (uint j = this->width() + left; j < this->width() + left + right; j++)
        new_pixels[i][j] = color; 
    }
}
```

- Bottom padding: fills the final bottom rows entirely with color.
```cpp
for (uint i = this->height() + top; i < this->height() + top + bottom; i++)
    for (uint j = 0; j < this->width() + left + right; j++)
        new_pixels[i][j] = color;
    
```

After padding, the original pixel matrix is replaced.
```cpp    

    this->pixels = new_pixels;
    return *this;
}
```
