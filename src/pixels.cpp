#include <algorithm>
#include <cstdint>
#include <cstring>
#include <utility>

#include "../include/pixels.hpp"
#include "../include/utils.hpp"

namespace eikon {

PixelBuffer::PixelBuffer(const Size &size, bool owns)
    : size(size), owns_memory(owns)
{
    if (size.height == 0)
        return;
        
    this->pixels = new uint32_t*[size.height];
    
    if (size.width == 0)
        return;
    
    for (uint i = 0; i < size.height; i++)
        this->pixels[i] = new uint32_t[size.width];
}
    
PixelBuffer::~PixelBuffer()
{    
    if (!this->pixels)
        return;
    
    if (this->owns_memory)
        utils::free_pixels(this->pixels, this->size.height);
    else
        delete[] this->pixels;

    this->pixels = nullptr;
}

PixelBuffer::PixelBuffer(const PixelBuffer& copy)
    : size(copy.size), owns_memory(true)
{
    pixels = new uint32_t*[size.height];
    
    for (uint y = 0; y < size.height; y++) {
        pixels[y] = new uint32_t[size.width];
        std::memcpy(pixels[y], copy.pixels[y], sizeof(pixel_t) * size.width);
    }
}

PixelBuffer::PixelBuffer(PixelBuffer&& other)
    : size(other.size), owns_memory(other.owns_memory)
{
    std::swap(this->pixels, other.pixels);
}

PixelBuffer& PixelBuffer::operator=(const PixelBuffer& copy)
{
    if (&copy == this)
        return *this;

    if (this->pixels) {
        if (this->owns_memory)
            utils::free_pixels(this->pixels, this->size.height);
        else
            delete[] this->pixels;
    }

    this->size = copy.size;
    this->owns_memory = copy.owns_memory;

    this->pixels = new uint32_t*[copy.size.height];
    
    for (uint y = 0; y < copy.size.height; y++) {
        this->pixels[y] = new pixel_t[copy.size.width];
        std::memcpy(this->pixels[y], copy.pixels[y], sizeof(pixel_t) * copy.size.width);
    }

    return *this;
}

PixelBuffer& PixelBuffer::operator=(PixelBuffer&& other)
{
    if (this == &other)
        return *this;

    if (this->pixels && this->owns_memory)
        utils::free_pixels(pixels, size.height);
    else
        delete[] pixels;

    this->pixels = other.pixels;
    this->size = other.size;
    this->owns_memory = other.owns_memory;

    other.pixels = nullptr;
    other.size = {0, 0};
    other.owns_memory = false;

    return *this;
}

} // namespace eikon
