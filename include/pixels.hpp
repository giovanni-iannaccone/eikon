#pragma once

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
    
    explicit PixelBuffer(uint32_t height = 0, uint32_t width = 0, bool free = true);
    
    PixelBuffer(const PixelBuffer& copy);
    PixelBuffer(PixelBuffer&& other);
    
    ~PixelBuffer();
    
    PixelBuffer& operator=(const PixelBuffer& copy);
    PixelBuffer& operator=(PixelBuffer&& other);
    
    const uint32_t *operator[](const uint index) const;
    uint32_t *&operator[](const uint index);
    
    bool operator==(const PixelBuffer &other) const;
    bool operator!=(const PixelBuffer &other) const;
    
    const uint32_t at(const uint row, const uint col) const noexcept;
    uint32_t &at(const uint row, const uint col) noexcept;

    uint32_t blend_at(uint y, uint x, uint32_t color);
};
    
} // namespace eikon
