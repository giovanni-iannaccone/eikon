#include "../include/raccoon.hpp"

Pixels::~Pixels() {
    delete[] this->pixels_ptrs;
}

uint32_t*& Pixels::operator[] (size_t idx) {
    return this->pixels_ptrs[idx];
}

const uint32_t* Pixels::operator[] (size_t idx) const {
    return this->pixels_ptrs[idx];
}

RaccoonCanvas::~RaccoonCanvas() {
    if (this->delete_data) {
        delete[] this->pixels;
    }
}

std::shared_ptr<RaccoonCanvas> RaccoonCanvas::area(size_t x1, size_t y1, size_t h, size_t b) {
    uint32_t *pixel_portion {this->pixels + y1 * this->width + x1};

    return std::make_shared<RaccoonCanvas>(
        pixel_portion, h, b, this->height, this->width
    );
}

void RaccoonCanvas::ascii(size_t scale) const {
    for (size_t y = 0; y < this->height; y += scale) {
        for (size_t x = 0; x < this->width; x += scale) {
            if (this->pixels[y*this->width + x] == 0xFF000000)
                std::cout << " ";
            else 
                std::cout << static_cast<char>(this->pixels[y*this->width + y]);
        }
        
        std::cout << "\n";
    }
 }

RaccoonCanvas *RaccoonCanvas::draw(Drawable &obj) {
    obj.draw(this->pixels, this->height, this->width);
    return this;
}

RaccoonCanvas *RaccoonCanvas::fill(uint32_t color) {
    for (size_t i = 0; i < this->height * this->width; i++)
        this->pixels[i] = color;
    
    return this;
}

RaccoonCanvas *RaccoonCanvas::flip() {
    for (size_t y = 0; y < this->height; y++)
        for (size_t x = 0; x < this->width / 2; x++)
            std::swap(
                this->pixels[y*this->width + x],
                this->pixels[(y + 1)*this->width - x - 1]
            );
    
    return this;
}

RaccoonCanvas *RaccoonCanvas::hue(int inc) {
    float h, s, v;
    uint8_t r, g, b;
    
    for (size_t i = 0; i < this->height * this->width; i++) {
        get_rgb(this->pixels[i], &r, &g, &b);
        rgb_2_hsv(r, g, b, &h, &s, &v);
        
        h += inc;
        hsv_2_rgb(h, s, v, &r, &g, &b);
        this->pixels[i] = get_hex(r, g, b);
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

RaccoonCanvas *RaccoonCanvas::rotate() {
    if (this->width != this->height)
        return nullptr;
    
    transpose_flat_matrix(this->pixels, this->height, this->width);
    reverse_flat_matrix(this->pixels, this->height, this->width);
    
    return this;
}

RaccoonCanvas *RaccoonCanvas::saturation(int inc) {
    float h, s, v;
    uint8_t r, g, b;
    
    for (size_t i = 0; i < this->height * this->width; i++) {
        get_rgb(this->pixels[i], &r, &g, &b);
        rgb_2_hsv(r, g, b, &h, &s, &v);
        
        s += inc;
        hsv_2_rgb(h, s, v, &r, &g, &b);
        this->pixels[i] = get_hex(r, g, b);
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

RaccoonCanvas *RaccoonCanvas::stretch(unsigned int size = 2) {
    uint32_t *new_pixels = new uint32_t[this->width * this->height * size];
    
    for (size_t i = 0; i < this->width * this->height; i++)
        for (unsigned int j = 0; j < size; j++)
            new_pixels[i*size + j] = this->pixels[i];
    
    this->pixels = new_pixels;
    this->width *= size;
    
    return this;
}

RaccoonCanvas *RaccoonCanvas::value(int inc) {
    float h, s, v;
    uint8_t r, g, b;

    for (size_t i = 0; i < this->height * this->width; i++) {
        get_rgb(this->pixels[i], &r, &g, &b);
        rgb_2_hsv(r, g, b, &h, &s, &v);
        
        v += inc;
        hsv_2_rgb(h, s, v, &r, &g, &b);
        this->pixels[i] = get_hex(r, g, b);
    }
    
    return this;
}
