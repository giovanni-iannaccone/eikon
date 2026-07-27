#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string_view>
#include <utility>

#include "../include/colors.hpp"
#include "../include/formats.hpp"
#include "../include/matrix.hpp"
#include "../include/pixels.hpp"
#include "../include/utils.hpp"

#include "../include/eikon.hpp"

namespace eikon {

static void load_kernel(PixelBuffer &kernel, const PixelBuffer &original, uint x, uint y, uint size)
{
    for (uint row = 0; row < size; row++)
        std::memcpy(
            kernel[row],
            original[y + row] + x,
            size * sizeof(Canvas::pixel_t)
        );
}

Canvas::Canvas() {}

Canvas::Canvas(uint height, uint width, pixel_t pixel)
    : pixels(PixelBuffer({height, width}))
{
    if (pixel != 0x0)
        this->fill(pixel);
}

Canvas::Canvas(std::istream &file, files::Type ft)
{
    auto handler = get_format_handler(ft);

    Size size {};
    handler->get_dimensions(file, size);
    
    this->pixels = PixelBuffer(size);
    this->read(file, ft);
}

Canvas::Canvas(const std::filesystem::path &file_name)
{
    files::Type ft = files::detect_type(file_name);
    auto handler = get_format_handler(ft);
    
    Size size {};

    std::ifstream file {file_name, std::ios::in | std::ios::binary};
    if (file.fail())
        return;

    handler->get_dimensions(file, size);
    
    this->pixels = {size};
    
    this->read(file, ft);
    file.close();
}

Canvas::Canvas(const PixelBuffer &pixels)
    : pixels(pixels) {}

Canvas::Canvas(PixelBuffer &&pixels)
    : pixels(std::move(pixels)) {}
    
const uint32_t * const &Canvas::operator[](uint index) const
{
    return this->pixels[index];
}

uint32_t *&Canvas::operator[](const uint index)
{
    return this->pixels[index];
}

Canvas &Canvas::operator+=(const Canvas &other) noexcept
{
    if (this->size() != other.size())
        return *this;
    
    for (uint y = 0; y < this->height(); y++) [[likely]]
        for (uint x = 0; x < this->width(); x++) [[likely]]
            this[y][x] += other[y][x];

    return *this;
}

Canvas &Canvas::operator-=(const Canvas &other) noexcept
{
    if (this->size() != other.size())
        return *this;

    for (uint y = 0; y < this->height(); y++) [[likely]]
        for (uint x = 0; x < this->width(); x++) [[likely]]
            this[y][x] -= other[y][x];

    return *this;
}

Canvas Canvas::operator~()
{
    Canvas copy {*this};
    return copy.negate();
}

Canvas &Canvas::add_noise(uint8_t intensity)
{
    const uint8_t interval = intensity * 2 + 1;

    static auto noise_gen = [&] (pixel_t pixel) -> pixel_t {
        auto [r, g, b] = utils::get_rgb(pixel);
        
        uint8_t noise_r = utils::random() % interval - intensity;
        uint8_t noise_g = utils::random() % interval - intensity;
        uint8_t noise_b = utils::random() % interval - intensity;
        
        r = std::clamp(r + noise_r, 0, 255);
        g = std::clamp(g + noise_g, 0, 255);
        b = std::clamp(b + noise_b, 0, 255);

        return utils::get_hex(r, g, b);
    };
    
    this->pixels.for_each(noise_gen);    
    return *this;
}

Canvas Canvas::area(uint x1, uint y1, uint h, uint b)
{
    PixelBuffer pixels_area {{h, 0}, DONT_OWN};
    
    for (uint i = 0; i < h; i++)
        pixels_area[i] = this->pixels[y1 + i] + x1;

    pixels_area.size.width = b;
    
    return Canvas {
        pixels_area
    };
}

Canvas &Canvas::ascii(uint scale, std::ostream &out)
{
    static constexpr char gradient[] = " `,^\":;~+_-iIl!?][*}{1)(|\\/tfjrvuncoazxmwqpdbkhXYUJCLQ0OZ#MW&8%B$@";
    static constexpr uint gradient_lenght = sizeof(gradient) - 1;
    
    for (uint y = 0; y < this->height(); y += scale) [[likely]] {
        for (uint x = 0; x < this->width(); x += scale) [[likely]] {
            uint8_t brightness = utils::get_pixel_brightness(this->pixels[y][x]);
            out << gradient[brightness * gradient_lenght / 256];
        }

        out << std::endl;
    }

    return *this;
}

Canvas &Canvas::blur(uint8_t radius)
{
    constexpr uint TILE_X = 64;
    constexpr uint TILE_Y = 32;

    const uint size = radius * 2 + 1;
    PixelBuffer kernel = {{size, size}};

    const uint x_begin = radius;
    const uint y_begin = radius;
    const uint x_end   = this->width()  - radius;
    const uint y_end   = this->height() - radius;

    for (uint ty = y_begin; ty < y_end; ty += TILE_Y) {
        const uint tile_y_end = std::min(ty + TILE_Y, y_end);

        for (uint tx = x_begin; tx < x_end; tx += TILE_X) {
            const uint tile_x_end = std::min(tx + TILE_X, x_end);

            for (uint y = ty; y < tile_y_end; y++) {
                for (uint x = tx; x < tile_x_end; x++) {
                    load_kernel(kernel, this->pixels, x - radius,y - radius, size);
                    pixels[y][x] = convolute(kernel, size);
                }
            }
        }
    }

    return *this;
}

Canvas &Canvas::brightness(float inc) noexcept
{
    static auto brightness_func = [&] (pixel_t pixel) -> pixel_t {
        return utils::increase_brightness(pixel, inc);
    };

    this->pixels.for_each(brightness_func);
    return *this;
}

Canvas &Canvas::chop(int cols) {
    PixelBuffer new_pixels {{this->height(), this->width() - abs(cols)}};

    if (cols > 0) {
        for (uint y = 0; y < this->height(); y++)
            for (uint x = 0; x < this->width() - cols; x++)
                new_pixels[y][x] = this->pixels[y][x - cols];
    } else {
        for (uint y = 0; y < this->height(); y++)
            for (uint x = 0; x < this->width() + cols; x++)
                new_pixels[y][x] = this->pixels[y][x];
    }
    
    this->pixels = std::move(new_pixels);
    return *this;
}
    
Canvas &Canvas::contrast(float inc) noexcept
{
    static auto contrast_func = [inc] (pixel_t pixel) -> pixel_t {
        auto [r, g, b] = utils::get_rgb(pixel);
        auto [h, s, i] = utils::rgb_2_hsi(r, g, b);
        
        i = std::min(1.0f, i * inc);
        
        return utils::get_hex(
            utils::hsi_2_rgb(h, s, i)
        );
    };

    this->pixels.for_each(contrast_func);
    return *this;
}

Canvas &Canvas::crop(int row)
{
    PixelBuffer new_pixels {{this->height() - abs(row), this->width()}};

    uint max = this->height() + row * (row < 0);

    for (uint i = row * !(row < 0); i < max; i++)
        std::swap(new_pixels[i], this->pixels[i]);
    
    this->pixels = new_pixels;
    return *this;
}

Canvas &Canvas::equalize()
{
    pixel_t hist[256] = {0};

    static auto init_hist = [&] (pixel_t pixel) -> pixel_t {
        auto [r, g, b] = utils::get_rgb(pixel);
        uint8_t brightness = 0.3 * r + 0.59 * g + 0.11 * b;
        hist[brightness]++;

        return pixel;
    };

    this->pixels.for_each(init_hist);
    
    pixel_t cdf[256];
    cdf[0] = hist[0];
    for (uint i = 1; i < 256; i++)
        cdf[i] = cdf[i - 1] + hist[i];

    pixel_t total_pixels = this->height() * this->width();
    float cdf_min = *std::min_element(cdf, cdf + 256);
    float cdf_range = total_pixels - cdf_min;

    static auto equalize_func = [&] (pixel_t pixel) -> pixel_t {
        auto [r, g, b] = utils::get_rgb(pixel);
        uint8_t brightness = 0.3 * r + 0.59 * g + 0.11 * b;
        
        uint8_t equalized_brightness = static_cast<uint8_t>(
            ((cdf[brightness] - cdf_min) * 255.0f) / cdf_range
        );

        return utils::get_hex(equalized_brightness, equalized_brightness, equalized_brightness);
    };
    
    this->pixels.for_each(equalize_func);
    return *this;
}

Canvas &Canvas::fill(const pixel_t color) noexcept
{
    for (uint y = 0; y < this->height(); y++)
        std::fill_n(this->pixels[y], this->width(), color);
    
    return *this;
}

Canvas &Canvas::flip() noexcept
{
    for (uint y = 0; y < this->height() / 2; y++)
        for (uint x = 0; x < this->width(); x++)
            std::swap(
                this->pixels[y][x],
                this->pixels[this->height() - y - 1][x]
            );
    
    return *this;
}

Canvas &Canvas::flop() noexcept
{
    for (uint y = 0; y < this->height(); y++)
        for (uint x = 0; x < this->width() / 2; x++)
            std::swap(
                this->pixels[y][x],
                this->pixels[y][this->width() - x - 1]
            );
    
    return *this;
}

Canvas &Canvas::gray_scale() noexcept
{
    static auto gray_scale_func = [] (pixel_t pixel) -> pixel_t {
        auto && [r, g, b] = utils::get_rgb(pixel);
        uint8_t gray = 0.30 * r + 0.59 * g + 0.11 * b;
        return utils::get_hex(gray, gray, gray);
    };

    this->pixels.for_each(gray_scale_func);
    return *this;
}

Canvas &Canvas::hue(float inc) noexcept
{
    static auto hue_func = [inc] (pixel_t pixel) -> pixel_t {
        auto && [r, g, b] = utils::get_rgb(pixel);
        auto && [h, s, v] = utils::rgb_2_hsv(r, g, b);
        
        h *= inc;
        return utils::get_hex(
            utils::hsv_2_rgb(h, s, v)
        );
    };

    this->pixels.for_each(hue_func);
    return *this;
}

Canvas &Canvas::isolate(Channel c) noexcept
{
    pixel_t mask = 0xFF000000 | (0xFF << c);
    static auto isolate_func = [mask] (pixel_t pixel) -> pixel_t {
        return pixel & mask;
    };

    this->pixels.for_each(isolate_func);
    return *this;
}

Canvas &Canvas::negate() noexcept
{
    static auto negate_func = [] (pixel_t pixel) -> pixel_t {
        return (pixel & 0xFF000000) | (0x00FFFFFF & ~pixel);
    };

    this->pixels.for_each(negate_func);
    return *this;
}

Canvas &Canvas::padding(uint top, uint right, uint bottom, uint left, pixel_t color)
{
    const uint new_height = height() + top + bottom;
    const uint new_width  = width() + left + right;

    PixelBuffer new_buffer({new_height, new_width});

    for (uint y = 0; y < top; ++y)
        for (uint x = 0; x < new_width; ++x)
            new_buffer[y][x] = color;

    for (uint y = 0; y < height(); ++y) {
        uint ny = y + top;

        for (uint x = 0; x < left; ++x)
            new_buffer[ny][x] = color;

        std::memcpy(new_buffer[ny] + left, pixels[y], width() * sizeof(pixel_t));

        for (uint x = left + width(); x < new_width; ++x)
            new_buffer[ny][x] = color;
    }

    for (uint y = height() + top; y < new_height; ++y)
        for (uint x = 0; x < new_width; ++x)
            new_buffer[y][x] = color;

    pixels = std::move(new_buffer);
    return *this;
}
    
Canvas &Canvas::raise(uint border_width) noexcept
{
    for (uint y = 0; y < border_width; y++)
        for (uint x = y; x < this->width() - y; x++)
            utils::increase_brightness(this->pixels[y][x], 1.5f);

    for (uint y = border_width; y > 0; y--)
        for (uint x = y; x < this->width() - y; x++)
            utils::increase_brightness(this->pixels[this->height() - y][x], 0.5f);

    for (uint y = 0; y < this->height() - border_width; y++) {
        for (uint x = 0; x < std::min<uint>(border_width, y); x++)
            utils::increase_brightness(this->pixels[y][x], 1.25f);

        for (uint x = this->width() - std::min<uint>(border_width, y); x < this->width(); x++)
            utils::increase_brightness(this->pixels[y][x], 0.75f);
    }

    for (uint y = this->height() - border_width; y < this->height(); y++)
        for (uint x = 0; x < this->width() - y; x++) {
            utils::increase_brightness(this->pixels[y][x], 1.25f);
            utils::increase_brightness(this->pixels[y][this->width() - x - 1], 0.75f);
        }

    return *this;
}

Canvas &Canvas::read(std::istream &file, files::Type ft)
{
    auto &&handler = get_handler(ft);
    handler->read(file, this->pixels);
    
    return *this;
}

Canvas &Canvas::read(const std::filesystem::path &file_name)
{
    files::Type ft = files::detect_type(file_name);
    
    auto &&handler = get_handler(ft);
    
    std::ifstream file {file_name, std::ios::in | std::ios::binary};
    if (file.fail()) {
        return *this;
    }
    
    handler->read(file, this->pixels);

    file.close();
    return *this;
}

Canvas &Canvas::roll(int col) noexcept
{
    uint ecol = utils::select<uint>(col < 0, this->width() + col, col);

    for (uint i = 0; i < this->height(); i++)
        std::rotate(
            this->pixels[i],
            this->pixels[i] + this->width() - ecol,
            this->pixels[i] + this->width()
        );

    return *this;
}

Canvas &Canvas::rotate()
{
    rotate_matrix(this->pixels, this->height(), this->width());    
    return *this;
}

Canvas &Canvas::saturation(float inc) noexcept
{
    static auto saturation_func = [inc] (pixel_t pixel) -> pixel_t {
        auto && [r, g, b] = utils::get_rgb(pixel);
        auto && [h, s, v] = utils::rgb_2_hsv(r, g, b);
        
        s *= inc;
        return utils::get_hex(
            utils::hsv_2_rgb(h, s, v)
        );
    };
    
    this->pixels.for_each(saturation_func);
    return *this;
}

int Canvas::save(std::ostream &file, files::Type ft, FormatData *data) const
{
    auto &&handler = get_handler(ft);
    return handler->save(file, this->pixels, data);
}

int Canvas::save(const std::filesystem::path &file_name, FormatData *data) const
{
    files::Type ft = files::detect_type(file_name);
    auto &&handler = get_handler(ft);
    
    std::ofstream file {file_name, std::ios::out | std::ios::binary};
    int success = handler->save(file, this->pixels, data);

    file.close();
    return success;
}

Canvas &Canvas::sepia() noexcept
{
    static auto sepia_func = [] (pixel_t pixel) -> pixel_t {
        auto && [r, g, b] = utils::get_rgb(pixel);
        
        return utils::get_hex(
            0.393 * r + 0.769 * g + 0.189 * b,
            0.349 * r + 0.686 * g + 0.168 * b,
            0.272 * r + 0.534 * g + 0.131 * b
        );
    };

    this->pixels.for_each(sepia_func);
    return *this;
}

Canvas &Canvas::solarize(float perc) noexcept
{
    uint8_t limit = 2.55 * perc;
    static auto solarize_func = [limit] (pixel_t pixel) -> pixel_t {
        auto && [r, g, b] = utils::get_rgb(pixel);
        
        return utils::get_hex(
            ~r * (r > limit) | r * !(r > limit),
            ~g * (g > limit) | g * !(g > limit),
            ~b * (b > limit) | b * !(b > limit)
        );
    };
    
    this->pixels.for_each(solarize_func);
    return *this;
}

Canvas &Canvas::stretch(uint size)
{
    PixelBuffer new_pixels {{this->height(), this->width() * size}};

    for (uint y = 0; y < this->height(); y++) [[likely]]
        for (uint x = 0; x < this->width(); x++) [[likely]]
            for (uint i = 0; i < size; i++)
                new_pixels[y][x * size + i] = this->pixels[y][x];

    this->pixels = new_pixels;
    return *this;
}

Canvas &Canvas::value(float inc) noexcept
{
    static auto value_func = [inc] (pixel_t pixel) -> pixel_t {
        auto && [r, g, b] = utils::get_rgb(pixel);
        auto && [h, s, v] = utils::rgb_2_hsv(r, g, b);
        
        v *= inc;
        return utils::get_hex(
            utils::hsv_2_rgb(h, s, v)
        );
    };
    
    this->pixels.for_each(value_func);
    return *this;
}

Canvas Canvas::x_concat(const Canvas &other) const
{
    Canvas result {
        std::max(this->height(), other.height()),
        this->width() + other.width(),
        colors::BLACK
    };
    
    for (uint y = 0; y < result.height(); y++) {
        if (y < this->height())
            std::memcpy(result[y], this->pixels[y], this->width() * sizeof(pixel_t));

        if (y < other.height())
            std::memcpy(result[y] + this->width(), other[y], other.width() * sizeof(pixel_t));
    }
    
    return result;
}

Canvas Canvas::y_concat(const Canvas &other) const
{
    Canvas result {
        this->height() + other.height(),
        std::max(this->width(), other.width()),
        colors::BLACK
    };

    for (uint y = 0; y < this->height(); y++)
        std::memcpy(result[y], this->pixels[y], this->width() * sizeof(pixel_t));

    for (uint y = 0; y < other.height(); y++)
        std::memcpy(result[y + this->height()], other[y], other.width() * sizeof(pixel_t));

    return result;
}
    
} // namespace eikon
