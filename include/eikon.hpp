#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include <vector>
#include <utility>

#include "bmp.hpp"
#include "font.hpp"
#include "matrix.hpp"
#include "png.hpp"
#include "ppm.hpp"
#include "shapes.hpp"
#include "utils.hpp"

using reader = std::function<bool (std::istream&, uint32_t**, uint*, uint*)>;
using saver  = std::function<bool (std::ostream&, uint32_t**, uint, uint, void*)>;

enum FileType {
    BMP,
    PNG,
    PPM
};

class EikonCanvas {

private:
    uint32_t **pixels;
    uint height;
    uint width;

public:

    EikonCanvas(uint32_t *pixels, uint height, uint width);
    EikonCanvas(uint32_t **pixels, uint height, uint width);
    
    ~EikonCanvas();

    void delete_all();
    
    std::shared_ptr<EikonCanvas> area(uint x1, uint y1, uint h, uint b);
    void ascii(uint scale = 1) const;
    EikonCanvas *draw(Drawable &obj);

    EikonCanvas *blur();
    EikonCanvas *fill(uint32_t color);
    EikonCanvas *flip();
    EikonCanvas *flop();
    EikonCanvas *roll(uint col);
    EikonCanvas *rotate();
    EikonCanvas *stretch(uint size = 2);

    EikonCanvas *chop(int cols);
    EikonCanvas *chop_and_delete(int cols);
    EikonCanvas *crop(int rows);
    EikonCanvas *crop_and_delete(int rows);

    EikonCanvas *brightness(float inc);
    EikonCanvas *contrast(float inc);
    EikonCanvas *equalize();
    EikonCanvas *gray_scale();
    EikonCanvas *negate();

    EikonCanvas *hue(int inc);
    EikonCanvas *saturation(int inc);
    EikonCanvas *value(int inc);

    EikonCanvas *sepia();

    EikonCanvas *read(std::istream &file, FileType ft);
    bool save(std::ostream &file, FileType ft, void *args = nullptr);
};
