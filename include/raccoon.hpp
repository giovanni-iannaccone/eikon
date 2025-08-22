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

using reader = std::function<bool (std::istream&, uint32_t**, uint*, uint*)>;
using saver  = std::function<bool (std::ostream&, uint32_t**, uint, uint, void*)>;

enum FileType {
    JPEG,
    PNG,
    PPM
};

class RaccoonCanvas {

private:
    uint32_t **pixels;
    uint height;
    uint width;

public:

    RaccoonCanvas(uint32_t *pixels, uint height, uint width);
    RaccoonCanvas(uint32_t **pixels, uint height, uint width);
    
    ~RaccoonCanvas();

    void delete_all();
    
    std::shared_ptr<RaccoonCanvas> area(uint x1, uint y1, uint h, uint b);
    void ascii(uint scale = 1) const;
    RaccoonCanvas *draw(Drawable &obj);

    RaccoonCanvas *blur();
    RaccoonCanvas *fill(uint32_t color);
    RaccoonCanvas *flip();
    RaccoonCanvas *flop();
    RaccoonCanvas *roll(uint col);
    RaccoonCanvas *rotate();
    RaccoonCanvas *stretch(uint size = 2);

    RaccoonCanvas *brightness(int perc);
    RaccoonCanvas *contrast(int perc);
    RaccoonCanvas *gray_scale();
    RaccoonCanvas *negate();
    RaccoonCanvas *sepia();

    RaccoonCanvas *hue(int inc);
    RaccoonCanvas *saturation(int inc);
    RaccoonCanvas *value(int inc);

    RaccoonCanvas *chop(int cols);
    RaccoonCanvas *chop_and_delete(int cols);
    RaccoonCanvas *crop(int rows);
    RaccoonCanvas *crop_and_delete(int rows);

    RaccoonCanvas *read(std::istream &file, FileType ft);
    bool save(std::ostream &file, FileType ft, void *args = nullptr);
};
