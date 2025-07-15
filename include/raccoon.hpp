#pragma once

#include <array>
#include <bits/stdc++.h>
#include <cstdint>
#include <iostream>
#include <map>
#include <vector>
#include <utility>

#include "font.hpp"
#include "shapes.hpp"
#include "utils.hpp"

class RaccoonCanvas {

private:
    uint32_t *pixels;
    size_t height;
    size_t width;

    auto reverse_matrix() -> void {
        for (size_t y = 0; y < this->height; y++)
            for(size_t x = 0; x < this->height/2; x++)
                std::swap(this->pixels[y*this->height + x], 
                    this->pixels[y*this->height + this->height - x - 1]
                );
    }

    auto transpose_matrix() -> void {
        for (size_t y = 0; y < this->height; y++) 
            for (size_t x = y + 1; x < this->width; x++)
                std::swap(this->pixels[y*this->width + x], 
                    this->pixels[x*this->width + y]
                );
    }

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

    //-------------- UTILS --------------
    RaccoonCanvas* fill(uint32_t color) {
        for (size_t i = 0; i < this->height * this->width; i++)
            this->pixels[i] = color;

        return this;
    }

    RaccoonCanvas* read_ppm(std::istream &file) {
        uint8_t b {}, g {}, r {};
    
        get_ppm_dimensions(file, &this->height, &this->width);

        for (size_t i = 0; i < this->height * this->width; i++) {
            file >> r >> g >> b;
            this->pixels[i] = obtain_hex(r, g, b);
        }

        return this;
    }

    void save_to_ppm(std::ostream &file) {
        file << "P6\n" << this->width << " " << this->height << "\n255\n";

        for (size_t  i = 0; i < this->height * this->width; i++) {
            uint32_t pixel = this->pixels[i];
            
            uint8_t r = (pixel >> (8 * 0)) & 0xFF;
            uint8_t g = (pixel >> (8 * 1)) & 0xFF;
            uint8_t b = (pixel >> (8 * 2)) & 0xFF;

            file << r << g << b;
        }
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

        this->transpose_matrix();
        this->reverse_matrix();

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
            obtain_rgb(this->pixels[i], &r, &g, &b);
            rgb_2_hsv(r, g, b, &h, &s, &v);

            h += inc;
            hsv_2_rgb(h, s, v, &r, &g, &b);
            this->pixels[i] = obtain_hex(r, g, b);
        }

        return this;
    }

    RaccoonCanvas* saturation(int inc) {
        float h, s, v;
        uint8_t r, g, b;

        for (size_t i = 0; i < this->height * this->width; i++) {
            obtain_rgb(this->pixels[i], &r, &g, &b);
            rgb_2_hsv(r, g, b, &h, &s, &v);

            s += inc;
            hsv_2_rgb(h, s, v, &r, &g, &b);
            this->pixels[i] = obtain_hex(r, g, b);
        }

        return this;
    }

    RaccoonCanvas* value(int inc) {
        float h, s, v;
        uint8_t r, g, b;

        for (size_t i = 0; i < this->height * this->width; i++) {
            obtain_rgb(this->pixels[i], &r, &g, &b);
            rgb_2_hsv(r, g, b, &h, &s, &v);

            v += inc;
            hsv_2_rgb(h, s, v, &r, &g, &b);
            this->pixels[i] = obtain_hex(r, g, b);
        }

        return this;
    }
};