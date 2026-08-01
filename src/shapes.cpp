#include <cmath>

#include "../include/shapes.hpp"

namespace eikon::shapes {

static constexpr inline int cross_product(int px, int py, int qx, int qy, int rx, int ry) noexcept
{
    return (qx - px) * (ry - py) - (qy - py) * (rx - px);
}

constexpr bool is_inside(uint x1, uint y1, uint x2, uint y2, uint x3, uint y3, int px, int py) noexcept
{
    int d1 = cross_product(x1, y1, x2, y2, px, py);
    int d2 = cross_product(x2, y2, x3, y3, px, py);
    int d3 = cross_product(x3, y3, x1, y1, px, py);

    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return has_neg != has_pos;
}

Circle::Circle(float radius, uint cx, uint cy, uint32_t color)
    : radius(radius), cx(cx), cy(cy), 
      color(color) {}

void Circle::impl(PixelBuffer &pixels) const
{
    uint x = this->cx;
    uint y  = this->cy - this->radius;
    float radius_squared = this->radius * this->radius;

    while (y < this->cy) {
        for (; x < this->cx; x++) {
            pixels.blend_at(y, x, this->color);
            pixels.blend_at(y, 2*cx - x, this->color);

            pixels.blend_at(2*cy - y, x, this->color);
            pixels.blend_at(2*cy - y, 2*cx - x, this->color);
        }

        pixels.blend_at(y, x, this->color);
        pixels.blend_at(2*cy - y, x, this->color);
        
        y++;
        x = this->cx - std::sqrt(radius_squared - (y - this->cy) * (y - this->cy));
    }
    
    for (; x < this->cx; x++) {
        pixels.blend_at(y, x, this->color);
        pixels.blend_at(y, 2*cx - x, this->color);
    }

    pixels.blend_at(y, x, this->color);
}

Ellipse::Ellipse(uint cx, uint cy, uint a, uint b, uint32_t color)
    : cx(cx), cy(cy), a(a), b(b), color(color) {}

void Ellipse::impl(PixelBuffer &pixels) const
{
    uint a2 = this->a * this->a;
    uint b2 = this->b * this->b;
    uint y = this->cy - this->b;

    uint x1 = 0;
    
    while (y < this->cy) {
        for (uint x = this->cx - x1; x <= this->cx; x++) {
            pixels.blend_at(y, x, this->color);
            pixels.blend_at(y, 2*cx - x, this->color);

            pixels.blend_at(2*cy - y, x, this->color);
            pixels.blend_at(2*cy - y, 2*cx - x, this->color);
        }

        y++;
        x1 = std::sqrt(a2 - (y - this->cy) * (y - this->cy) * a2 / b2);
    }
    
    for (uint x = this->cx - this->a; x <= this->cx + this->a; x++)
        pixels.blend_at(y, x, this->color);
}

Line::Line(uint x1, uint y1, uint x2, uint y2, uint32_t color)
    : x1(x1), y1(y1), x2(x2), y2(y2), color(color) {}

void Line::impl(PixelBuffer &pixels) const
{
    int dx = abs(static_cast<int>(this->x2 - this->x1));
    int dy = abs(static_cast<int>(this->y2 - this->y1));
    int sx = this->x1 < this->x2 ? 1 : -1;
    int sy = this->y1 < this->y2 ? 1 : -1;
    int err = dx - dy;

    int x = this->x1, y = this->y1;
    
    while (x != this->x2 || y != this->y2) {
    pixels.blend_at(y, x, this->color);
        
        int e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

Rectangle::Rectangle(uint x1, uint y1, uint h, uint b, uint32_t color)
    : x1(x1), y1(y1), h(h), b(b), color(color) {}

void Rectangle::impl(PixelBuffer &pixels) const
{
    for (uint y = this->y1; y < this->y1 + this->h; y++)
        for (uint x = this->x1; x < this->x1 + this->b; x++)
            pixels.blend_at(y, x, this->color);
}

Text::Text(const std::string &word, uint x1, uint y1, uint font_size, uint32_t color, const Font &font) 
    : word(word), x1(x1), y1(y1), font_size(font_size), 
      color(color), font(font) {}

void Text::impl(PixelBuffer &pixels) const noexcept
{
    int gx, gy;
    
    for (uint i = 0; i < word.length(); i++) {
        gx = x1 + i * font.width * font_size;
        gy = y1;
        const Glyph &glyph = font.glyphs.at(word.at(i));
        
        for (uint dy = 0; dy < font.height; dy++) {
            for (uint dx = 0; dx < font.width; dx++) {
                uint px = gx + dx*font_size;
                uint py = gy + dy*font_size;
               
                if (glyph[dy][dx])
                    rectangle(pixels, px, py, font_size, font_size);
            }
        }
    }
}

Triangle::Triangle(uint x1, uint y1, uint x2, uint y2, uint x3, uint y3, uint32_t color)
    : x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3), color(color) {}

void Triangle::impl(PixelBuffer &pixels) const
{
    int minX = std::min({this->x1, this->x2, this->x3});
    int maxX = std::max({this->x1, this->x2, this->x3});
    int minY = std::min({this->y1, this->y2, this->y3});
    int maxY = std::max({this->y1, this->y2, this->y3});

    for (int y = minY; y <= maxY; y++)
        for (int x = minX; x <= maxX; x++)
            if (is_inside(this->x1, this->y1, this->x2, this->y2, this->x3, this->y3, x, y))
                    pixels.blend_at(y, x, this->color);
}

SafeCircle::SafeCircle(float radius, uint cx, uint cy, uint32_t color)
    : radius(radius), cx(cx), cy(cy), color(color) {}
    
void SafeCircle::impl(PixelBuffer &pixels) const noexcept
{
    float radius_squared = radius * radius;

    uint ymin = cy - radius < pixels.height() ? cy - radius : 0;
    uint ymax = cy + radius < pixels.height() ? cy + radius : pixels.height() - 1;
   
    for (uint y = ymin; y <= ymax; y++) {
        uint dist = cx - std::sqrt(radius_squared - (y - cy) * (y - cy));
        
        uint xmin = dist >= 0 ? dist : 0;
        uint xmax = 2*cx - xmin < pixels.width() ? 2*cx - xmin : pixels.width();

        for (uint x = xmin; x <= xmax; x++)
            pixels.blend_at(y, x, this->color);
    }
}

SafeEllipse::SafeEllipse(uint cx, uint cy, uint a, uint b, uint32_t color)
    : cx(cx), cy(cy), a(a), b(b), color(color) {}

void SafeEllipse::impl(PixelBuffer &pixels) const noexcept
{
    uint a2 = a * a;
    uint b2 = b * b;
    
    uint ymin = cy >= b ? cy - b : 0;
    uint ymax = cy + b + 1 < pixels.height() ? cy + b + 1: pixels.height() - 1;
    
    uint x1 = 0;

    for (uint y = ymin; y <= ymax; y++) {
        uint xmin = cx - x1 >= 0 ? cx - x1 : 0;
        uint xmax = cx + x1 < pixels.width() ? cx + x1 : pixels.width();

        for (uint x = xmin; x <= xmax; x++)
            pixels.blend_at(y, x, this->color);

        x1 = std::sqrt(a2 - (y - cy) * (y - cy) * a2 / b2);
    }
}

SafeLine::SafeLine(uint x1, uint y1, uint x2, uint y2, uint32_t color)
    : x1(x1), y1(y1), x2(x2), y2(y2), color(color) {}
    
void SafeLine::impl(PixelBuffer &pixels) const noexcept
{
    int dx = abs((int)x2 - (int)x1);
    int dy = abs((int)y2 - (int)y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    int x = x1, y = y1;
    
    while ((x != x2 || y != y2) && (x < pixels.width() && y < pixels.height())) {
        pixels.blend_at(y, x, this->color);
        
        int e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

SafeRectangle::SafeRectangle(uint x1, uint y1, uint h, uint b, uint32_t color)
    : x1(x1), y1(y1), h(h), b(b), color(color) {}

void SafeRectangle::impl(PixelBuffer &pixels) const noexcept
{
    uint ymin = y1 > 0 ? y1 : 0;
    uint xmin = x1 > 0 ? x1 : 0;
    
    uint ymax = y1 + h <= pixels.height() ? y1 + h : pixels.height() - 1;
    uint xmax = x1 + h <= pixels.width() ? x1 + b : pixels.width() - 1;
    
    for (uint y = ymin ; y < ymax; y++)
        for (uint x = xmin; x < xmax; x++)
            pixels.blend_at(y, x, this->color);
}

SafeText::SafeText(const std::string &word, uint x1, uint y1, uint font_size, uint32_t color, const Font &font)
    : word(word), x1(x1), y1(y1), font_size(font_size), color(color), font(font) {}

SafeTriangle::SafeTriangle(uint x1, uint y1, uint x2, uint y2, uint x3, uint y3, uint32_t color)
    : x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3), color(color) {}

void SafeTriangle::impl(PixelBuffer &pixels) const noexcept
{
    int minX = std::min({this->x1, this->x2, this->x3});
    int maxX = std::max({this->x1, this->x2, this->x3});
    int minY = std::min({this->y1, this->y2, this->y3});
    int maxY = std::max({this->y1, this->y2, this->y3});
    
    for (int y = minY; y <= maxY; y++)
        for (int x = minX; x <= maxX; x++)
            if (x >= 0 && x < pixels.width() &&
                y >= 0 && y < pixels.height() &&
                is_inside(this->x1, this->y1, this->x2, this->y2, this->x3, this->y3, x, y))
                    pixels.blend_at(y, x, this->color);
}

} // namespace shapes
