#ifndef _RACCOON_HPP_
#define _RACCOON_HPP_

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>

typedef struct {
    uint32_t *pixels;
    size_t height;
    size_t width;
} Canvas;

auto fill(Canvas &canvas, uint32_t color) -> void;
auto get_ppm_dimensions(std::ifstream &file, Canvas &canvas) -> void;
auto read_from_ppm(std::ifstream &file, Canvas &canvas) -> void;
auto save_to_ppm(std::ofstream &file, Canvas &canvas) -> void;

namespace effects {
    auto flip_ppm(Canvas &canvas) -> void;
    auto rotate_ppm(Canvas &canvas) -> int;
}

namespace shapes {
    auto circle(Canvas &canvas, size_t xc, size_t yc, float radius, uint32_t color) -> void;
    auto rectangle(Canvas &canvas, size_t x1, size_t y1, size_t h, size_t b, uint32_t color) -> void; 
    auto triangle(Canvas &canvas, size_t x1, size_t y1, size_t x2, size_t y2, size_t x3, size_t y3, uint32_t color) -> void;
}

#endif