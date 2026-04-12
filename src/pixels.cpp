#include <algorithm>
#include <cstdint>
#include <cstring>
#include <utility>

#include "../include/pixels.hpp"
#include "../include/utils.hpp"

namespace eikon {

PixelBuffer::PixelBuffer(uint height, uint width, bool free)
    : height(height), width(width), free(free) {
    
    if (height != 0) {
        this->pixels = new uint32_t*[height];

        if (width != 0)
            for (uint i = 0; i < height; i++)
                this->pixels[i] = new uint32_t[width];
    }
}

PixelBuffer::~PixelBuffer() {    
    if (this->pixels) {
        if (this->free)
            utils::free_pixels(this->pixels, this->height);
        else
            delete[] this->pixels;
    }
}

PixelBuffer::PixelBuffer(const PixelBuffer& copy)
    : width(copy.width), height(copy.height), free(copy.free) {
    
    if (this->pixels && this->free)
        utils::free_pixels(this->pixels, this->height);

    this->pixels = new uint32_t*[copy.height];
    
    for (uint y = 0; y < copy.height; y++) {
        this->pixels[y] = new uint32_t[copy.width];
        std::memcpy(this->pixels[y], copy.pixels[y], sizeof(uint32_t) * copy.width);
    }
}

PixelBuffer::PixelBuffer(PixelBuffer&& other)
    : width(other.width), height(other.height), free(other.free) {

    std::swap(this->pixels, other.pixels);
}

PixelBuffer& PixelBuffer::operator=(const PixelBuffer& copy) {    
    if (&copy == this)
        return *this;

    if (this->pixels && this->free)
        utils::free_pixels(this->pixels, this->height);

    this->height = copy.height;
    this->width  = copy.width;
    this->free   = copy.free;

    this->pixels = new uint32_t*[copy.height];
    
    for (uint y = 0; y < copy.height; y++) {
        this->pixels[y] = new uint32_t[copy.width];
        std::memcpy(this->pixels[y], copy.pixels[y], sizeof(uint32_t) * copy.width);
    }

    return *this;
}

PixelBuffer& PixelBuffer::operator=(PixelBuffer&& other) {        
    this->height = other.height;
    this->width  = other.width;
    this->free   = other.free;
    
    std::swap(this->pixels, other.pixels);
    return *this;
}

const uint32_t *PixelBuffer::operator[](const uint index) const {
    return this->pixels[index];
}

uint32_t *&PixelBuffer::operator[](const uint index) {
    return this->pixels[index];
}

uint32_t PixelBuffer::blend_at(uint y, uint x, uint32_t color) {
    auto [r1, g1, b1] = utils::get_rgb(this->pixels[y][x]);
    auto [r2, g2, b2] = utils::get_rgb(color);

    uint8_t a = (color >> (8 * 3)) & 0xFF;

    uint8_t nr = (a * r2 + (255 - a) * r1) / 255;
    uint8_t ng = (a * g2 + (255 - a) * g1) / 255;
    uint8_t nb = (a * b2 + (255 - a) * b1) / 255;

    this->pixels[y][x] = utils::get_hex(nr, ng, nb);
    return this->pixels[y][x];
}

} // namespace eikon
