#pragma once

#include <array>
#include <bits/stdc++.h>
#include <cstdint>
#include <iostream>
#include <map>
#include <vector>
#include <utility>

#define LETTER_HEIGHT 6
#define LETTER_WIDTH  5

using Glyph = std::array<std::array<bool, LETTER_WIDTH>, LETTER_HEIGHT>;

static const std::map<char, Glyph> glyphs = {
    {'a', Glyph{{
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
    }}},
    {'b', Glyph{{
        {1, 0, 0, 0, 0},
        {1, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 1, 1, 0, 0},
    }}},
    {'c', Glyph{{
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 0, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
    }}},
    {'d', Glyph{{
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
    }}},
    {'e', Glyph{{
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 1, 1, 1, 0},
        {1, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
    }}},
    {'f', Glyph{{
        {0, 0, 1, 1, 0},
        {0, 1, 0, 0, 0},
        {1, 1, 1, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 0, 0, 0},
    }}},
    {'g', Glyph{{
        {0, 1, 1, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
    }}},
    {'h', Glyph{{
        {1, 0, 0, 0, 0},
        {1, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
    }}},
    {'i', Glyph{{
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
    }}},
    {'j', Glyph{{
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {1, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
    }}},
    {'k', Glyph{{
        {1, 0, 0, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 1, 0, 0},
        {1, 1, 0, 0, 0},
        {1, 0, 1, 0, 0},
        {1, 0, 0, 1, 0},
    }}},
    {'l', Glyph{{
        {0, 1, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 1, 0},
    }}},
    {'m', Glyph{{
        {0, 0, 0, 0, 0},
        {0, 1, 0, 1, 1},
        {1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1},
    }}},
    {'n', Glyph{{
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
    }}},
    {'o', Glyph{{
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
    }}},
    {'p', Glyph{{
        {1, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 1, 1, 0, 0},
        {1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0},
    }}},
    {'q', Glyph{{
        {0, 1, 1, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 1, 0},
    }}},
    {'r', Glyph{{
        {0, 0, 0, 0, 0},
        {1, 0, 1, 1, 0},
        {1, 1, 0, 0, 1},
        {1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0},
    }}},
    {'s', Glyph{{
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {1, 1, 1, 0, 0},
    }}},
    {'t', Glyph{{
        {0, 1, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {1, 1, 1, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 1, 1, 0, 0},
    }}},
    {'u', Glyph{{
        {0, 0, 0, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
    }}},
    {'v', Glyph{{
        {0, 0, 0, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
    }}},
    {'w', Glyph{{
        {0, 0, 0, 0, 0},
        {1, 0, 0, 0, 1},
        {1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1},
        {0, 1, 1, 1, 1},
    }}},
    {'x', Glyph{{
        {0, 0, 0, 0, 0},
        {1, 0, 1, 0, 0},
        {1, 0, 1, 0, 0},
        {0, 1, 0, 0, 0},
        {1, 0, 1, 0, 0},
        {1, 0, 1, 0, 0},
    }}},
    {'y', Glyph{{
        {0, 0, 0, 0, 0},
        {1, 0, 1, 0, 0},
        {1, 0, 1, 0, 0},
        {1, 0, 1, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 0, 0, 0},
    }}},
    {'z', Glyph{{
        {0, 0, 0, 0, 0},
        {1, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 1, 0},
    }}},

    {'0', Glyph{{
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
    }}},
    {'1', Glyph{{
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 1, 0},
    }}},
    {'2', Glyph{{
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 1, 0},
    }}},
    {'3', Glyph{{
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
    }}},
    {'4', Glyph{{
        {0, 0, 1, 1, 0},
        {0, 1, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 1, 0},
    }}},
    {'5', Glyph{{
        {1, 1, 1, 0, 0},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 0, 0},
        {0, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
    }}},
    {'6', Glyph{{
        {0, 1, 1, 0, 0},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
    }}},
    {'7', Glyph{{
        {1, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 0, 0, 0},
    }}},
    {'8', Glyph{{
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},

    }}},
    {'9', Glyph{{
        {0, 1, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 0, 0},
    }}},

    {',', Glyph{{
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 1, 0, 0},
    }}},
    {'.', Glyph{{
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
    }}},
    {'-', Glyph{{
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
    }}},
    {'!', Glyph{{
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
    }}},
    {'?', Glyph{{
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
    }}},

    {' ', Glyph{{
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
    }}}
};

typedef struct {
    const std::map<char, std::array<std::array<bool, 5>, 6>> * const glyphs;
    unsigned int height;
    unsigned int width;
} Font;

inline Font default_font{&glyphs, LETTER_HEIGHT, LETTER_WIDTH};

inline auto obtain_hex(uint8_t r, uint8_t g, uint8_t b) -> uint32_t {
    return ((0xFF00 | b) << 8 | g) << 8 | r;
}

auto obtain_rgb(uint32_t pixel, uint8_t *r, uint8_t *g, uint8_t *b) -> void {
    *r = (pixel >> (8 * 0)) & 0xFF;
    *g = (pixel >> (8 * 1)) & 0xFF;
    *b = (pixel >> (8 * 2)) & 0xFF;
}

auto get_ppm_dimensions(std::istream &file, size_t *height, size_t *width) -> void {
    std::string format {};
    uint8_t buffer;

    file.seekg(0);
    file >> format >> *width >> *height >> buffer >> buffer >> buffer;
}

auto hsv_2_rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b) -> void {
    float c = v * s;
    float fHPrime = fmod(h / 60.0, 6);
    float fX = c * (1 - fabs(fmod(fHPrime, 2) - 1));
    float fM = v - c;
    
    if(0 <= fHPrime && fHPrime < 1) {
        *r = c;
        *g = fX;
        *b = 0;
    } else if(1 <= fHPrime && fHPrime < 2) {
        *r = fX;
        *g = c;
        *b = 0;
    } else if(2 <= fHPrime && fHPrime < 3) {
        *r = 0;
        *g = c;
        *b = fX;
    } else if(3 <= fHPrime && fHPrime < 4) {
        *r = 0;
        *g = fX;
        *b = c;
    } else if(4 <= fHPrime && fHPrime < 5) {
        *r = fX;
        *g = 0;
        *b = c;
    } else if(5 <= fHPrime && fHPrime < 6) {
        *r = c;
        *g = 0;
        *b = fX;
    } else {
        *r = 0;
        *g = 0;
        *b = 0;
    }
    
    *r += fM;
    *g += fM;
    *b += fM;
}

auto mix_colors(uint32_t first_color, uint32_t second_color, float second_opacity) -> uint32_t  {
    uint8_t r1 = (first_color >> (8 * 0)) & 0xFF;
    uint8_t g1 = (first_color >> (8 * 1)) & 0xFF;
    uint8_t b1 = (first_color >> (8 * 2)) & 0xFF;

    uint8_t r2 = (second_color >> (8 * 0)) & 0xFF;
    uint8_t g2 = (second_color >> (8 * 1)) & 0xFF;
    uint8_t b2 = (second_color >> (8 * 2)) & 0xFF;

    uint8_t nr = (r1 + r2 * second_opacity) / (1 + second_opacity);
    uint8_t ng = (g1 + g2 * second_opacity) / (1 + second_opacity);
    uint8_t nb = (b1 + b2 * second_opacity) / (1 + second_opacity);

    return obtain_hex(nr, ng, nb);
}

auto rgb_2_hsv(uint8_t r, uint8_t g, uint8_t b, float *h, float *s, float *v) -> void {
    r = r / 255.0;
    g = g / 255.0;
    b = b / 255.0;

    double cmax = std::max(r, std::max(g, b));
    double cmin = std::min(r, std::min(g, b));
    double diff = cmax - cmin;
    *h = -1, *s = -1;

    if (cmax == cmin)
        *h = 0;

    else if (cmax == r)
        *h = fmod(60 * ((g - b) / diff) + 360, 360);

    else if (cmax == g)
        *h = fmod(60 * ((b - r) / diff) + 120, 360);

    else if (cmax == b)
        *h = fmod(60 * ((r - g) / diff) + 240, 360);

    if (cmax == 0)
        *s = 0;
    else
        *s = (diff / cmax) * 100;

    *v = cmax * 100;
}

static inline auto sort_points(size_t *x1, size_t *y1, size_t *x2, size_t *y2, size_t *x3, size_t *y3) -> void {
    if (*y1 > *y2) { std::swap(*y1, *y2); std::swap(*x1, *x2); }
    if (*y1 > *y3) { std::swap(*y1, *y3); std::swap(*x1, *x3); }
    if (*y2 > *y3) { std::swap(*y2, *y3); std::swap(*x2, *x3); }
}

class RaccoonCanvas {

private:

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
    uint32_t *pixels;
    size_t height;
    size_t width;

    RaccoonCanvas(uint32_t *pixels, size_t height, size_t width) {
        this->pixels = pixels;
        this->height = height;
        this->width = width;
    }

    ~RaccoonCanvas() = default;

    //-------------- UTILS --------------
    auto fill(uint32_t color) -> void {
        for (size_t i = 0; i < this->height * this->width; i++)
            this->pixels[i] = color;
    }

    auto read_ppm(std::istream &file) -> void {
        uint8_t b {}, g {}, r {};
    
        get_ppm_dimensions(file, &this->height, &this->width);

        for (size_t i = 0; i < this->height * this->width; i++) {
            file >> r >> g >> b;
            this->pixels[i] = obtain_hex(r, g, b);
        }
    }

    auto save_to_ppm(std::ostream &file) -> void {
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
    auto ascii(size_t scale) -> void {
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

    auto flip() -> void {
        for (size_t y = 0; y < this->height; y++)
            for (size_t x = 0; x < this->width / 2; x++)
                std::swap(
                    this->pixels[y*this->width + x],
                    this->pixels[(y + 1)*this->width - x - 1]
                );
    }

    auto rotate() -> int {
        if (this->width != this->height)
        return 1;

        this->transpose_matrix();
        this->reverse_matrix();

        return 0;
    }

    auto stretch(unsigned int size = 2) -> void {
        uint32_t *new_pixels = (uint32_t *)malloc(this->width * this->height * size * sizeof(uint32_t));
    
        for (size_t i = 0; i < this->width * this->height; i++)
            for (unsigned int j = 0; j < size; j++)
                new_pixels[i*size + j] = this->pixels[i];

        this->pixels = new_pixels;
        this->width *= size;
    }

    // -------------- COLOR --------------
    auto hue(int inc) -> void {
        float h, s, v;
        uint8_t r, g, b;

        for (size_t i = 0; i < this->height * this->width; i++) {
            obtain_rgb(this->pixels[i], &r, &g, &b);
            rgb_2_hsv(r, g, b, &h, &s, &v);

            h += inc;
            hsv_2_rgb(h, s, v, &r, &g, &b);
            this->pixels[i] = obtain_hex(r, g, b);
        }
    }

    auto saturation(int inc) -> void {
        float h, s, v;
        uint8_t r, g, b;

        for (size_t i = 0; i < this->height * this->width; i++) {
            obtain_rgb(this->pixels[i], &r, &g, &b);
            rgb_2_hsv(r, g, b, &h, &s, &v);

            s += inc;
            hsv_2_rgb(h, s, v, &r, &g, &b);
            this->pixels[i] = obtain_hex(r, g, b);
        }
    }

    auto value(int inc) -> void {
        float h, s, v;
        uint8_t r, g, b;

        for (size_t i = 0; i < this->height * this->width; i++) {
            obtain_rgb(this->pixels[i], &r, &g, &b);
            rgb_2_hsv(r, g, b, &h, &s, &v);

            v += inc;
            hsv_2_rgb(h, s, v, &r, &g, &b);
            this->pixels[i] = obtain_hex(r, g, b);
        }
    }

    // -------------- SHAPES --------------
    auto circle(size_t xc, size_t yc, float radius, uint32_t color, float opacity = 1) -> void {
        float radius_squared {radius * radius};
        size_t dist {};
        
        for (size_t y = yc - radius; y <= yc + radius; y++) {
            
            dist = xc - radius;
            while(radius_squared < (y - yc) * (y - yc) + (dist - xc) * (dist - xc))
                dist++;
            
            for (size_t x = dist; x <= 2*xc - dist; x++) 
                this->pixels[y * this->width + x] = opacity != 1
                    ? mix_colors(this->pixels[y * this->width + x], color, opacity)
                    : color;
        }
    }

    auto line(size_t x1, size_t y1, size_t x2, size_t y2, uint32_t color) -> void {
        int dx = abs((int)x2 - (int)x1);
        int dy = abs((int)y2 - (int)y1);
        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;
        int err = dx - dy;

        while (x1 != x2 || y1 != y2) {
            this->pixels[y1 * this->width + x1] = color;

            int e2 = err * 2;
            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
    }

    auto rectangle(size_t x1, size_t y1, size_t h, size_t b, uint32_t color, float opacity = 1) -> void {
        for (size_t y = y1; y < y1 + h; y++)
            for (size_t x = x1; x < x1 + b; x++)
                this->pixels[y * this->width + x] = opacity != 1
                    ? mix_colors(this->pixels[y * this->width + x], color, opacity)
                    : color;
    }

    auto text(const std::string &word, size_t x1, size_t y1, size_t font_size, uint32_t color, Font &font = default_font) -> void {
        Glyph glyph {};
        int gx {}, gy {};

        for (size_t i = 0; i < word.length(); i++) {
            gx = x1 + i * font.width * font_size;
            gy = y1;
            glyph = font.glyphs->at(word[i]);

            for (int dy = 0; dy < font.height; dy++) {
                for (int dx = 0; dx < font.width; dx++) {
                    int px = gx + dx*font_size;
                    int py = gy + dy*font_size;
                    
                    if (0 <= px && px < this->width && 0 <= py && py < this->height)
                        if (glyph[dy][dx])
                            this->rectangle(px, py, font_size, font_size, color);
                }
            }
        }
    }

    auto triangle(size_t x1, size_t y1, size_t x2, size_t y2, size_t x3, size_t y3, uint32_t color, float opacity = 1) -> void {
        bool in_triangle {false};
        const size_t start_x = std::min(x1, std::min(x2, x3));

        sort_points(&x1, &y1, &x2, &y2, &x3, &y3);

        this->line(x1, y1, x2, y2, color);
        this->line(x1, y1, x3, y3, color);
        this->line(x2, y2, x3, y3, color);

        for (size_t y = y1 + 1; y < this->height; y++) {
            for (size_t x = start_x; x < this->width; x++) {
                if (in_triangle)
                    this->pixels[y*this->width + x] = opacity != 1
                        ? mix_colors(this->pixels[y * this->width + x], color, opacity)
                        : color;

                if (this->pixels[y*this->width + x + 1] == color) {
                    in_triangle = !in_triangle;
                    
                    if (!in_triangle)
                        break;
                    else
                        x++;
                }
            }
        }
    }
};