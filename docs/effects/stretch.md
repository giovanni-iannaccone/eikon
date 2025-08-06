# Stretch

The stretch function accepts one parameter: size. This determines how many times each pixel is duplicated along the x-axis.

To perform the operation, the function creates a new pixel array with expanded dimensions. After using it, you can _feel free to free_ the old pixels array—yes, I know, pun intended, and I humbly apologize for the low-tier wordplay... but I couldn’t resist.
```cpp
RaccoonCanvas* stretch(unsigned int size = 2) {
    uint32_t *new_pixels = new uint32_t[this->width * this->height * size];

    for (size_t i = 0; i < this->width * this->height; i++)
        for (unsigned int j = 0; j < size; j++)
            new_pixels[i*size + j] = this->pixels[i];

    this->pixels = new_pixels;
    this->width *= size;

    return this;
}
```