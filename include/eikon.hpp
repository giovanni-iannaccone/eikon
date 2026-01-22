#pragma once

#include <cstdint>
#include <iostream>

#include "files.hpp"
#include "formats.hpp"
#include "pixels.hpp"
#include "shapes.hpp"
#include "utils.hpp"

namespace eikon {

using format_handler = std::function<std::unique_ptr<FormatHandler> (files::Type)>;

enum Channel: int {
    BLUE = 0,
    GREEN = 1,
    RED = 2
};

class Canvas {

private:
    PixelBuffer pixels;
    format_handler get_handler = get_format_handler;

public:
    explicit Canvas(uint height, uint width);

    Canvas(std::istream &file, files::Type ft);
    Canvas(const std::string &file_name);
    Canvas(PixelBuffer &pixels);
    
    ~Canvas();

    Canvas(const Canvas &canvas);
    Canvas(Canvas &&canvas);

    Canvas &operator=(const Canvas &canvas);
    Canvas &operator=(Canvas &&canvas);

    const uint32_t *operator[](const uint index) const;
    uint32_t *&operator[](const uint index);
    
    bool operator==(const Canvas &other) const;
    bool operator!=(const Canvas &other) const;

    Canvas &operator+(const Canvas &other);
    Canvas &operator-(const Canvas &other);

    constexpr uint height() const noexcept {
        return this->pixels.height;
    }

    constexpr uint width() const noexcept {
        return this->pixels.width;
    }
    
    const std::pair<uint, uint> size() const noexcept;

    void set_format_handler(std::function<std::unique_ptr<FormatHandler> (files::Type)> get_handler);
    
    Canvas &ascii(uint scale = 1, std::ostream &out = std::cout);
    Canvas area(uint x1, uint y1, uint h, uint b);

    template <drawable D>
    Canvas &draw(D &&obj) {
        std::forward<D>(obj).draw(this->pixels);
        return *this;
    }

    template <std::invocable<uint32_t &> F>
    Canvas &map(F &&f, bool cache_values = true) {
        if (!cache_values) {
            for (uint y = 0; y < this->height(); y++)
                for (uint x = 0; x < this->width(); x++)
                    std::invoke(std::forward<F>(f), this->pixels[y][x]);
            
            return *this;
        }
        
        utils::cache::Cache<uint32_t> cache = utils::cache::initialize(this->pixels[0][0], f);
        
        for (uint y = 0; y < this->height(); y++)
            for (uint x = 0; x < this->width(); x++)
                utils::cache::handle(cache, this->pixels[y][x], f);
        
        return *this;
    }
    
    const uint32_t at(uint y, uint x) const noexcept;
    uint32_t &at(uint y, uint x) noexcept;
    
    PixelBuffer &get_pixels();
    
    Canvas concat(const Canvas &other, utils::Axis axis) const;
    Canvas x_concat(const Canvas &other) const;
    Canvas y_concat(const Canvas &other) const;
    
    Canvas &fill(const uint32_t color = 0) noexcept;
    Canvas &flip() noexcept;
    Canvas &flop() noexcept;
    Canvas &padding(uint top, uint right, uint bottom, uint left, uint32_t color);
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
    Canvas &read(const std::string &file_name);

    int save(std::ostream &file, files::Type ft, FormatData *data = nullptr) const;
    int save(const std::string &file_name, FormatData *data = nullptr) const;
};

} // namespace eikon
