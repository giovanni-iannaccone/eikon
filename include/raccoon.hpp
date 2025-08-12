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
using saver  = std::function<void (std::ostream&, uint32_t*, size_t, size_t, void*)>;

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

    bool delete_data;

public:

    RaccoonCanvas(uint32_t *pixels, size_t height, size_t width, bool delete_data = true)
    : pixels(pixels),
    height(height),
    width(width), 
    delete_data(delete_data) {

        if (this->pixels == nullptr)
            this->pixels = new uint32_t[this->height * this->width];
    }

    ~RaccoonCanvas();

    std::shared_ptr<RaccoonCanvas> area(size_t x1, size_t y1, size_t h, size_t b);
    void ascii(size_t scale);
    RaccoonCanvas *draw(Drawable *obj);

    RaccoonCanvas *fill(uint32_t color);
    RaccoonCanvas *flip();
    RaccoonCanvas *rotate();
    RaccoonCanvas *stretch(unsigned int size = 2);

    RaccoonCanvas *hue(int inc);
    RaccoonCanvas *saturation(int inc);
    RaccoonCanvas *value(int inc);

    RaccoonCanvas *read(std::istream &file, filetype ft);
    void save(std::ostream &file, filetype ft, void *args);
};
