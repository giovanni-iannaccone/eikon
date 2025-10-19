#include <fstream>
#include <random>
#include <utility>

#include "../include/formats.hpp"
#include "../include/matrix.hpp"
#include "../include/pixels.hpp"
#include "../include/shapes.hpp"
#include "../include/utils.hpp"

#include "../include/eikon.hpp"

EikonCanvas::EikonCanvas(uint height, uint width)
    : pixels(PixelBuffer(height, width)) {}

EikonCanvas::EikonCanvas(std::istream &file, FileType ft) {
    auto handler = get_format_handler(ft);

    uint height {}, width {};
    handler->get_dimensions(file, &height, &width);

    this->pixels = PixelBuffer(height, width);
    this->read(file, ft);
}

EikonCanvas::EikonCanvas(const std::string &file_name) {
    FileType ft = detect_filetype(file_name);
    auto handler = get_format_handler(ft);

    uint height {}, width {};
    std::ifstream file {file_name, std::ios::in};
    handler->get_dimensions(file, &height, &width);
    
    this->pixels = PixelBuffer(height, width);
    
    this->read(file, ft);
    file.close();
}

EikonCanvas::EikonCanvas(PixelBuffer &pixels)
    : pixels(std::move(pixels)) {}

EikonCanvas::~EikonCanvas() {}

EikonCanvas::EikonCanvas(const EikonCanvas &canvas)
    : pixels(canvas.pixels) {}

EikonCanvas::EikonCanvas(EikonCanvas &&canvas)
    : pixels(std::move(canvas.pixels)) {}

EikonCanvas &EikonCanvas::operator=(const EikonCanvas &canvas) {
    this->pixels = canvas.pixels;
    return *this;
}

EikonCanvas &EikonCanvas::operator=(EikonCanvas &&canvas) {
    this->pixels = std::move(canvas.pixels);    
    return *this;
}

const uint32_t *EikonCanvas::operator[](const uint index) const {
    return this->pixels[index];
}

uint32_t *&EikonCanvas::operator[](const uint index) {
    return this->pixels[index];
}

bool EikonCanvas::operator==(const EikonCanvas &other) const {
    return this->pixels == other.pixels;
}

bool EikonCanvas::operator!=(const EikonCanvas &other) const {
    return this->pixels != other.pixels;
}

EikonCanvas *EikonCanvas::add_noise(uint8_t intensity) {
    uint8_t r {}, g {}, b {};
    uint8_t noise_r {}, noise_g {}, noise_b {};

    std::mt19937 gen = initialize_randomness();

    for (uint y = 0; y < this->height(); y++)
        for (uint x = 0; x < this->width(); x++) {
            get_rgb(this->pixels[y][x], r, g, b);

            noise_r = gen() % (intensity * 2 + 1) - intensity;
            noise_g = gen() % (intensity * 2 + 1) - intensity;
            noise_b = gen() % (intensity * 2 + 1) - intensity;

            r = std::clamp(r + noise_r, 0, 255);
            g = std::clamp(g + noise_g, 0, 255);
            b = std::clamp(b + noise_b, 0, 255);

            this->pixels[y][x] = get_hex(r, g, b);
        }
    
    return this;
}

std::shared_ptr<EikonCanvas> EikonCanvas::area(uint x1, uint y1, uint h, uint b) {
    PixelBuffer pixels_area {h, 0, false};
    
    for (uint i = 0; i < h; i++)
        pixels_area[i] = this->pixels[y1 + i] + x1;

    pixels_area.width = b;
    
    return std::make_shared<EikonCanvas>(
        pixels_area
    );
}

EikonCanvas *EikonCanvas::ascii(uint scale, std::ostream &out) {
    const std::string gradient = " `,^\":;~+_-iIl!?][*}{1)(|\\/tfjrvuncoazxmwqpdbkhXYUJCLQ0OZ#MW&8%B$@";
    
    for (uint y = 0; y < this->height(); y += scale) {
        for (uint x = 0; x < this->width(); x += scale) {
            uint8_t brightness = get_pixel_brightness(this->pixels[y][x]);
            out << gradient[brightness * gradient.length() / 256];
        }

        out << std::endl;
    }

    return this;
}

const uint32_t EikonCanvas::at(uint x, uint y) const {
    return this->pixels.at(x, y);
}

uint32_t &EikonCanvas::at(uint x, uint y) {
    return this->pixels.at(x, y);
}

EikonCanvas *EikonCanvas::blur(uint8_t radius) {
    uint16_t kernel_size = radius * 2 + 1;
    PixelBuffer matrix {kernel_size, kernel_size};

    for (uint y = radius; y < this->height() - radius; y++)
        for (uint x = radius; x < this->width() - radius; x++) {

            for (uint i = 0; i < kernel_size; i++)
                for (uint j = 0; j < kernel_size; j++)
                    matrix[i][j] = this->pixels[y - radius + i][x - radius + j];
            
            this->pixels[y][x] = convolute(matrix, kernel_size);
    
        }

    return this;
}

EikonCanvas *EikonCanvas::brightness(float inc) {
    this->map([inc] (uint32_t &pixel) {
            increase_brightness(pixel, inc);
    });
    
    return this;
}

EikonCanvas *EikonCanvas::chop(int cols) {
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
    return this;
}

EikonCanvas *EikonCanvas::contrast(float inc) {
    uint h {};
    uint8_t r {}, g {}, b {};
    float s {}, i {};

    for (uint y = 0; y < this->height(); y++)
        for (uint x = 0; x < this->width(); x++) {
            get_rgb(this->pixels[y][x], r, g, b);
            rgb_2_hsi(r, g, b, &h, &s, &i);

            i = std::min(1.0f, i * inc);
            hsi_2_rgb(h, s, i, &r, &g, &b);

            this->pixels[y][x] = get_hex(r, g, b);
        }

    return this;
}

EikonCanvas *EikonCanvas::crop(int row) {
    PixelBuffer new_pixels {this->height() - abs(row), this->width()};

    if (row < 0)
        for (uint i = 0; i < this->height() + row; i++)
            std::swap(new_pixels[i], this->pixels[i]);
    else
        for (uint i = row; i < this->height(); i++)
            std::swap(new_pixels[i], this->pixels[i]);
    
    this->pixels = new_pixels;
    return this;
}

EikonCanvas *EikonCanvas::draw(Drawable &obj) {
    obj.draw(this->pixels);
    return this;
}

EikonCanvas *EikonCanvas::equalize() {
    uint32_t hist[256] = {0};
    uint8_t r {}, g {}, b {};

    for (uint y = 0; y < this->height(); y++)
        for (uint x = 0; x < this->width(); x++) {
            get_rgb(this->pixels[y][x], r, g, b);
            uint8_t brightness = 0.3 * r + 0.59 * g + 0.11 * b;
            hist[brightness]++;
        }

    uint32_t cdf[256];
    cdf[0] = hist[0];
    for (uint i = 1; i < 256; i++)
        cdf[i] = cdf[i - 1] + hist[i];

    uint32_t total_pixels = this->height() * this->width();
    float cdf_min = *std::min_element(cdf, cdf + 256);
    float cdf_range = total_pixels - cdf_min;

    for (uint y = 0; y < this->height(); y++)
        for (uint x = 0; x < this->width(); x++) {
            get_rgb(this->pixels[y][x], r, g, b);
            uint8_t brightness = 0.3 * r + 0.59 * g + 0.11 * b;
            
            uint8_t equalized_brightness = static_cast<uint8_t>(
                ((cdf[brightness] - cdf_min) * 255.0f) / cdf_range
            );

            this->pixels[y][x] = get_hex(equalized_brightness, equalized_brightness, equalized_brightness);
        }

    return this;
}

EikonCanvas *EikonCanvas::fill(const uint32_t color) {
    for (uint y = 0; y < this->height(); y++)
        std::memset(this->pixels[y], color, sizeof(uint32_t) * this->width());
    
    return this;
}

EikonCanvas *EikonCanvas::flip() {
    for (uint y = 0; y < this->height() / 2; y++)
        for (uint x = 0; x < this->width(); x++)
            std::swap(
                this->pixels[y][x],
                this->pixels[this->height() - y - 1][x]
            );
    
    return this;
}

EikonCanvas *EikonCanvas::flop() {
    for (uint y = 0; y < this->height(); y++)
        for (uint x = 0; x < this->width() / 2; x++)
            std::swap(
                this->pixels[y][x],
                this->pixels[y][this->width() - x - 1]
            );
    
    return this;
}

PixelBuffer &EikonCanvas::get_pixels() {
    return this->pixels;
}

PixelBuffer EikonCanvas::get_pixels_copy() {
    return {this->pixels};
}

EikonCanvas *EikonCanvas::gray_scale() {
    this->map([] (uint32_t &pixel) {
        to_gray(pixel);
    });

    return this;
}

constexpr uint EikonCanvas::height() const {
    return this->pixels.height;
}

EikonCanvas *EikonCanvas::hue(float inc) {
    uint h {};
    uint8_t r {}, g {}, b {};
    float s {}, v {};
    
    for (uint y = 0; y < this->height(); y++) {
        for (uint x = 0; x < this->width(); x++) {
            get_rgb(this->pixels[y][x], r, g, b);
            rgb_2_hsv(r, g, b, &h, &s, &v);
            
            h *= inc;
            hsv_2_rgb(h, s, v, &r, &g, &b);
            this->pixels[y][x] = get_hex(r, g, b);
        }
    }
    
    return this;
}

EikonCanvas *EikonCanvas::isolate(Channel c) {
    uint32_t mask = 0xFF000000 | (0xFF << c);
    
    this->map([mask] (uint32_t &pixel) {
            pixel &= mask;
    });

    return this;
}

EikonCanvas *EikonCanvas::map(std::function<void (uint32_t &)> f, bool cache_values) {
    if (!cache_values) {

        for (uint y = 0; y < this->height(); y++)
            for (uint x = 0; x < this->width(); x++)
                f(this->pixels[y][x]);

        return this;
    }

    cache value = initialize_cache(this->pixels[0][0], f);
    
    for (uint y = 0; y < this->height(); y++)
        for (uint x = 0; x < this->width(); x++)
            
            if (this->pixels[y][x] == value.input) {
                this->pixels[y][x] = value.output;
            } else {
                value.input = this->pixels[y][x];
                f(this->pixels[y][x]);
                value.output = this->pixels[y][x];
            }
    
    return this;
}

EikonCanvas *EikonCanvas::negate() {    
    this->map([] (uint32_t &pixel) {
        negate_pixel(pixel);
    });
    
    return this;
}

EikonCanvas *EikonCanvas::padding(uint top, uint right, uint bottom, uint left, uint32_t color) {
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
    return this;
}

EikonCanvas *EikonCanvas::raise(uint border_width) {

    for (uint y = 0; y < border_width; y++)
        for (uint x = y; x < this->width() - y; x++)
            increase_brightness(this->pixels[y][x], 1.5f);

    for (uint y = border_width; y > 0; y--)
        for (uint x = y; x < this->width() - y; x++)
            increase_brightness(this->pixels[this->height() - y][x], 0.5f);

    for (uint y = 0; y < this->height() - border_width; y++) {
        for (uint x = 0; x < std::min<uint>(border_width, y); x++)
            increase_brightness(this->pixels[y][x], 1.25f);

        for (uint x = this->width() - std::min<uint>(border_width, y); x < this->width(); x++)
            increase_brightness(this->pixels[y][x], 0.75f);
    }

    for (uint y = this->height() - border_width; y < this->height(); y++)
        for (uint x = 0; x < this->width() - y; x++) {
            increase_brightness(this->pixels[y][x], 1.25f);
            increase_brightness(this->pixels[y][this->width() - x - 1], 0.75f);
        }

    return this;
}

EikonCanvas *EikonCanvas::read(std::istream &file, FileType ft) {
    auto handler = get_format_handler(ft);
    handler->read(file, this->pixels);
    
    return this;
}

EikonCanvas *EikonCanvas::read(const std::string &file_name) {
    FileType ft = detect_filetype(file_name);
    auto handler = get_format_handler(ft);
    
    std::ifstream file {file_name, std::ios::in};
    handler->read(file, this->pixels);

    file.close();
    return this;
}

EikonCanvas *EikonCanvas::roll(int col) {
    uint ecol = col < 0
        ? this->width() + col
        : col;

    for (uint i = 0; i < this->height(); i++)
        std::rotate(
            this->pixels[i],
            this->pixels[i] + this->width() - ecol,
            this->pixels[i] + this->width()
        );

    return this;
}

EikonCanvas *EikonCanvas::rotate() {
    if (this->width() != this->height())
        return this;
    
    transpose_matrix(this->pixels, this->height(), this->width());
    reverse_matrix(this->pixels, this->height(), this->width());
    
    return this;
}

EikonCanvas *EikonCanvas::saturation(float inc) {
    uint h {};
    uint8_t r {}, g {}, b {};
    float s {}, v {};
    
    for (uint y = 0; y < this->height(); y++) {
        for (uint x = 0; x < this->width(); x++) {
            get_rgb(this->pixels[y][x], r, g, b);
            rgb_2_hsv(r, g, b, &h, &s, &v);
            
            s *= inc;
            hsv_2_rgb(h, s, v, &r, &g, &b);
            this->pixels[y][x] = get_hex(r, g, b);
        }
    }
    
    return this;
}

int EikonCanvas::save(std::ostream &file, FileType ft, FormatData *data) const {
    auto handler = get_format_handler(ft);
    return handler->save(file, this->pixels, data);
}

int EikonCanvas::save(const std::string &file_name, FormatData *data) const {
    FileType ft = detect_filetype(file_name);
    auto handler = get_format_handler(ft);
    
    std::ofstream file {file_name, std::ios::out};
    int success = handler->save(file, this->pixels, data);

    file.close();
    return success;
}

EikonCanvas *EikonCanvas::sepia() {
    uint8_t r {}, g {}, b {};

    for (uint y = 0; y < this->height(); y++)
        for (uint x = 0; x < this->width(); x++) {
            get_rgb(this->pixels[y][x], r, g, b);

            this->pixels[y][x] = get_hex(
                0.393 * r + 0.769 * g + 0.189 * b,
                0.349 * r + 0.686 * g + 0.168 * b,
                0.272 * r + 0.534 * g + 0.131 * b
            );
        }

    return this;
}

const std::pair<uint, uint> EikonCanvas::size() const {
    return std::make_pair(this->height(), this->width());
}

EikonCanvas *EikonCanvas::solarize(float perc) {
    uint8_t limit = 2.55f * perc;
    uint8_t r {}, g {}, b {};

    for (uint y = 0; y < this->height(); y++)
        for (uint x = 0; x < this->width(); x++) {
            get_rgb(this->pixels[y][x], r, g, b);

            r = (r > limit) ? (255 - r) : r;
            g = (g > limit) ? (255 - g) : g;
            b = (b > limit) ? (255 - b) : b;

            this->pixels[y][x] = get_hex(r, g, b);
        }
    
    return this;
}

EikonCanvas *EikonCanvas::stretch(uint size) {
    PixelBuffer new_pixels = PixelBuffer(this->height(), this->width() * size);

    for (uint y = 0; y < this->height(); y++)
        for (uint x = 0; x < this->width(); x++)
            for (uint i = 0; i < size; i++)
                new_pixels[y][x * size + i] = this->pixels[y][x];

    this->pixels = new_pixels;
    return this;
}

EikonCanvas *EikonCanvas::value(float inc) {
    uint h {};
    uint8_t r {}, g {}, b {};
    float s {}, v {};

    for (uint y = 0; y < this->height(); y++) {
        for (uint x = 0; x < this->width(); x++) {
            get_rgb(this->pixels[y][x], r, g, b);
            rgb_2_hsv(r, g, b, &h, &s, &v);
            
            v *= inc;
            hsv_2_rgb(h, s, v, &r, &g, &b);
            this->pixels[y][x] = get_hex(r, g, b);
        }
    }

    return this;
}

constexpr uint EikonCanvas::width() const {
    return this->pixels.width;
}
