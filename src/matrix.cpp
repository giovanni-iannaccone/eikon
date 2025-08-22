#include "../include/matrix.hpp"

void reverse_matrix(uint32_t *matrix[], uint n, uint m) {
    for (uint y = 0; y < n; y++)
        for(uint x = 0; x < n / 2; x++)
            std::swap(
                matrix[y][x],
                matrix[y][n - x - 1]
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
            
