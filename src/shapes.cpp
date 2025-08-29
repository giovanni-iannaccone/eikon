#include "../include/shapes.hpp"

Circle::Circle(float radius, uint xc, uint yc, uint32_t color)
    : radius(radius), xc(xc), yc(yc), 
    color(color) {}

void Circle::draw(uint32_t **pixels, uint height, uint width) {
    float radius_squared {radius * radius};
    uint dist {};
    
    for (uint y = yc - radius; y <= yc + radius; y++) {
        
        dist = xc - radius;
        while(radius_squared < (y - yc) * (y - yc) + (dist - xc) * (dist - xc))
            dist++;
            
        for (uint x = dist; x <= 2*xc - dist; x++) 
            pixels[y][x] = get_alpha_blend_color(pixels[y][x], color);
    }
}

Line::Line(uint x1, uint y1, uint x2, uint y2, uint32_t color)
    : x1(x1), y1(y1), x2(x2), y2(y2), color(color) {}

void Line::draw(uint32_t **pixels, uint height, uint width) {
    int dx = abs((int)x2 - (int)x1);
    int dy = abs((int)y2 - (int)y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    
    while (x1 != x2 || y1 != y2) {
        pixels[y1][x1] = color;
        
        int e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

Rectangle::Rectangle(uint x1, uint y1, uint h, uint b, uint32_t color)
    : x1(x1), y1(y1), h(h), b(b),
    color(color) {}

void Rectangle::draw(uint32_t **pixels, uint height, uint width) {
    for (uint y = y1; y < y1 + h; y++)
        for (uint x = x1; x < x1 + b; x++)
            pixels[y][x] = get_alpha_blend_color(pixels[y][x], color);
}

Text::Text(const std::string &word, uint x1, uint y1, uint font_size, uint32_t color, const Font &font) 
    : word(word), x1(x1), y1(y1), font_size(font_size), 
    color(color), font(font) {}

void Text::draw(uint32_t **pixels, uint height, uint width) {
    int gx {}, gy {};
    
    for (uint i = 0; i < word.length(); i++) {
        gx = x1 + i * font.width * font_size;
        gy = y1;
        const Glyph* const glyph = &font.glyphs->at(word.at(i));
        
        for (uint dy = 0; dy < font.height; dy++) {
            for (uint dx = 0; dx < font.width; dx++) {
                uint px = gx + dx*font_size;
                uint py = gy + dy*font_size;
                
                if (px < width && py < height)
                    if ((*glyph)[dy][dx])
                        rectangle(pixels, height, width, px, py, font_size, font_size);
            }
        }
    }
}

void Text::rectangle(uint32_t **pixels, uint height, uint width, uint x, uint y, uint h, uint b) {
    Rectangle rec {x, y, h, b, color};
    rec.draw(pixels, height, width);
}

Triangle::Triangle(uint x1, uint y1, uint x2, uint y2, uint x3, uint y3, uint32_t color)
    : x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3), color(color) {}

int Triangle::cross_product(int px, int py, int qx, int qy, int rx, int ry) const {
    return (qx - px) * (ry - py) - (qy - py) * (rx - px);
}

bool Triangle::is_inside(int px, int py) const {
    int d1 = cross_product(x1, y1, x2, y2, px, py);
    int d2 = cross_product(x2, y2, x3, y3, px, py);
    int d3 = cross_product(x3, y3, x1, y1, px, py);

    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

void Triangle::draw(uint32_t **pixels, uint width, uint height) {
    int minX = tmin(x1, x2, x3);
    int maxX = tmax(x1, x2, x3);
    int minY = tmin(y1, y2, y3);
    int maxY = tmax(y1, y2, y3);

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                if (is_inside(x, y)) {
                    pixels[y][x] = get_alpha_blend_color(pixels[y][x], color);
                }
            }
        }
    }
}