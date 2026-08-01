#pragma once

#include <compare>
#include <concepts>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <utility>

#include "files.hpp"
#include "formats.hpp"
#include "pixels.hpp"
#include "shapes.hpp"
#include "utils.hpp"

namespace eikon {

enum Channel: uint8_t {
    BLUE = 0,
    GREEN = 1,
    RED = 2
};

template<class F>
concept PixelMapper =
    std::regular_invocable<F, uint32_t> &&
    std::same_as<
        std::invoke_result_t<F, uint32_t>,
        uint32_t>;
    
class Canvas {

private:
    PixelBuffer pixels;
    format_handler get_handler = get_format_handler;
    
public:
    using pixel_t = PixelBuffer::pixel_t;

    Canvas();
    Canvas(uint height, uint width, pixel_t pixel = 0x0);

    Canvas(std::istream &file, files::Type ft);
    Canvas(const std::filesystem::path& file_name);

    Canvas(const PixelBuffer &pixels);
    Canvas(PixelBuffer &&pixels);
    
    ~Canvas() = default;

    Canvas(const Canvas& other) = default;
    Canvas(Canvas&& other) noexcept = default;
    
    Canvas& operator=(const Canvas& other) = default;
    Canvas& operator=(Canvas&& other) noexcept = default;
    
    const pixel_t * const &operator[](uint index) const;
    pixel_t *&operator[](uint index);

    Canvas &operator+=(const Canvas &other) noexcept;
    Canvas &operator-=(const Canvas &other) noexcept;

    Canvas operator~();

    constexpr bool operator==(const Canvas &other) const noexcept
    {
        return this->pixels == other.pixels;
    }
    
    constexpr std::strong_ordering operator<=>(const Canvas &other) const noexcept
    {
        return this->pixels <=> other.pixels;
    }

    constexpr uint height() const noexcept
    {
        return this->pixels.size.height;
    }

    constexpr uint width() const noexcept
    { 
        return this->pixels.size.width;
    }
    
    constexpr Size size() const noexcept
    {
        return this->pixels.size;
    }

    constexpr pixel_t at(uint y, uint x) const noexcept
    {
        return this->pixels.at(x, y);
    }
    
    pixel_t &at(uint y, uint x) noexcept
    {
        return this->pixels.at(y, x);
    }

    const PixelBuffer &get_pixels() const noexcept
    {
        return this->pixels;
    }

    PixelBuffer &get_pixels() noexcept
    {
        return this->pixels;
    }

    void set_format_handler(format_handler get_handler) noexcept
    {
        this->get_handler = std::move(get_handler);
    }

    template <typename F>
    Canvas& apply(F&& f)
    {
        f(this->pixels);
        return *this;
    }

    template <drawable D>
    Canvas &draw(D &&obj)
    {
        std::forward<D>(obj).draw(pixels);
        return *this;
    }

    template <bool cache_values = true, PixelMapper F>
    Canvas& map(F&& f)
    {
        if constexpr (!cache_values) {
            for_each_pixel(std::forward<F>(f));
            
        } else {
            utils::Cache<pixel_t, pixel_t> cache(std::forward<F>(f));
            
            this->pixels.for_each([&cache](pixel_t pixel) -> pixel_t {
                return cache[pixel];
            });
        }
        
        return *this;    
    }
    
    Canvas x_concat(const Canvas &other) const;
    Canvas y_concat(const Canvas &other) const;
    
    Canvas concat(const Canvas &other, utils::Axis axis) const {
        if (axis == utils::Axis::X)
            return x_concat(other);
        else
            return y_concat(other);
    }

    Canvas &ascii(uint scale = 1, std::ostream &out = std::cout);    
    Canvas area(uint x1, uint y1, uint h, uint b);

    Canvas &fill(const pixel_t color = 0) noexcept;
    Canvas &flip() noexcept;
    Canvas &flop() noexcept;
    Canvas &padding(uint top, uint right, uint bottom, uint left, pixel_t color);
    Canvas &roll(int col) noexcept;
    Canvas &rotate();
    Canvas &stretch(uint size = 2);

    Canvas &chop(int cols);
    Canvas &crop(int rows);

    Canvas &brightness(float inc) noexcept;
    Canvas &contrast(float inc) noexcept;
    Canvas &equalize();
    Canvas &gray_scale() noexcept;
    Canvas &negate() noexcept;
    
    Canvas &hue(float inc) noexcept;
    Canvas &saturation(float inc) noexcept;
    Canvas &value(float inc) noexcept;
    
    Canvas &add_noise(uint8_t intensity = 50);
    Canvas &blur(uint8_t radius = 1);
    Canvas &raise(uint border_width) noexcept;

    Canvas &isolate(Channel c) noexcept;
    Canvas &sepia() noexcept;
    Canvas &solarize(float perc = 60.0f) noexcept;

    Canvas &read(std::istream &file, files::Type ft);
    Canvas &read(const std::filesystem::path& file_name);

    int save(std::ostream &file, files::Type ft, FormatData *data = nullptr) const;
    int save(const std::filesystem::path& file_name, FormatData *data = nullptr) const;
};

} // namespace eikon
