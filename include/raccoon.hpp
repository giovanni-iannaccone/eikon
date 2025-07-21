#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include <vector>
#include <utility>

#include "font.hpp"
#include "jpeg.hpp"
#include "matrix.hpp"
#include "png.hpp"
#include "ppm.hpp"
#include "shapes.hpp"
#include "utils.hpp"

using reader = std::function<void (std::istream&, uint32_t*, size_t*, size_t*)>;
using saver  = std::function<void (std::ostream&, uint32_t*, size_t, size_t)>;

typedef enum filetype {
    PPM,
    PNG,
    JPEG
};

class RaccoonCanvas {

private:
    uint32_t *pixels;
    size_t height;
    size_t width;

    const std::map<filetype, reader> readers = {
        {JPEG, read_jpeg},
        {PNG,  read_png},
        {PPM,  read_ppm}
    };

    const std::map<filetype, saver> savers = {
        {JPEG, save_jpeg},
        {PNG,  save_png},
        {PPM,  save_ppm}
    };

public:

    RaccoonCanvas(uint32_t *pixels, size_t height, size_t width)
    : pixels(pixels),
    height(height),
    width(width) {}

    ~RaccoonCanvas() {
        delete[] this->pixels;
    }

    RaccoonCanvas*  draw(Drawable *obj) {
        obj->draw(this->pixels, this->height, this->width);
        return this;
    }

    //-------------- UTILS -------------
    RaccoonCanvas* fill(uint32_t color) {
        for (size_t i = 0; i < this->height * this->width; i++)
            this->pixels[i] = color;

        return this;
    }

    RaccoonCanvas* read(std::istream &file, filetype ft) {
        if (!readers.count(ft))
            return nullptr;
        
        readers.at(ft)(file, this->pixels, &this->height, &this->width);
        return this;
    }

    void save(std::ostream &file, filetype ft) {
        if (!savers.count(ft))
            savers.at(ft)(file, this->pixels, this->height, this->width);
    }

    // -------------- EFFECTS --------------
    void ascii(size_t scale) {
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

    RaccoonCanvas*  flip() {
        for (size_t y = 0; y < this->height; y++)
            for (size_t x = 0; x < this->width / 2; x++)
                std::swap(
                    this->pixels[y*this->width + x],
                    this->pixels[(y + 1)*this->width - x - 1]
                );
        
        return this;
    }

    RaccoonCanvas* rotate() {
        if (this->width != this->height)
            return nullptr;

        transpose_flat_matrix(this->pixels, this->height, this->width);
        reverse_flat_matrix(this->pixels, this->height, this->width);

        return this;
    }

    RaccoonCanvas* stretch(unsigned int size = 2) {
        uint32_t *new_pixels = new uint32_t[this->width * this->height * size];
    
        for (size_t i = 0; i < this->width * this->height; i++)
            for (unsigned int j = 0; j < size; j++)
                new_pixels[i*size + j] = this->pixels[i];

        this->pixels = new_pixels;
        this->width *= size;

        return this;
    }

    // -------------- COLOR --------------
    RaccoonCanvas* hue(int inc) {
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

    RaccoonCanvas* saturation(int inc) {
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

    RaccoonCanvas* value(int inc) {
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
};