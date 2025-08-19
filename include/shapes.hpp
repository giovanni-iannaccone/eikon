#pragma once

#include <bits/stdc++.h>

#include "font.hpp"
#include "utils.hpp"

class Drawable {

public:
    virtual void draw(uint32_t **pixels, size_t height, size_t width) = 0;
};

class Circle: public Drawable {

private:
    float radius;
    uint32_t color;
    size_t xc, yc;
    float opaqueness;

public: 

    Circle(float radius, size_t xc, size_t yc, uint32_t color, float opaqueness = 1)
        : radius(radius), xc(xc), yc(yc), 
        color(color), opaqueness(opaqueness) {}

    void draw(uint32_t **pixels, size_t height, size_t width) override;
};

class Line: public Drawable {

private:
    size_t x1, y1, x2, y2;
    uint32_t color;

public:

    Line(size_t x1, size_t y1, size_t x2, size_t y2, uint32_t color)
        : x1(x1), y1(y1), x2(x2), y2(y2), color(color) {}

    void draw(uint32_t **pixels, size_t height, size_t width) override;
};

class Rectangle: public Drawable {

private:
    size_t x1, y1, h, b;
    uint32_t color;
    float opaqueness;

public: 

    Rectangle(size_t x1, size_t y1, size_t h, size_t b, uint32_t color, float opaqueness = 1)
        : x1(x1), y1(y1), h(h), b(b),
        color(color), opaqueness(opaqueness) {}

    void draw(uint32_t **pixels, size_t height, size_t width) override;
};

class Text: public Drawable {

private:
    const std::string &word;
    size_t x1, y1, font_size;
    uint32_t color;
    const Font &font;

    void rectangle(uint32_t **pixels, size_t height, size_t width, size_t x, size_t y, size_t h, size_t b);

public:
    Text(const std::string &word, size_t x1, size_t y1, size_t font_size, uint32_t color, const Font &font) 
        : word(word), x1(x1), y1(y1), font_size(font_size), 
        color(color), font(font) {}

    void draw(uint32_t **pixels, size_t height, size_t width) override;
};

class Triangle: public Drawable {

private:
    size_t x1, y1, x2, y2, x3, y3;
    uint32_t color;
    float opaqueness;

    void draw_borders(uint32_t **pixels, size_t height, size_t width);
    
public:
    Triangle(size_t x1, size_t y1, size_t x2, size_t y2, size_t x3, size_t y3, uint32_t color, float opaqueness = 1)
        : x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3),
        color(color), opaqueness(opaqueness) {}

    void draw(uint32_t **pixels, size_t width, size_t height) override;
};
