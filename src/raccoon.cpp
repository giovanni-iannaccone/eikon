#include "../include/raccoon.hpp"

RaccoonCanvas::RaccoonCanvas(uint32_t *pixels, size_t height, size_t width)
: height(height),
width(width) {

    this->pixels = new uint32_t*[height];

    for (size_t i = 0; i < height; i++)
        this->pixels[i] = pixels + i * width;
}

RaccoonCanvas::~RaccoonCanvas() {
    delete[] this->pixels;
}

std::shared_ptr<RaccoonCanvas> RaccoonCanvas::area(size_t x1, size_t y1, size_t h, size_t b) {
    uint32_t **pixels_portion = new uint32_t*[h];
    for (size_t i = y1; i < y1 + h; i++)
        pixels_portion[i] = &this->pixels[i][x1];

    return std::make_shared<RaccoonCanvas>(
        pixels_portion, h, b
    );
}

void RaccoonCanvas::ascii(size_t scale) const {
    for (size_t y = 0; y < this->height; y += scale) {
        for (size_t x = 0; x < this->width; x += scale)
            if (this->pixels[y][x] == 0xFF000000)
                std::cout << " ";
            else 
                std::cout << static_cast<char>(this->pixels[y][x]);
        
        std::cout << "\n";
    }
}

RaccoonCanvas *RaccoonCanvas::brightness(int perc) {
    float inc = 1 + static_cast<float>(perc) / 100;
    uint8_t r {}, g {}, b {};

    for (size_t y = 0; y < this->height; y++) {
        for (size_t x = 0; x < this->width; x++) {
            get_rgb(this->pixels[y][x], &r, &g, &b);

            r = std::min<uint8_t>(255, r * inc);
            g = std::min<uint8_t>(255, g * inc);
            b = std::min<uint8_t>(255, b * inc);

            this->pixels[y][x] = get_hex(r, g, b);
        }
    }

    return this;
}

RaccoonCanvas *RaccoonCanvas::draw(Drawable &obj) {
    obj.draw(this->pixels, this->height, this->width);
    return this;
}

RaccoonCanvas *RaccoonCanvas::fill(uint32_t color) {
    for (size_t y = 0; y < this->height; y++)
        for (size_t x = 0; x < this->width; x++)
            this->pixels[y][x] = color;
    
    return this;
}

RaccoonCanvas *RaccoonCanvas::flip() {
    for (size_t y = 0; y < this->height; y++)
        for (size_t x = 0; x < this->width / 2; x++)
            std::swap(
                this->pixels[y][x],
                this->pixels[y][this->width - x - 1]
            );
    
    return this;
}

RaccoonCanvas *RaccoonCanvas::hue(int inc) {
    float h {}, s {}, v {};
    uint8_t r {}, g {}, b {};
    
    for (size_t y = 0; y < this->height; y++) {
        for (size_t x = 0; x < this->width; x++) {
            get_rgb(this->pixels[y][x], &r, &g, &b);
            rgb_2_hsv(r, g, b, &h, &s, &v);
            
            h += inc;
            hsv_2_rgb(h, s, v, &r, &g, &b);
            this->pixels[y][x] = get_hex(r, g, b);
        }
    }
    
    return this;
}

RaccoonCanvas *RaccoonCanvas::negate() {
    uint8_t r {}, g {}, b {};

    for (size_t y = 0; y < this->height; y++) {
        for (size_t x = 0; x < this->width; x++) {
            get_rgb(this->pixels[y][x], &r, &g, &b);

            r = 255 - r;
            g = 255 - g;
            b = 255 - b;

            this->pixels[y][x] = get_hex(r, g, b);
        }
    }

    return this;
}

RaccoonCanvas *RaccoonCanvas::read(std::istream &file, filetype ft) {
    const std::map<filetype, reader> readers = {
        {JPEG, read_jpeg},
        {PNG,  read_png},
        {PPM,  read_ppm}
    };

    if (!readers.count(ft))
        return nullptr;
    
    readers.at(ft)(file, this->pixels, &this->height, &this->width);
    return this;
}

RaccoonCanvas *RaccoonCanvas::roll(size_t col) {

    for (size_t y = 0; y < this->height; y++)
        std::rotate(
            this->pixels[y], 
            &this->pixels[y][this->width - col], 
            &this->pixels[y][this->width]
        );

    return this;
}

RaccoonCanvas *RaccoonCanvas::rotate() {
    if (this->width != this->height)
        return nullptr;
    
    transpose_matrix(this->pixels, this->height, this->width);
    reverse_matrix(this->pixels, this->height, this->width);
    
    return this;
}

RaccoonCanvas *RaccoonCanvas::saturation(int inc) {
    float h, s, v;
    uint8_t r, g, b;
    
    for (size_t y = 0; y < this->height; y++) {
        for (size_t x = 0; x < this->width; x++) {
            get_rgb(this->pixels[y][x], &r, &g, &b);
            rgb_2_hsv(r, g, b, &h, &s, &v);
            
            s += inc;
            hsv_2_rgb(h, s, v, &r, &g, &b);
            this->pixels[y][x] = get_hex(r, g, b);
        }
    }
    
    return this;
}

void RaccoonCanvas::save(std::ostream &file, filetype ft, void *args) {
    const std::map<filetype, saver> savers = {
        {JPEG, save_jpeg},
        {PNG,  save_png},
        {PPM,  save_ppm}
    };
    
    if (!savers.count(ft))
        savers.at(ft)(file, this->pixels, this->height, this->width, args);
}

RaccoonCanvas *RaccoonCanvas::stretch(uint size) {
    uint32_t *new_pixels {};

    for (size_t y = 0; y < this->height; y++) {
        new_pixels = new uint32_t[this->width * size];
        
        for (size_t x = 0; x < this->width; x++)
            for (uint i = 0; i < size; i++)
                new_pixels[x * size + i] = this->pixels[y][x];

        this->pixels[y] = new_pixels;
    }

    new_pixels = nullptr;
    return this;
}

RaccoonCanvas *RaccoonCanvas::value(int inc) {
    float h, s, v;
    uint8_t r, g, b;

    for (size_t y = 0; y < this->height; y++) {
        for (size_t x = 0; x < this->width; x++) {
            get_rgb(this->pixels[y][x], &r, &g, &b);
            rgb_2_hsv(r, g, b, &h, &s, &v);
            
            v += inc;
            hsv_2_rgb(h, s, v, &r, &g, &b);
            this->pixels[y][x] = get_hex(r, g, b);
        }
    }

    return this;
}
