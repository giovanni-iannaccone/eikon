# Rotate

Rotation is performed in two phases: first, the `pixels` matrix is transposed; then, it is reversed.

```cpp
EikonCanvas* rotate() {
    if (this->width != this->height)
        return nullptr;

    transpose_matrix(this->pixels, this->height, this->width);
    reverse_matrix(this->pixels, this->height, this->width);

    return this;
}
```

Both functions—`transpose_matrix` and `reverse_matrix`—are located in the `include/matrix.hpp` file and implement basic matrix operations.