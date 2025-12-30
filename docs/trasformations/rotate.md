# Rotate

Rotation is performed in two phases: first, the `pixels` matrix is transposed; then, it is reversed.

```cpp
eikon::Canvas &rotate() {
    rotate_matrix(this->pixels, this->height(), this->width());
    return *this;
}
```

The function `rotate_matrix` is located in the `include/matrix.hpp` file and implement basic matrix operations. The rotation is implemented as follows:

```cpp
template <typename T>
concept matrix = requires (T a) {
    a[0][0];
};


template <matrix T>
void rotate_matrix(T &mat, uint n, uint m) {
    T new_mat {m, n};

    for (uint i = 0; i < n; i++)
        for (uint j = 0; j < m; j++)
            new_mat[j][n - i - 1] = mat[i][j];

    mat = new_mat;
}
```

A new matrix is created, and its elements are copied by transposing rows and columns and rotating the columns. Finally, the new matrix is moved back into the original one.
Using this algorithm, eikon can rotate every nxm matrix.
