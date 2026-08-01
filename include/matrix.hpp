#pragma once

#include <cstdint>
#include <memory>

#include "pixels.hpp"
#include "utils.hpp"

namespace eikon {

inline
void blur(PixelBuffer &pixels, uint8_t radius) noexcept
{
    const uint w = pixels.width();
    const uint h = pixels.height();
    
    PixelBuffer original {pixels};
    const size_t total = w * h;
    auto integral = std::make_unique<utils::rgb[]>(total);
    
    auto idx = [w](uint x, uint y) noexcept {
        return static_cast<size_t>(y) * w + x;
    };
    
    for (uint y = 0; y < h; ++y) {
        uint32_t sr = 0;
        uint32_t sg = 0;
        uint32_t sb = 0;

        const auto* src = original[y];

        for (uint x = 0; x < w; ++x) {
            const auto rgb = utils::get_rgb(src[x]);

            sr += rgb.r;
            sg += rgb.g;
            sb += rgb.b;

            auto& current = integral[idx(x, y)];

            if (y > 0) {
                const auto& up = integral[idx(x, y - 1)];

                current.r = up.r + sr;
                current.g = up.g + sg;
                current.b = up.b + sb;

            } else {
                current.r = sr;
                current.g = sg;
                current.b = sb;
            }
        }
    }

    auto rect_sum =
        [&](uint x0, uint y0, uint x1, uint y1) noexcept {
        utils::rgb result = integral[idx(x1, y1)];

        if (x0) {
            const auto& p = integral[idx(x0 - 1, y1)];

            result.r -= p.r;
            result.g -= p.g;
            result.b -= p.b;
        }

        if (y0) {
            const auto& p = integral[idx(x1, y0 - 1)];

            result.r -= p.r;
            result.g -= p.g;
            result.b -= p.b;
        }

        if (x0 && y0) {
            const auto& p = integral[idx(x0 - 1, y0 - 1)];

            result.r += p.r;
            result.g += p.g;
            result.b += p.b;
        }

        return result;
    };

    constexpr uint TILE_X = 64;
    constexpr uint TILE_Y = 32;

    const uint start_x = radius;
    const uint start_y = radius;
    const uint end_x = w - radius;
    const uint end_y = h - radius;

    const uint area =
        (radius * 2 + 1) *
        (radius * 2 + 1);

    for (uint ty = start_y; ty < end_y; ty += TILE_Y) {        
        const uint tile_end_y = std::min(ty + TILE_Y, end_y);

        for (uint tx = start_x; tx < end_x; tx += TILE_X) {            
            const uint tile_end_x = std::min(tx + TILE_X, end_x);
            const bool reverse = ((ty / TILE_Y) & 1);

            for (uint y = ty; y < tile_end_y; ++y) {
                auto* dst = pixels[y];

                if (!reverse) {
                    for (uint x = tx; x < tile_end_x; ++x) {
                        const auto sum =
                            rect_sum(
                                x - radius,
                                y - radius,
                                x + radius,
                                y + radius
                            );

                        dst[x] = utils::get_hex(
                            sum.r / area,
                            sum.g / area,
                            sum.b / area
                        );
                    }
                } else {
                    
                    for (uint x = tile_end_x; x-- > tx;) {
                        const auto sum =
                            rect_sum(
                                x - radius,
                                y - radius,
                                x + radius,
                                y + radius
                            );

                        dst[x] = utils::get_hex(
                            sum.r / area,
                            sum.g / area,
                            sum.b / area
                        );
                    }
                }
            }
        }
    }
}

inline 
void rotate_matrix(PixelBuffer &mat) noexcept
{
    PixelBuffer new_mat {mat};

    const uint m = mat.height();
    const uint n = mat.width();
    
    constexpr uint TILE = 32;

    for (uint y = 0; y < m; y += TILE) {
        const uint y_end = std::min(y + TILE, m);

        for (uint x = 0; x < n; x += TILE) {
            const uint x_end = std::min(x + TILE, n);

            for (uint j = y; j < y_end; ++j) {
                auto* dst = new_mat[j];

                for (uint i = x; i < x_end; ++i)
                    dst[n - i - 1] = mat[i][j];
            }
        }
    }

    for (uint y = 0; y < m; y++)
        std::memcpy(
            mat[y],
            new_mat[y],
            n * sizeof(uint32_t)
        );
}
    
} // namespace eikon
