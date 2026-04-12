#pragma once

#include <compare>
#include <cstdint>
#include <cstdlib>

#include "utils.hpp"

namespace eikon {

class PixelBuffer {
private:
    uint32_t** pixels = nullptr;
    
public:
    bool free;
    uint width;
    uint height;
    
    explicit PixelBuffer(uint height = 0, uint width = 0, bool free = true);
    
    PixelBuffer(const PixelBuffer& copy);
    PixelBuffer(PixelBuffer&& other);
    
    ~PixelBuffer();
    
    PixelBuffer& operator=(const PixelBuffer& copy);
    PixelBuffer& operator=(PixelBuffer&& other);
    
    const uint32_t *operator[](const uint index) const;
    uint32_t *&operator[](const uint index);
    
    constexpr inline std::strong_ordering operator<=>(const PixelBuffer &other) const noexcept {
        if (this->height < other.height || this->width < other.width)
            return std::strong_ordering::less;

        else if (this->height > other.height || this->width > other.width)
            return std::strong_ordering::greater;
        
        for (uint y = 0; y < this->height; y++)
            for (uint x = 0; x < this->width; x++)
                if (this->pixels[y][x] < other.pixels[y][x])
                    return std::strong_ordering::less;

                else if (this->pixels[y][x] > other.pixels[y][x])
                    return std::strong_ordering::greater;

        return std::strong_ordering::equal;
    }

    constexpr inline const uint32_t at(const uint row, const uint col) const noexcept {
        return utils::select(
            row < this->height && col < this->width,
            this->pixels[row][col],
            this->pixels[0][0]
         );
    }

    constexpr inline uint32_t &at(const uint row, const uint col) noexcept {
        return (row < this->height && col < this->width)
            ? this->pixels[row][col]
            : this->pixels[0][0];
    }

    uint32_t blend_at(uint y, uint x, uint32_t color);

    inline uint32_t **get_raw() {
        return this->pixels;
    }
};
    
} // namespace eikon
