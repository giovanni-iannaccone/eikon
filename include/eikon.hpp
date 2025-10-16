#pragma once

#include <cstdint>

#include "pixels.hpp"
#include "shapes.hpp"
#include "utils.hpp"

typedef const std::function<int (std::istream&, PixelBuffer&)> reader;
typedef const std::function<int (std::ostream&, const PixelBuffer&, void*)> saver;

enum Channel: int {
    BLUE = 0,
    GREEN = 1,
    RED = 2
};

class EikonCanvas {
private:
    PixelBuffer pixels;
    
public:

    explicit EikonCanvas(uint height, uint width);
    EikonCanvas(const std::string &file_name);
    EikonCanvas(PixelBuffer &pixels);
    
    ~EikonCanvas();

    EikonCanvas(const EikonCanvas &canvas);
    EikonCanvas(EikonCanvas &&canvas);

    EikonCanvas &operator=(const EikonCanvas &canvas);
    EikonCanvas &operator=(EikonCanvas &&canvas);

    bool operator==(const EikonCanvas &other);
    
    EikonCanvas *ascii(uint scale = 1, std::ostream &out = std::cout);

    std::shared_ptr<EikonCanvas> area(uint x1, uint y1, uint h, uint b);
    EikonCanvas *draw(Drawable &obj);
    EikonCanvas *map(std::function <void (uint32_t &)> f, bool cache_values = true);

    uint32_t at(uint x, uint y) const;

    PixelBuffer &get_pixels();
    PixelBuffer get_pixels_copy();

    constexpr uint height() const;
    constexpr uint width() const;
    const std::pair<uint, uint> size() const;
    
    EikonCanvas *fill(const uint32_t color = 0);
    EikonCanvas *flip();
    EikonCanvas *flop();
    EikonCanvas *padding(uint top, uint right, uint bottom, uint left, uint32_t color);
    EikonCanvas *roll(int col);
    EikonCanvas *rotate();
    EikonCanvas *stretch(uint size = 2);

    EikonCanvas *chop(int cols);
    EikonCanvas *crop(int rows);

    EikonCanvas *brightness(float inc);
    EikonCanvas *contrast(float inc);
    EikonCanvas *equalize();
    EikonCanvas *gray_scale();
    EikonCanvas *negate();
    
    EikonCanvas *hue(float inc);
    EikonCanvas *saturation(float inc);
    EikonCanvas *value(float inc);
    
    EikonCanvas *add_noise(uint8_t intensity = 50);
    EikonCanvas *blur(uint8_t radius = 1);
    EikonCanvas *raise(uint border_width);

    EikonCanvas *isolate(Channel c);
    EikonCanvas *sepia();
    EikonCanvas *solarize(float perc = 60.0f);

    EikonCanvas *read(std::istream &file, FileType ft);
    EikonCanvas *read(const std::string &file_name);

    int save(std::ostream &file, FileType ft, void *args = nullptr) const;
    int save(const std::string &file_name, void *args = nullptr) const;
};
