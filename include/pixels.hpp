#pragma once

#include <cstdint>
#include <cstdlib>

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
    
    bool operator==(const PixelBuffer &other) const;
    bool operator!=(const PixelBuffer &other) const;
    
    constexpr inline const uint32_t at(const uint row, const uint col) const noexcept {
        return (row < this->height && col < this->width)
            ? this->pixels[row][col]
            : this->pixels[0][0];
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
