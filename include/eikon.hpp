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

typedef std::function<bool (std::istream&, uint32_t**, uint*, uint*)> reader;
typedef std::function<bool (std::ostream&, uint32_t**, uint, uint, void*)> saver;

class EikonCanvas {

private:
    uint32_t **pixels;
    uint height;
    uint width;

public:

    EikonCanvas(uint32_t *pixels, uint height, uint width);
    EikonCanvas(uint32_t **pixels, uint height, uint width);
    
    ~EikonCanvas();
    
    std::shared_ptr<EikonCanvas> area(uint x1, uint y1, uint h, uint b);
    EikonCanvas *ascii(uint scale = 1, std::ostream &out = std::cout);
    EikonCanvas *draw(Drawable &obj);

    EikonCanvas *fill(uint32_t color);
    EikonCanvas *flip();
    EikonCanvas *flop();
    EikonCanvas *roll(uint col);
    EikonCanvas *rotate();
    EikonCanvas *stretch(uint size = 2, uint32_t ***new_pixels = nullptr);

    EikonCanvas *chop(int cols);
    EikonCanvas *chop_and_delete(int cols);
    EikonCanvas *crop(int rows);
    EikonCanvas *crop_and_delete(int rows);

    EikonCanvas *brightness(float inc);
    EikonCanvas *contrast(float inc);
    EikonCanvas *equalize();
    EikonCanvas *gray_scale();
    EikonCanvas *negate();

    EikonCanvas *hue(float inc);
    EikonCanvas *saturation(float inc);
    EikonCanvas *value(float inc);

    EikonCanvas *blur();
    EikonCanvas *sepia();

    EikonCanvas *read(std::istream &file, FileType ft);
    EikonCanvas *read(const std::string &file_name);

    bool save(std::ostream &file, FileType ft, void *args = nullptr);
    bool save(const std::string &file_name, void *args = nullptr);
};
