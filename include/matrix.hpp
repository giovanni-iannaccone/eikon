#pragma once

#include <cstdint>
#include <span>

#include "utils.hpp"

namespace eikon {
    
template <typename T>
concept matrix = requires (T a) {
    a[0][0];
};

template <matrix T>
uint32_t convolute(const T& mat, uint n) {
    uint32_t tr = 0, tg = 0, tb = 0;

    for (uint y = 0; y < n; y++) {
        const auto& row = mat[y];

        for (uint x = 0; x < n; x++) {
            utils::rgb chans = utils::get_rgb(row[x]);

            tr += chans.r;
            tg += chans.g;
            tb += chans.b;
        }
    }

    const uint32_t n_elements = n * n;

    return utils::get_hex(
        tr / n_elements,
        tg / n_elements,
        tb / n_elements
    );
}

template <matrix T>
void rotate_matrix(T &mat, uint n, uint m) {
    T new_mat {{m, n}};

    for (uint j = 0; j < m; j++) [[likely]] { 
        std::span dst_row {new_mat[j], n};

        for (uint i = 0; i < n; i++) [[likely]]
            dst_row[n - i - 1] = mat[i][j];
    }

    std::swap(mat, new_mat);
}
    
} // namespace eikon
