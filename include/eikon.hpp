#pragma once

#include <cstdint>
#include <iostream>

#include "formats.hpp"
#include "pixels.hpp"
#include "shapes.hpp"
#include "utils.hpp"

namespace eikon {

using format_handler = std::function<std::unique_ptr<FormatHandler> (utils::FileType)>;

enum Channel: int {
    BLUE = 0,
    GREEN = 1,
    RED = 2
};

class Canvas {

private:
    PixelBuffer pixels;
    format_handler get_handler = utils::get_format_handler; 
    
public:
    explicit Canvas(uint height, uint width);

    Canvas(std::istream &file, utils::FileType ft);
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
    
    void set_format_handler(std::function<std::unique_ptr<FormatHandler> (utils::FileType)> get_handler);
    
    Canvas &ascii(uint scale = 1, std::ostream &out = std::cout);
    Canvas area(uint x1, uint y1, uint h, uint b);

    Canvas &draw(const Drawable &obj);
    Canvas &draw(const Drawable &&obj);

    Canvas &map(std::function <void (uint32_t &)> &f, bool cache_values = true);
    Canvas &map(std::function <void (uint32_t &)> &&f, bool cache_values = true);
    
    const uint32_t at(uint x, uint y) const;
    uint32_t &at(uint x, uint y);
    
    PixelBuffer &get_pixels();

    constexpr uint height() const;
    constexpr uint width() const;
    const std::pair<uint, uint> size() const;

    Canvas concat(const Canvas &other, utils::Axis axis) const;
    Canvas x_concat(const Canvas &other) const;
    Canvas y_concat(const Canvas &other) const;
    
    Canvas &fill(const uint32_t color = 0);
    Canvas &flip();
    Canvas &flop();
    Canvas &padding(uint top, uint right, uint bottom, uint left, uint32_t color);
    Canvas &roll(int col);
    Canvas &rotate();
    Canvas &stretch(uint size = 2);

    Canvas &chop(int cols);
    Canvas &crop(int rows);

    Canvas &brightness(float inc);
    Canvas &contrast(float inc);
    Canvas &equalize();
    Canvas &gray_scale();
    Canvas &negate();
    
    Canvas &hue(float inc);
    Canvas &saturation(float inc);
    Canvas &value(float inc);
    
    Canvas &add_noise(uint8_t intensity = 50);
    Canvas &blur(uint8_t radius = 1);
    Canvas &raise(uint border_width);

    Canvas &isolate(Channel c);
    Canvas &sepia();
    Canvas &solarize(float perc = 60.0f);

    Canvas &read(std::istream &file, utils::FileType ft);
    Canvas &read(const std::string &file_name);

    int save(std::ostream &file, utils::FileType ft, FormatData *data = nullptr) const;
    int save(const std::string &file_name, FormatData *data = nullptr) const;
};

} // namespace eikon
