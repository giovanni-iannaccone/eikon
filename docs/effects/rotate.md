# Rotate

Rotation is performed in two phases: first, the `pixels` array is transposed; then, it is reversed.

```cpp
RaccoonCanvas* rotate() {
    if (this->width != this->height)
        return nullptr;

    transpose_flat_matrix(this->pixels, this->height, this->width);
    reverse_flat_matrix(this->pixels, this->height, this->width);

    return this;
}
```

Both functions—`transpose_flat_matrix` and `reverse_flat_matrix`—are located in the `include/matrix.hpp` file and implement basic matrix operations.