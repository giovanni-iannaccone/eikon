# Blur
The blur algorithm used here is the box blur. It works by selecting a small matrix (kernel) around each pixel and calculating the average of the values within that matrix. The resulting average becomes the new value of the pixel at the center of the matrix.

```cpp
EikonCanvas *blur(uint8_t radius) {
    uint kernel_size = radius * 2 + 1;
    uint32_t **matrix = new uint32_t*[kernel_size];

    for (uint y = radius; y < this->height - radius; y++)
        for (uint x = radius; x < this->width - radius; x++) {

            for (int i = 0; i < kernel_size; i++)
                matrix[i] = this->pixels[y] + x;

            this->pixels[y][x] = convolute(matrix, kernel_size);
        }
    
    delete[] matrix;
    return this;
}
```

The function takes a radius parameter, which defines how far the kernel extends from the center pixel. For example, a radius of 1 means the kernel is 3×3, a radius of 2 means 5×5, and so on.

The `convolute` function computes the mean value of the matrix. This algorithm is quite basic and could be improved using a Gaussian blur, which produces smoother results but is computationally more expensive.
