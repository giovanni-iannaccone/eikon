#include "../include/shapes.hpp"

void Circle::draw(uint32_t pixels[], size_t height, size_t width) {
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

void Line::draw(uint32_t pixels[], size_t height, size_t width) {
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

void Rectangle::draw(uint32_t pixels[], size_t height, size_t width) {
    for (size_t y = y1; y < y1 + h; y++)
        for (size_t x = x1; x < x1 + b; x++)
            pixels[y * width + x] = opaqueness != 1
                ? mix_colors(pixels[y * width + x], color, opaqueness)
                : color;
}

void Text::rectangle(uint32_t pixels[], size_t height, size_t width, int x, int y, int h, int b) {
    auto rec = new Rectangle(x, y, h, b, color);
    rec->draw(pixels, height, width);
    delete rec;
}

void Text::draw(uint32_t pixels[], size_t height, size_t width) {
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

void Triangle::draw_borders(uint32_t pixels[], size_t height, size_t width) {
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

void Triangle::draw(uint32_t pixels[], size_t width, size_t height) {
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
