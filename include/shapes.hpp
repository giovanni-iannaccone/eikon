#pragma once

#include <bits/stdc++.h>

#include "font.hpp"
#include "utils.hpp"

class Drawable {

public:
    virtual void draw(uint32_t pixels[], size_t height, size_t width) = 0;
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

    void draw(uint32_t pixels[], size_t height, size_t width) override {
        float radius_squared {radius * radius};
        size_t dist {};
        
        for (size_t y = yc - radius; y <= yc + radius; y++) {
            
            dist = xc - radius;
            while(radius_squared < (y - yc) * (y - yc) + (dist - xc) * (dist - xc))
                dist++;
            
            for (size_t x = dist; x <= 2*xc - dist; x++) 
                pixels[y * width + x] = opaqueness != 1
                    ? mix_colors(pixels[y * width + x], color, opaqueness)
                    : color;
        }
    }
};

class Line: public Drawable {

private:
    size_t x1, y1, x2, y2;
    uint32_t color;

public:

    Line(size_t x1, size_t y1, size_t x2, size_t y2, uint32_t color)
        : x1(x1), y1(y1), x2(x2), y2(y2), color(color) {}

    void draw(uint32_t pixels[], size_t height, size_t width) override {
        int dx = abs((int)x2 - (int)x1);
        int dy = abs((int)y2 - (int)y1);
        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;
        int err = dx - dy;

        while (x1 != x2 || y1 != y2) {
            pixels[y1 * width + x1] = color;

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

    void draw(uint32_t pixels[], size_t height, size_t width) override {
        for (size_t y = y1; y < y1 + h; y++)
            for (size_t x = x1; x < x1 + b; x++)
                pixels[y * width + x] = opaqueness != 1
                    ? mix_colors(pixels[y * width + x], color, opaqueness)
                    : color;
    }
};

class Text: public Drawable {

private:
    std::string *word;
    size_t x1, y1, font_size;
    uint32_t color;
    Font *font;

    void rectangle(uint32_t pixels[], size_t height, size_t width, int x, int y, int h, int b) {
        auto rec = new Rectangle(x, y, h, b, color);
        rec->draw(pixels, height, width);
        delete rec;
    }

public:
    Text(std::string *word, size_t x1, size_t y1, size_t font_size, uint32_t color, Font *font) 
        : word(word), x1(x1), y1(y1), font_size(font_size), 
        color(color), font(font) {}

    void draw(uint32_t pixels[], size_t height, size_t width) override {
        Glyph glyph {};
        int gx {}, gy {};

        for (size_t i = 0; i < word->length(); i++) {
            gx = x1 + i * font->width * font_size;
            gy = y1;
            glyph = font->glyphs->at(word->at(i));

            for (unsigned int dy = 0; dy < font->height; dy++) {
                for (unsigned int dx = 0; dx < font->width; dx++) {
                    unsigned int px = gx + dx*font_size;
                    unsigned int py = gy + dy*font_size;
                    
                    if (px < width && py < height)
                        if (glyph[dy][dx])
                            rectangle(pixels, height, width, px, py, font_size, font_size);
                }
            }
        }
    }
};

class Triangle: public Drawable {

private:
    size_t x1, y1, x2, y2, x3, y3;
    uint32_t color;
    float opaqueness;

    void draw_borders(uint32_t pixels[], size_t height, size_t width) {
        Line *l;
        
        l = new Line(x1, y1, x2, y2, color);
        l->draw(pixels, height, width);
        delete l;

        l = new Line(x1, y1, x3, y3, color);
        l->draw(pixels, height, width);
        delete l;

        l = new Line(x2, y2, x3, y3, color);
        l->draw(pixels, height, width);
        delete l;
    }

public:
    Triangle(size_t x1, size_t y1, size_t x2, size_t y2, size_t x3, size_t y3, uint32_t color, float opaqueness = 1)
        : x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3),
        color(color), opaqueness(opaqueness) {}

    void draw(uint32_t pixels[], size_t width, size_t height) override {
        bool in_triangle {false};
        const size_t start_x = std::min(x1, std::min(x2, x3));

        sort_points(&x1, &y1, &x2, &y2, &x3, &y3);
        draw_borders(pixels, height, width);

        for (size_t y = y1 + 1; y < height; y++) {
            for (size_t x = start_x; x < width; x++) {
                if (in_triangle)
                    pixels[y*width + x] = opaqueness != 1
                        ? mix_colors(pixels[y * width + x], color, opaqueness)
                        : color;

                if (pixels[y*width + x + 1] == color) {
                    in_triangle = !in_triangle;
                    
                    if (!in_triangle)
                        break;
                    else
                        x++;
                }
            }
        }
    }
};