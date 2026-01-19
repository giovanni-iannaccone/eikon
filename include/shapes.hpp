#pragma once

#include <cstdint>
#include <string>

#include "font.hpp"
#include "pixels.hpp"

namespace eikon {

class Drawable {

public:
    virtual ~Drawable() = default;
    virtual void draw(PixelBuffer &pixels) const = 0;
};

namespace shapes {

class Circle: public Drawable {

protected:
    float radius;
    uint cx, cy;
    uint32_t color;

public: 
    Circle(float radius, uint cx, uint cy, uint32_t color);
    ~Circle() override = default;
    
    void draw(PixelBuffer &pixels) const override;
};

class Ellipse: public Drawable {

protected:
    uint cx, cy;
    uint a, b;
    uint32_t color;
    
public:
    Ellipse(uint cx, uint cy, uint a, uint b, uint32_t color);
    ~Ellipse() override = default;
    
    void draw(PixelBuffer &pixels) const override;
};

class Line: public Drawable {

protected:
    uint x1, y1, x2, y2;
    uint32_t color;

public:
    Line(uint x1, uint y1, uint x2, uint y2, uint32_t color);
    ~Line() override = default;

    void draw(PixelBuffer &pixels) const override;
};

class Rectangle: public Drawable {

protected:
    uint x1, y1, h, b;
    uint32_t color;

public: 
    Rectangle(uint x1, uint y1, uint h, uint b, uint32_t color);
    ~Rectangle() override = default;
    
    void draw(PixelBuffer &pixels) const override;
};

class Text: public Drawable {

protected:
    const std::string &word;
    uint x1, y1, font_size;
    uint32_t color;
    const Font &font;

    virtual void rectangle(PixelBuffer &pixels, uint x, uint y, uint h, uint b) const;

public:
    Text(const std::string &word, uint x1, uint y1, uint font_size, uint32_t color, const Font &font = default_font);
    ~Text() override = default;
    
    void draw(PixelBuffer &pixels) const override;
};

class Triangle : public Drawable {

protected:
    uint x1, y1, x2, y2, x3, y3;
    uint32_t color;

    int cross_product(int px, int py, int qx, int qy, int rx, int ry) const;
    bool is_inside(int px, int py) const;

public:
    Triangle(uint x1, uint y1, uint x2, uint y2, uint x3, uint y3, uint32_t color);
    ~Triangle() override = default;

    void draw(PixelBuffer &pixels) const override;
};

class SafeCircle: public Circle {
public: 
    SafeCircle(float radius, uint cx, uint cy, uint32_t color);
    ~SafeCircle() override = default;
    
    void draw(PixelBuffer &pixels) const override;
};

class SafeEllipse: public Ellipse {
public:
    SafeEllipse(uint cx, uint cy, uint a, uint b, uint32_t color);
    ~SafeEllipse() override = default;
    
    void draw(PixelBuffer &pixels) const override;
};

class SafeLine: public Line {
public:
    SafeLine(uint x1, uint y1, uint x2, uint y2, uint32_t color);
    ~SafeLine() override = default;
    
    void draw(PixelBuffer &pixels) const override;
};

class SafeRectangle: public Rectangle {
public:
    SafeRectangle(uint x1, uint y1, uint h, uint b, uint32_t color);
    ~SafeRectangle() override = default;

    void draw(PixelBuffer &pixels) const override;
};

class SafeText: public Text {
protected:
    void rectangle(PixelBuffer &pixels, uint x, uint y, uint h, uint b) const override;

public:
    SafeText(const std::string &word, uint x1, uint y1, uint font_size, uint32_t color, const Font &font = default_font);
    ~SafeText() override = default;
};

class SafeTriangle : public Triangle {
public:
    SafeTriangle(uint x1, uint y1, uint x2, uint y2, uint x3, uint y3, uint32_t color);
    ~SafeTriangle() override = default;
    
    void draw(PixelBuffer &pixels) const override;
};

} // namespace shapes

} // namespace eikon
