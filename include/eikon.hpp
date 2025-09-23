#pragma once

#include <cstdint>
#include <map>
#include <random>
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
    EikonCanvas(const std::string &file_name, uint32_t ***pixels = nullptr, uint *height = nullptr, uint *width = nullptr);
    
    ~EikonCanvas();

    EikonCanvas(const EikonCanvas &canvas);
    EikonCanvas(EikonCanvas &&canvas);

    EikonCanvas &operator=(const EikonCanvas &canvas);
    EikonCanvas &operator=(EikonCanvas &&canvas);

    bool operator==(const EikonCanvas &other);
    
    void free_all();
    
    std::shared_ptr<EikonCanvas> area(uint x1, uint y1, uint h, uint b);
    EikonCanvas *ascii(uint scale = 1, std::ostream &out = std::cout);
    EikonCanvas *draw(Drawable &obj);
    uint32_t get_pixel(uint x, uint y);
    EikonCanvas *map(const std::function <void (uint32_t &)> &f);

    EikonCanvas *fill(uint32_t color);
    EikonCanvas *flip();
    EikonCanvas *flop();
    EikonCanvas *padding(uint top, uint right, uint bottom, uint left, uint32_t color, uint32_t ***pixels = nullptr);
    EikonCanvas *roll(int col);
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
    
    EikonCanvas *add_noise(uint intensity = 50);
    EikonCanvas *blur(uint8_t radius = 1);
    EikonCanvas *raise(uint border_width);
    
    EikonCanvas *sepia();
    EikonCanvas *solarize(float perc = 60.0f);

    EikonCanvas *read(std::istream &file, FileType ft);
    EikonCanvas *read(const std::string &file_name);

    bool save(std::ostream &file, FileType ft, void *args = nullptr);
    bool save(const std::string &file_name, void *args = nullptr);
};
