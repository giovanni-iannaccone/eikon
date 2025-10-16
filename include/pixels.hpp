#pragma once

#include <algorithm>
#include <cstdint>

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
    
    uint32_t& at(const uint32_t row, const uint32_t col) noexcept;
};
