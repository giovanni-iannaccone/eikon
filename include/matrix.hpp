#pragma once

#include <algorithm>
#include <cstdint>

void reverse_flat_matrix(uint32_t matrix[], size_t n, size_t m) {
        for (size_t y = 0; y < n; y++)
            for(size_t x = 0; x < n / 2; x++)
                std::swap(matrix[y*n + x], 
                    matrix[y*n + n - x - 1]
                );
    }

void transpose_flat_matrix(uint32_t matrix[], size_t n, size_t m) {
    for (size_t y = 0; y < n; y++) 
        for (size_t x = y + 1; x < m; x++)
            std::swap(matrix[y*m + x], 
                matrix[x*m + y]
            );
}