# Stretch

The stretch function takes one parameter:
- `size` is an `uint` that determines how many times each pixel is duplicated along the x-axis.

```cpp
eikon::Canvas &stretch(uint size) {
    PixelBuffer new_pixels = PixelBuffer(this->height(), this->width() * size);

    for (uint y = 0; y < this->height(); y++)
        for (uint x = 0; x < this->width(); x++)
            for (uint i = 0; i < size; i++)
                new_pixels[y][x * size + i] = this->pixels[y][x];

    this->pixels = new_pixels;
    return *this;
}
```

Basically for every pixel in the original canvas, the function writes `size` consecutive copies of that pixel into the new buffer.
