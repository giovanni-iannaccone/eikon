#include <algorithm>
#include <cstdint>
#include <cstring>
#include <utility>

#include "../include/pixels.hpp"
#include "../include/utils.hpp"

PixelBuffer::PixelBuffer(uint32_t height, uint32_t width, bool free)
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
            free_pixels(this->pixels, this->height);
        else
            delete[] this->pixels;
    }
}

PixelBuffer::PixelBuffer(const PixelBuffer& copy)
    : width(copy.width), height(copy.height), free(copy.free) {
    
    if (this->pixels && this->free)
        free_pixels(this->pixels, this->height);

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
        free_pixels(this->pixels, this->height);

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

bool PixelBuffer::operator==(const PixelBuffer &other) const {
    if (this->height != other.height || this->width != other.width)
        return false;

    for (uint y = 0; y < this->height; y++)
        for (uint x = 0; x < this->width; x++)
            if (this->pixels[y][x] != other.pixels[y][x])
                return false;

    return true;
}

bool PixelBuffer::operator!=(const PixelBuffer &other) const {
    if (this->height != other.height || this->width != other.width)
        return true;

    for (uint y = 0; y < this->height; y++)
        for (uint x = 0; x < this->width; x++)
            if (this->pixels[y][x] != other.pixels[y][x])
                return true;

    return false;
}

const uint32_t PixelBuffer::at(const uint row, const uint col) const noexcept {
    return (row >= this->height || col >= this->width)
        ? pixels[0][0]
        : pixels[row][col];
}

uint32_t& PixelBuffer::at(const uint32_t row, const uint32_t col) noexcept {
    return (row >= this->height || col >= this->width)
        ? pixels[0][0]
        : pixels[row][col];
}
