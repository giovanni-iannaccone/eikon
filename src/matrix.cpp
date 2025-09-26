#include "../include/matrix.hpp"

uint32_t convolute(uint32_t *matrix[], uint n) {
    uint8_t r {}, g {}, b {};
    uint16_t tr = 0, tg = 0, tb = 0;

    uint n_elements = n * n;

    for (uint y = 0; y < n; y++)
        for (uint x = 0; x < n; x++) {
            get_rgb(matrix[y][x], r, g, b);

            tr += r;
            tg += g;
            tb += b;
        }

    return get_hex(
        tr / n_elements,
        tg / n_elements,
        tb / n_elements
    );
}

void reverse_matrix(uint32_t *matrix[], uint n, uint m) {
    for (uint y = 0; y < n; y++)
        for(uint x = 0; x < m / 2; x++)
            std::swap(
                matrix[y][x],
                matrix[y][m - x - 1]
            );
}

void transpose_matrix(uint32_t *matrix[], uint n, uint m) {
    for (uint y = 0; y < n; y++) 
        for (uint x = y + 1; x < m; x++)
            std::swap(
                matrix[y][x], 
                matrix[x][y]
            );
}
