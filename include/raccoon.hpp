#ifndef _RACCOON_HPP_
#define _RACCOON_HPP_

#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <utility>

#include "./data.hpp"

#define LETTER_HEIGHT 6
#define LETTER_WIDTH  5

typedef struct {
    const std::map<char, std::vector<std::vector<bool>>> *glyphs;
    unsigned int height;
    unsigned int width;
} Font;

inline Font default_font {&glyphs, LETTER_HEIGHT, LETTER_WIDTH};

auto get_ppm_dimensions(std::ifstream &file, size_t *height, size_t *width) -> void;
auto mix_colors(uint32_t first_color, uint32_t second_color, float second_opacity) -> uint32_t;
inline auto obtain_hex(uint8_t r, uint8_t g, uint8_t b) -> uint32_t;

class RaccoonCanvas {
    private:

        auto reverse_matrix() -> void;
        auto transpose_matrix() -> void;
    
    public:

        uint32_t *pixels;
        size_t height;
        size_t width;

        RaccoonCanvas(uint32_t *pixels, size_t height, size_t width) {
            this->pixels = pixels;
            this->height = height;
            this->width = width;
        }

        ~RaccoonCanvas() {
            free(this->pixels);
        }

        auto fill(uint32_t color) -> void;
        
        auto read_ppm(std::ifstream &file) -> void;
        auto save_to_ppm(std::ofstream &file) -> void;

        auto ascii(size_t scale) -> void;
        auto flip() -> void;
        auto rotate() -> int;
        auto stretch(unsigned int size = 2) -> void;

        auto circle(size_t xc, size_t yc, float radius, uint32_t color, float opacity = 1) -> void;
        auto line(size_t x1, size_t y1, size_t x2, size_t y2, uint32_t color) -> void;
        auto rectangle(size_t x1, size_t y1, size_t h, size_t b, uint32_t color, float opacity = 1) -> void; 
        auto text(const std::string word, size_t x1, size_t y1, size_t font_size, uint32_t color, Font &font = default_font) -> void;
        auto triangle(size_t x1, size_t y1, size_t x2, size_t y2, size_t x3, size_t y3, uint32_t color, float opacity = 1) -> void;
};

#endif