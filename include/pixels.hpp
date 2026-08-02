#pragma once

#include <compare>
#include <cstdint>
#include <cstdlib>
#include <sys/cdefs.h>

#include "utils.hpp"

namespace eikon {

constexpr bool OWN = true;
constexpr bool DONT_OWN = false;

struct Size {
    uint height = 0;
    uint width  = 0;

    constexpr bool operator==(const Size& other) const noexcept {
        return height == other.height && width == other.width;
    }
    
    constexpr std::strong_ordering operator<=>(const Size &other) const noexcept {
        if (auto cmp = height <=> other.height; cmp != 0)
            return cmp;

        return width <=> other.width;
    }

    constexpr bool has_any_zero() const noexcept
    {
        return this->height == 0 || this->width == 0;
    }
};

class PixelBuffer {
private:
    uint32_t** pixels = nullptr;
    
public:
    bool owns_memory;
    Size size {};

    using pixel_t = uint32_t;
    
    PixelBuffer(const Size &size = {0, 0}, bool owns = OWN);
    
    PixelBuffer(const PixelBuffer& copy);
    PixelBuffer(PixelBuffer&& other);
    
    ~PixelBuffer();
    
    PixelBuffer& operator=(const PixelBuffer& copy);
    PixelBuffer& operator=(PixelBuffer&& other);
    
    const pixel_t *operator[](const uint index) const noexcept
    {
        return this->pixels[index];
    }

    pixel_t *&operator[](const uint index) noexcept
    {
        return this->pixels[index];
    }

    constexpr bool operator==(const PixelBuffer& other) const noexcept
    {
        if (this->size != other.size)
            return false;
        
        for (uint y = 0; y < this->size.height; ++y)
            if (std::memcmp(this->pixels[y], other.pixels[y], this->size.width * sizeof(pixel_t)))
                return false;
        
        return true;
    }

    constexpr std::strong_ordering operator<=>(const PixelBuffer& other) const noexcept
    {
        if (auto cmp = this->size <=> other.size; cmp != 0)
            return cmp;
        
        for (uint y = 0; y < this->size.height; ++y)
            for (uint x = 0; x < this->size.width; ++x)
                if (auto cmp = pixels[y][x] <=> other.pixels[y][x]; cmp != 0)
                    return cmp;

        return std::strong_ordering::equal;
    }

    constexpr uint height() const noexcept
    {
        return this->size.height;
    }

    constexpr uint width() const noexcept
    {
        return this->size.width;
    }

    constexpr pixel_t at(const uint row, const uint col) const noexcept
    {
        return utils::select(
            row < this->size.height && col < this->size.width,
            this->pixels[row][col],
            this->pixels[0][0]
         );
    }

    constexpr pixel_t &at(const uint row, const uint col) noexcept
    {
        return (row < this->size.height && col < this->size.width)
            ? this->pixels[row][col]
            : this->pixels[0][0];
    }

    pixel_t blend_at(uint y, uint x, pixel_t src) noexcept
    {
        pixel_t dst = pixels[y][x];
        pixel_t a = src >> 24;
        
        if (a == 0)
            return dst;

        if (a == 0xFF)
            return pixels[y][x] = src & 0x00FFFFFF;

        pixel_t rb_dst = dst & 0x00FF00FF;
        pixel_t g_dst  = dst & 0x0000FF00;

        pixel_t rb_src = src & 0x00FF00FF;
        pixel_t g_src  = src & 0x0000FF00;
        
        rb_dst += (((rb_src - rb_dst) * a) >> 8) & 0x00FF00FF;
        g_dst  += (((g_src  - g_dst ) * a) >> 8) & 0x0000FF00;
        
        return pixels[y][x] = (rb_dst & 0x00FF00FF) | (g_dst & 0x0000FF00);
    }

    pixel_t **data() noexcept
    {
        return this->pixels;
    }

    template <auto func>
    requires std::invocable<decltype(func), pixel_t&>
    HOT FORCE_INLINE void for_each() noexcept
    {
        const uint max = this->size.width - this->size.width % 10;
        uint x;
        
        FAST_LOOP for (uint y = 0; y < this->size.height; y++) [[likely]] {
            uint * __restrict row = this->pixels[y];
            
            for (x = 0; x < max; x += 10) [[likely]] {
                row[x + 0] = func(row[x + 0]);
                row[x + 1] = func(row[x + 1]);
                row[x + 2] = func(row[x + 2]);
                row[x + 3] = func(row[x + 3]);
                row[x + 4] = func(row[x + 4]);
                row[x + 5] = func(row[x + 5]);
                row[x + 6] = func(row[x + 6]);
                row[x + 7] = func(row[x + 7]);
                row[x + 8] = func(row[x + 8]);
                row[x + 9] = func(row[x + 9]);
            }

            for (; x < this->size.width; x++)
                row[x] = func(row[x]);
        }
    }

    template <std::invocable<uint32_t&> F>
    HOT FORCE_INLINE void for_each(F&& func) noexcept
    {
        const uint max = this->size.width - this->size.width % 10;
        uint x;
        
        FAST_LOOP for (uint y = 0; y < this->size.height; y++) [[likely]] {
            pixel_t * __restrict row = this->pixels[y];

            for (x = 0; x < max; x += 10) [[likely]] {
                row[x + 0] = func(row[x + 0]);
                row[x + 1] = func(row[x + 1]);
                row[x + 2] = func(row[x + 2]);
                row[x + 3] = func(row[x + 3]);
                row[x + 4] = func(row[x + 4]);
                row[x + 5] = func(row[x + 5]);
                row[x + 6] = func(row[x + 6]);
                row[x + 7] = func(row[x + 7]);
                row[x + 8] = func(row[x + 8]);
                row[x + 9] = func(row[x + 9]);
            }

            for (; x < this->size.width; x++)
                row[x] = func(row[x]);
        }
    }
};

} // namespace eikon
