#pragma once

#include <cmath>
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
void reverse_matrix(T &mat, uint n, uint m) {
    for (uint y = 0; y < n; y++)
        for(uint x = 0; x < m / 2; x++)
            std::swap(
                mat[y][x],
                mat[y][m - x - 1]
            );
}

template <matrix T>
void transpose_matrix(T &mat, uint n, uint m) {
    for (uint y = 0; y < n; y++) 
        for (uint x = y + 1; x < m; x++)
            std::swap(
                mat[y][x], 
                mat[x][y]
            );
}
