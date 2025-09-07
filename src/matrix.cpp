#include "../include/matrix.hpp"
#include <iostream>

uint32_t convolute(uint32_t *matrix[], uint n) {
    int half_size = n / 2;
    uint8_t r {}, g {}, b {};

    uint tr = 0, tg = 0, tb = 0;

    for (int y = -half_size; y < half_size; y++)
        for (int x = -half_size; x < half_size; x++) {
            get_rgb(matrix[y + half_size][x + half_size], &r, &g, &b);

            tr += r;
            tg += g;
            tb += b;
        }
    
    uint kernel_size = n * n;

    return get_hex(
        tr / kernel_size,
        tg / kernel_size, 
        tb / kernel_size
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
