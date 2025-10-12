#pragma once

#include <cmath>
#include <cstdint>

#include "font.hpp"
#include "pixels.hpp"

class Drawable {

public:
    virtual ~Drawable() = default;
    virtual void draw(PixelBuffer &pixels) = 0;
};

class Circle: public Drawable {

private:
    float radius;
    uint cx, cy;
    uint32_t color;

public: 
    Circle(float radius, uint cx, uint cy, uint32_t color);
    void draw(PixelBuffer &pixels) override;
};

class Ellipse: public Drawable {

private:
    uint cx, cy;
    uint a, b;
    uint32_t color;
    
public:
    Ellipse(uint cx, uint cy, uint a, uint b, uint32_t color);
    void draw(PixelBuffer &pixels) override;
};

class Line: public Drawable {

private:
    uint x1, y1, x2, y2;
    uint32_t color;

public:
    Line(uint x1, uint y1, uint x2, uint y2, uint32_t color);
    void draw(PixelBuffer &pixels) override;
};

class Rectangle: public Drawable {

private:
    uint x1, y1, h, b;
    uint32_t color;

public: 
    Rectangle(uint x1, uint y1, uint h, uint b, uint32_t color);
    void draw(PixelBuffer &pixels) override;
};

class Text: public Drawable {

private:
    const std::string &word;
    uint x1, y1, font_size;
    uint32_t color;
    const Font &font;

    void rectangle(PixelBuffer &pixels, uint x, uint y, uint h, uint b);

public:
    Text(const std::string &word, uint x1, uint y1, uint font_size, uint32_t color, const Font &font = default_font);
    void draw(PixelBuffer &pixels) override;
};

class Triangle : public Drawable {
private:
    uint x1, y1, x2, y2, x3, y3;
    uint32_t color;

    int cross_product(int px, int py, int qx, int qy, int rx, int ry) const;
    bool is_inside(int px, int py) const;

public:
    Triangle(uint x1, uint y1, uint x2, uint y2, uint x3, uint y3, uint32_t color);
    void draw(PixelBuffer &pixels) override;
};
