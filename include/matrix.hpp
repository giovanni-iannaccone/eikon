#pragma once

#include <cstdint>

#include "utils.hpp"

template <typename T>
concept matrix = requires (T a) {
    a[0][0];
};

template <matrix T>
uint32_t convolute(const T &mat, uint n) {
    uint8_t r {}, g {}, b {};
    uint32_t tr = 0, tg = 0, tb = 0;

    for (uint y = 0; y < n; y++) {
        for (uint x = 0; x < n; x++) {
            get_rgb(mat[y][x], r, g, b);

            tr += r;
            tg += g;
            tb += b;
        }
    }

    uint n_elements = n * n;
    return get_hex(
        tr / n_elements,
        tg / n_elements,
        tb / n_elements
    );
}

template <matrix T>
void rotate_matrix(T &mat, uint n, uint m) {
    T new_mat {m, n};

    for (uint i = 0; i < n; i++)
        for (uint j = 0; j < m; j++)
            new_mat[j][n - i - 1] = mat[i][j];

    mat = new_mat;
}
