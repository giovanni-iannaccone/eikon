#include "../include/raccoon.hpp"

RaccoonCanvas::RaccoonCanvas(uint32_t *pixels, size_t height, size_t width)
: height(height),
width(width) {

    this->pixels = new uint32_t*[height];

    for (size_t i = 0; i < height; i++)
        this->pixels[i] = pixels + i * width;
}

RaccoonCanvas::RaccoonCanvas(uint32_t **pixels, size_t height, size_t width)
: pixels(pixels),
height(height),
width(width) {};

RaccoonCanvas::~RaccoonCanvas() {
    delete[] this->pixels;
}

std::shared_ptr<RaccoonCanvas> RaccoonCanvas::area(size_t x1, size_t y1, size_t h, size_t b) {
    uint32_t **pixels_portion = new uint32_t*[h];
    for (size_t i = 0; i < h; i++)
        pixels_portion[i] = &this->pixels[y1 + i][x1];

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

RaccoonCanvas *RaccoonCanvas::blur() {
    return this;
}

RaccoonCanvas *RaccoonCanvas::brightness(int perc) {
    float inc = 1 + (static_cast<float>(perc) / 100);
    uint8_t r {}, g {}, b {};

    for (size_t y = 0; y < this->height; y++) {
        for (size_t x = 0; x < this->width; x++) {
            get_rgb(this->pixels[y][x], &r, &g, &b);

            r = static_cast<uint8_t>(std::min(255.0f, r * inc));
            g = static_cast<uint8_t>(std::min(255.0f, g * inc));
            b = static_cast<uint8_t>(std::min(255.0f, b * inc));

            this->pixels[y][x] = get_hex(r, g, b);
        }
    }

    return this;
}

void RaccoonCanvas::delete_all() {
    for (size_t i = 0; i < this->height; i++)
        delete[] this->pixels[i];
}

RaccoonCanvas *RaccoonCanvas::draw(Drawable &obj) {
    obj.draw(this->pixels, this->height, this->width);
    return this;
}

RaccoonCanvas *RaccoonCanvas::fill(uint32_t color) {
    for (size_t i = 0; i < this->height; i++)
        memset(this->pixels[i], color, sizeof(uint32_t) * this->width);
    
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

RaccoonCanvas *RaccoonCanvas::read(std::istream &file, FileType ft) {
    const std::map<FileType, reader> readers = {
        {FileType::JPEG, read_jpeg},
        {FileType::PNG,  read_png},
        {FileType::PPM,  read_ppm}
    };

    if (!readers.count(ft))
        return nullptr;
    
    readers.at(ft)(file, this->pixels, &this->height, &this->width);
    return this;
}

RaccoonCanvas *RaccoonCanvas::roll(size_t col) {

    for (size_t i = 0; i < this->height; i++)
        std::rotate(
            this->pixels[i], 
            &this->pixels[i][this->width - col], 
            &this->pixels[i][this->width]
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

bool RaccoonCanvas::save(std::ostream &file, FileType ft, void *args) {
    const std::map<FileType, saver> savers = {
        {FileType::JPEG, save_jpeg},
        {FileType::PNG,  save_png},
        {FileType::PPM,  save_ppm}
    };

    if (!savers.count(ft))
        return false;
    
    return savers.at(ft)(file, this->pixels, this->height, this->width, args);
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

    this->width *= size;
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
