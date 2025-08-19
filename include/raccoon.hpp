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

using reader = std::function<void (std::istream&, uint32_t**, size_t*, size_t*)>;
using saver  = std::function<void (std::ostream&, uint32_t**, size_t, size_t, void*)>;

enum FileType {
    JPEG,
    PNG,
    PPM
};

class RaccoonCanvas {

private:
    uint32_t **pixels;
    size_t height;
    size_t width;

public:

    RaccoonCanvas(uint32_t *pixels, size_t height, size_t width);
    RaccoonCanvas(uint32_t **pixels, size_t height, size_t width);
    
    ~RaccoonCanvas();

    std::shared_ptr<RaccoonCanvas> area(size_t x1, size_t y1, size_t h, size_t b);
    void ascii(size_t scale) const;
    RaccoonCanvas *draw(Drawable &obj);

    RaccoonCanvas *blur();
    RaccoonCanvas *fill(uint32_t color);
    RaccoonCanvas *flip();
    RaccoonCanvas *roll(size_t col);
    RaccoonCanvas *rotate();
    RaccoonCanvas *stretch(uint size = 2);

    RaccoonCanvas *brightness(int perc);
    RaccoonCanvas *negate();

    RaccoonCanvas *hue(int inc);
    RaccoonCanvas *saturation(int inc);
    RaccoonCanvas *value(int inc);

    RaccoonCanvas *read(std::istream &file, FileType ft);
    void save(std::ostream &file, FileType ft, void *args = nullptr);
};
