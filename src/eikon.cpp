#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <sys/select.h>
#include <utility>

#include "../include/formats.hpp"
#include "../include/matrix.hpp"
#include "../include/pixels.hpp"
#include "../include/utils.hpp"

#include "../include/eikon.hpp"

namespace eikon {

Canvas::Canvas(uint height, uint width)
    : pixels(PixelBuffer(height, width)) {}

Canvas::Canvas(std::istream &file, files::Type ft) {
    auto handler = get_format_handler(ft);

    uint height, width;
    handler->get_dimensions(file, &height, &width);

    this->pixels = PixelBuffer(height, width);
    this->read(file, ft);
}

Canvas::Canvas(const std::string &file_name) {
    files::Type ft = files::detect_type(file_name);
    auto handler = get_format_handler(ft);
    
    uint height, width;
    std::ifstream file {file_name, std::ios::in};
    handler->get_dimensions(file, &height, &width);
    
    this->pixels = PixelBuffer(height, width);
    
    this->read(file, ft);
    file.close();
}

Canvas::Canvas(PixelBuffer &pixels)
    : pixels(std::move(pixels)) {}

Canvas::~Canvas() {}

Canvas::Canvas(const Canvas &canvas)
    : pixels(canvas.pixels) {}

Canvas::Canvas(Canvas &&canvas)
    : pixels(std::move(canvas.pixels)) {}

Canvas &Canvas::operator=(const Canvas &canvas) {
    this->pixels = canvas.pixels;
    return *this;
}

Canvas &Canvas::operator=(Canvas &&canvas) {
    std::swap(this->pixels, canvas.pixels);    
    return *this;
}

const uint32_t *Canvas::operator[](const uint index) const {
    return this->pixels[index];
}

uint32_t *&Canvas::operator[](const uint index) {
    return this->pixels[index];
}

Canvas &Canvas::operator+(const Canvas &other) {
    if (this->size() != other.size())
        return *this;
    
    for (uint y = 0; y < this->height(); y++) [[likely]]
        for (uint x = 0; x < this->width(); x++) [[likely]]
            this[y][x] += other[y][x];

    return *this;
}

Canvas &Canvas::operator-(const Canvas &other) {
    if (this->size() != other.size())
        return *this;

    for (uint y = 0; y < this->height(); y++) [[likely]]
        for (uint x = 0; x < this->width(); x++) [[likely]]
            this[y][x] -= other[y][x];

    return *this;
}

Canvas &Canvas::add_noise(uint8_t intensity) {
    
    std::mt19937 gen = utils::initialize_randomness();
    uint8_t interval = intensity * 2 + 1;
    
    this->for_each_pixel([&] (uint32_t &pixel) {
        auto [r, g, b] = utils::get_rgb(pixel);
        
        uint8_t noise_r = gen() % interval - intensity;
        uint8_t noise_g = gen() % interval - intensity;
        uint8_t noise_b = gen() % interval - intensity;
        
        r = std::clamp(r + noise_r, 0, 255);
        g = std::clamp(g + noise_g, 0, 255);
        b = std::clamp(b + noise_b, 0, 255);

        pixel = utils::get_hex(r, g, b);
    });
    
    return *this;
}

Canvas Canvas::area(uint x1, uint y1, uint h, uint b) {
    PixelBuffer pixels_area {h, 0, false};
    
    for (uint i = 0; i < h; i++)
        pixels_area[i] = this->pixels[y1 + i] + x1;

    pixels_area.width = b;
    
    return std::move(
        Canvas{pixels_area}
    );
}

Canvas &Canvas::ascii(uint scale, std::ostream &out) {
    constexpr char gradient[] = " `,^\":;~+_-iIl!?][*}{1)(|\\/tfjrvuncoazxmwqpdbkhXYUJCLQ0OZ#MW&8%B$@";
    constexpr uint gradient_lenght = sizeof(gradient) - 1;
    
    for (uint y = 0; y < this->height(); y += scale) [[likely]] {
        for (uint x = 0; x < this->width(); x += scale) [[likely]] {
            uint8_t brightness = utils::get_pixel_brightness(this->pixels[y][x]);
            out << gradient[brightness * gradient_lenght / 256];
        }

        out << std::endl;
    }

    return *this;
}

Canvas &Canvas::blur(uint8_t radius) {
    const uint k = radius * 2 + 1;

    PixelBuffer matrix {k, k};

    for (uint y = radius; y < this->height() - radius; y++) [[likely]] {
        const uint base_y = y - radius;

        for (uint x = radius; x < this->width() - radius; x++) [[likely]] {
            const uint base_x = x - radius;

            for (uint i = 0; i < k; i++)
                std::memcpy(
                    matrix[i],
                    this->pixels[base_y + i] + base_x,
                    k * sizeof(uint32_t)
                );

            this->pixels[y][x] = convolute<PixelBuffer &>(matrix, k);
        }
    }

    return *this;
}

Canvas &Canvas::brightness(float inc) noexcept {

    this->map([&] (uint32_t &pixel) {
        pixel = utils::increase_brightness(pixel, inc);
    });
    
    return *this;
}

Canvas &Canvas::chop(int cols) {
    PixelBuffer new_pixels {this->height(), this->width() - abs(cols)};

    if (cols > 0)
        for (uint y = 0; y < this->height(); y++)
            for (uint x = 0; this->width() - cols; x++)
                new_pixels[y][x] = this->pixels[y][x - cols];
    else
        for (uint y = 0; y < this->height(); y++)
            for (uint x = 0; x < this->width() + cols; x++)
                new_pixels[y][x] = this->pixels[y][x];

    this->pixels = new_pixels;
    return *this;
}
    
Canvas &Canvas::contrast(float inc) noexcept {
    
    this->map([inc] (uint32_t &pixel) {
        auto [r, g, b] = utils::get_rgb(pixel);
        auto [h, s, i] = utils::rgb_2_hsi(r, g, b);
        
        i = std::min(1.0f, i * inc);
        
        pixel = utils::get_hex(
            utils::hsi_2_rgb(h, s, i)
        );
    });

    return *this;
}

Canvas &Canvas::crop(int row) {
    PixelBuffer new_pixels {this->height() - abs(row), this->width()};

    if (row < 0)
        for (uint i = 0; i < this->height() + row; i++)
            std::swap(new_pixels[i], this->pixels[i]);
    else
        for (uint i = row; i < this->height(); i++)
            std::swap(new_pixels[i], this->pixels[i]);
    
    this->pixels = new_pixels;
    return *this;
}

Canvas &Canvas::equalize() {
    uint32_t hist[256] = {0};

    this->for_each_pixel([&] (uint32_t &pixel) {
        auto [r, g, b] = utils::get_rgb(pixel);
        uint8_t brightness = 0.3 * r + 0.59 * g + 0.11 * b;
        hist[brightness]++;
    });

    uint32_t cdf[256];
    cdf[0] = hist[0];
    for (uint i = 1; i < 256; i++)
        cdf[i] = cdf[i - 1] + hist[i];

    uint32_t total_pixels = this->height() * this->width();
    float cdf_min = *std::min_element(cdf, cdf + 256);
    float cdf_range = total_pixels - cdf_min;

    this->for_each_pixel([&] (uint32_t &pixel) {
        auto [r, g, b] = utils::get_rgb(pixel);
        uint8_t brightness = 0.3 * r + 0.59 * g + 0.11 * b;
        
        uint8_t equalized_brightness = static_cast<uint8_t>(
            ((cdf[brightness] - cdf_min) * 255.0f) / cdf_range
        );

        pixel = utils::get_hex(equalized_brightness, equalized_brightness, equalized_brightness);
    });

    return *this;
}

Canvas &Canvas::fill(const uint32_t color) noexcept {
    for (uint y = 0; y < this->height(); y++)
        std::memset(this->pixels[y], color, sizeof(uint32_t) * this->width());
    
    return *this;
}

Canvas &Canvas::flip() noexcept {
    for (uint y = 0; y < this->height() / 2; y++)
        for (uint x = 0; x < this->width(); x++)
            std::swap(
                this->pixels[y][x],
                this->pixels[this->height() - y - 1][x]
            );
    
    return *this;
}

Canvas &Canvas::flop() noexcept {
    for (uint y = 0; y < this->height(); y++)
        for (uint x = 0; x < this->width() / 2; x++)
            std::swap(
                this->pixels[y][x],
                this->pixels[y][this->width() - x - 1]
            );
    
    return *this;
}

Canvas &Canvas::gray_scale() noexcept {

    this->map([] (uint32_t &pixel) {
        auto [r, g, b] = utils::get_rgb(pixel);
        uint8_t gray = 0.30 * r + 0.59 * g + 0.11 * b;
        pixel = utils::get_hex(gray, gray, gray);
    });

    return *this;
}

Canvas &Canvas::hue(float inc) noexcept {
    
    this->map([inc] (uint32_t &pixel) {
        auto [r, g, b] = utils::get_rgb(pixel);
        auto [h, s, v] = utils::rgb_2_hsv(r, g, b);
        
        h *= inc;
        pixel = utils::get_hex(
            utils::hsv_2_rgb(h, s, v)
        );
    });
    
    return *this;
}

Canvas &Canvas::isolate(Channel c) noexcept {
    uint32_t mask = 0xFF000000 | (0xFF << c);
    
    this->map([mask] (uint32_t &pixel) {
            pixel &= mask;
    });

    return *this;
}

Canvas &Canvas::negate() noexcept {    
    this->map([] (uint32_t &pixel) {
        pixel = (pixel & 0xFF000000) | (0x00FFFFFF & ~pixel);
    });
    
    return *this;
}

Canvas &Canvas::padding(uint top, uint right, uint bottom, uint left, uint32_t color) {
    PixelBuffer new_pixels {this->height() + top + bottom, this->width() + left + right};
    
    for (uint i = 0; i < top; i++)
        for (uint j = 0; j < this->width() + left + right; j++)
            new_pixels[i][j] = color;

    for (uint i = top; i < this->height() + top; i++) {
        for (uint j = 0; j < left; j++)
            new_pixels[i][j] = color;

        for (uint j = 0; j < this->width(); j++)
            new_pixels[i][j + left] = this->pixels[i - top][j];

        for (uint j = this->width() + left; j < this->width() + left + right; j++)
            new_pixels[i][j] = color; 
    }

    for (uint i = this->height() + top; i < this->height() + top + bottom; i++)
        for (uint j = 0; j < this->width() + left + right; j++)
            new_pixels[i][j] = color;
    
    this->pixels = new_pixels;
    return *this;
}

Canvas &Canvas::raise(uint border_width) noexcept {

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

Canvas &Canvas::read(std::istream &file, files::Type ft) {
    auto &&handler = get_handler(ft);
    handler->read(file, this->pixels);
    
    return *this;
}

Canvas &Canvas::read(const std::string &file_name) {
    files::Type ft = files::detect_type(file_name);
    
    auto &&handler = get_handler(ft);
    
    std::ifstream file {file_name, std::ios::in};
    handler->read(file, this->pixels);

    file.close();
    return *this;
}

Canvas &Canvas::roll(int col) noexcept {
    uint ecol = utils::select<uint>(col < 0, this->width() + col, col);
    
    for (uint i = 0; i < this->height(); i++)
        std::rotate(
            this->pixels[i],
            this->pixels[i] + this->width() - ecol,
            this->pixels[i] + this->width()
        );

    return *this;
}

Canvas &Canvas::rotate() {
    rotate_matrix(this->pixels, this->height(), this->width());    
    return *this;
}

Canvas &Canvas::saturation(float inc) noexcept {
    
    this->map([inc] (uint32_t &pixel) {
        auto [r, g, b] = utils::get_rgb(pixel);
        auto [h, s, v] = utils::rgb_2_hsv(r, g, b);
        
        s *= inc;
        pixel = utils::get_hex(
            utils::hsv_2_rgb(h, s, v)
        );
    });
    
    return *this;
}

int Canvas::save(std::ostream &file, files::Type ft, FormatData *data) const {
    auto &&handler = get_handler(ft);
    return handler->save(file, this->pixels, data);
}

int Canvas::save(const std::string &file_name, FormatData *data) const {
    files::Type ft = files::detect_type(file_name);
    auto &&handler = get_handler(ft);
    
    std::ofstream file {file_name, std::ios::out};
    int success = handler->save(file, this->pixels, data);

    file.close();
    return success;
}

Canvas &Canvas::sepia() noexcept {

    this->map([] (uint32_t &pixel) {
        auto [r, g, b] = utils::get_rgb(pixel);
        
        pixel = utils::get_hex(
            0.393 * r + 0.769 * g + 0.189 * b,
            0.349 * r + 0.686 * g + 0.168 * b,
            0.272 * r + 0.534 * g + 0.131 * b
        );
    });

    return *this;
}

Canvas &Canvas::solarize(float perc) noexcept {
    uint8_t limit = 2.55 * perc;

    this->map([limit] (uint32_t &pixel) {
        auto [r, g, b] = utils::get_rgb(pixel);
        
        r = (r > limit) ? ~r : r;
        g = (g > limit) ? ~g : g;
        b = (b > limit) ? ~b : b;
        
        pixel = utils::get_hex(r, g, b);
    });
    
    return *this;
}

Canvas &Canvas::stretch(uint size) {
    PixelBuffer new_pixels = PixelBuffer(this->height(), this->width() * size);

    for (uint y = 0; y < this->height(); y++)
        for (uint x = 0; x < this->width(); x++)
            for (uint i = 0; i < size; i++)
                new_pixels[y][x * size + i] = this->pixels[y][x];

    this->pixels = new_pixels;
    return *this;
}

Canvas &Canvas::value(float inc) noexcept {

    this->map([inc] (uint32_t &pixel) {
        auto [r, g, b] = utils::get_rgb(pixel);
        auto [h, s, v] = utils::rgb_2_hsv(r, g, b);
        
        v *= inc;
        pixel = utils::get_hex(
            utils::hsv_2_rgb(h, s, v)
        );
    });

    return *this;
}

Canvas Canvas::x_concat(const Canvas &other) const {
    Canvas result {
        utils::max(this->height(), other.height()),
        this->width() + other.width()
    };
    
    for (uint y = 0; y < result.height(); y++) {
        std::memcpy(result[y], this->pixels[y], this->width() * sizeof(uint32_t));
        std::memcpy(result[y] + this->width(), other[y], other.width() * sizeof(uint32_t));
    }
    
    return result;
}

Canvas Canvas::y_concat(const Canvas &other) const {
    Canvas result {
        this->height() + other.height(),
        utils::max(this->width(), other.width())
    };
    
    for (uint y = 0; y < this->height(); y++)
        std::memcpy(result[y], this->pixels[y], this->width() * sizeof(uint32_t));

    for (uint y = 0; y < other.height(); y++)
        std::memcpy(result[y + this->height()], other[y], other.width() * sizeof(uint32_t));
    
    return result;
}
    
} // namespace eikon
