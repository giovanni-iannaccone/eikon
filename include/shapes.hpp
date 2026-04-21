#pragma once

#include <cstdint>
#include <string>
#include <type_traits>

#include "font.hpp"
#include "pixels.hpp"

namespace eikon {

template <typename T>
class Drawable {

protected:
    Drawable() = default;
    ~Drawable() = default;

public:
    inline void draw(PixelBuffer &pixels) const {
        static_cast<const T*>(this)->impl(pixels);
    }
};

template <typename T>
concept drawable = std::is_base_of_v<Drawable<std::remove_cvref_t<T>>, std::remove_cvref_t<T>>;

namespace shapes {

class Circle: public Drawable<Circle> {

private:
    float radius;
    uint cx, cy;
    uint32_t color;
 
public: 
    Circle(float radius, uint cx, uint cy, uint32_t color);
    ~Circle() = default;
    
    void impl(PixelBuffer &pixels) const;
};

class Ellipse: public Drawable<Ellipse> {

private:
    uint cx, cy;
    uint a, b;
    uint32_t color;
    
public:
    Ellipse(uint cx, uint cy, uint a, uint b, uint32_t color);
    ~Ellipse() = default;
    
    void impl(PixelBuffer &pixels) const;
};

class Line: public Drawable<Line> {

private:
    uint x1, y1, x2, y2;
    uint32_t color;

public:
    Line(uint x1, uint y1, uint x2, uint y2, uint32_t color);
    ~Line() = default;

    void impl(PixelBuffer &pixels) const;
};

class Rectangle: public Drawable<Rectangle> {

private:
    uint x1, y1, h, b;
    uint32_t color;

public: 
    Rectangle(uint x1, uint y1, uint h, uint b, uint32_t color);
    ~Rectangle() = default;
    
    void impl(PixelBuffer &pixels) const;
};

class Text: public Drawable<Text> {

private:
    const std::string &word;
    uint x1, y1, font_size;
    uint32_t color;
    const Font &font;

    inline void rectangle(PixelBuffer &pixels, uint x, uint y, uint h, uint b) const {
        Rectangle rec {x, y, h, b, color};
        rec.draw(pixels);
    }

public:
    Text(const std::string &word, uint x1, uint y1, uint font_size, uint32_t color, const Font &font = default_font);
    ~Text() = default;
    
    void impl(PixelBuffer &pixels) const noexcept;
};

class Triangle : public Drawable<Triangle> {

private:
    uint x1, y1, x2, y2, x3, y3;
    uint32_t color;

public:
    Triangle(uint x1, uint y1, uint x2, uint y2, uint x3, uint y3, uint32_t color);
    ~Triangle() = default;

    void impl(PixelBuffer &pixels) const;
};

class SafeCircle: public Drawable<SafeCircle> {

private:
    float radius;
    uint cx, cy;
    uint32_t color;

public: 
    SafeCircle(float radius, uint cx, uint cy, uint32_t color);
    ~SafeCircle() = default;
    
    void impl(PixelBuffer &pixels) const noexcept;
};

class SafeEllipse: public Drawable<SafeEllipse> {

private:
    uint cx, cy;
    uint a, b;
    uint32_t color;

public:
    SafeEllipse(uint cx, uint cy, uint a, uint b, uint32_t color);
    ~SafeEllipse() = default;
    
    void impl(PixelBuffer &pixels) const noexcept;
};

class SafeLine: public Drawable<SafeLine> {

private:
    uint x1, y1, x2, y2;
    uint32_t color;

public:
    SafeLine(uint x1, uint y1, uint x2, uint y2, uint32_t color);
    ~SafeLine() = default;
    
    void impl(PixelBuffer &pixels) const noexcept;
};

class SafeRectangle: public Drawable<Rectangle> {

private:
    uint x1, y1, h, b;
    uint32_t color;

public:
    SafeRectangle(uint x1, uint y1, uint h, uint b, uint32_t color);
    ~SafeRectangle() = default;

    void impl(PixelBuffer &pixels) const noexcept;
};

class SafeText: public Drawable<SafeText> {
    
private:
    const std::string &word;
    uint x1, y1, font_size;
    uint32_t color;
    const Font &font;

    inline void rectangle(PixelBuffer &pixels, uint x, uint y, uint h, uint b) const noexcept {
        SafeRectangle rec {x, y, h, b, color};
        rec.draw(pixels);
    }

public:
    SafeText(const std::string &word, uint x1, uint y1, uint font_size, uint32_t color, const Font &font = default_font);
    ~SafeText() = default;

    void impl(PixelBuffer &pixels) const noexcept;
};

class SafeTriangle : public Drawable<SafeTriangle> {

private:
    uint x1, y1, x2, y2, x3, y3;
    uint32_t color;

public:
    SafeTriangle(uint x1, uint y1, uint x2, uint y2, uint x3, uint y3, uint32_t color);
    ~SafeTriangle() = default;
    
    void impl(PixelBuffer &pixels) const noexcept;
};

} // namespace shapes

} // namespace eikon
