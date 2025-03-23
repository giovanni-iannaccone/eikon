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
    uint32_t *pixels;
    size_t height;
    size_t width;
} Canvas;

typedef struct {
    const std::map<char, std::vector<std::vector<bool>>> *glyphs;
    unsigned int height;
    unsigned int width;
} Font;

inline Font default_font {&glyphs, LETTER_HEIGHT, LETTER_WIDTH};

auto fill(Canvas &canvas, uint32_t color) -> void;
auto get_ppm_dimensions(std::ifstream &file, Canvas &canvas) -> void;
auto mix_colors(uint32_t first_color, uint32_t second_color, float second_opacity) -> uint32_t;
auto new_canvas(size_t height, size_t width) -> Canvas;
inline auto obtain_hex(uint8_t r, uint8_t g, uint8_t b) -> uint32_t;
auto read_ppm(std::ifstream &file, Canvas &canvas) -> void;
auto save_to_ppm(std::ofstream &file, Canvas &canvas) -> void;

namespace effects {
    auto ascii(Canvas &canvas, size_t scale) -> void;
    auto flip_ppm(Canvas &canvas) -> void;
    auto rotate_ppm(Canvas &canvas) -> int;
    auto stretch_ppm(Canvas &canvas, unsigned int size = 2) -> void;
}

namespace shapes {
    auto circle(Canvas &canvas, size_t xc, size_t yc, float radius, uint32_t color, float opacity = 1) -> void;
    auto line(Canvas &canvas, size_t x1, size_t y1, size_t x2, size_t y2, uint32_t color) -> void;
    auto rectangle(Canvas &canvas, size_t x1, size_t y1, size_t h, size_t b, uint32_t color, float opacity = 1) -> void; 
    auto text(Canvas &canvas, const std::string word, size_t x1, size_t y1, size_t font_size, uint32_t color, Font &font = default_font) -> void;
    auto triangle(Canvas &canvas, size_t x1, size_t y1, size_t x2, size_t y2, size_t x3, size_t y3, uint32_t color, float opacity = 1) -> void;
}

#endif