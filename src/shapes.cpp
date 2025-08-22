#include "../include/shapes.hpp"

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

void Rectangle::draw(uint32_t **pixels, uint height, uint width) {
    for (uint y = y1; y < y1 + h; y++)
        for (uint x = x1; x < x1 + b; x++)
            pixels[y][x] = get_alpha_blend_color(pixels[y][x], color);
}

void Text::rectangle(uint32_t **pixels, uint height, uint width, uint x, uint y, uint h, uint b) {
    Rectangle rec {x, y, h, b, color};
    rec.draw(pixels, height, width);
}

void Text::draw(uint32_t **pixels, uint height, uint width) {
    Glyph glyph {};
    int gx {}, gy {};
    
    for (uint i = 0; i < word.length(); i++) {
        gx = x1 + i * font.width * font_size;
        gy = y1;
        glyph = font.glyphs->at(word.at(i));
        
        for (uint dy = 0; dy < font.height; dy++) {
            for (uint dx = 0; dx < font.width; dx++) {
                uint px = gx + dx*font_size;
                uint py = gy + dy*font_size;
                
                if (px < width && py < height)
                    if (glyph[dy][dx])
                        rectangle(pixels, height, width, px, py, font_size, font_size);
            }
        }
    }
}

void Triangle::draw_borders(uint32_t **pixels, uint height, uint width) {    
    Line l = Line(x1, y1, x2, y2, color);
    l.draw(pixels, height, width);
    
    l = Line(x1, y1, x3, y3, color);
    l.draw(pixels, height, width);
    
    l = Line(x2, y2, x3, y3, color);
    l.draw(pixels, height, width);
}

void Triangle::draw(uint32_t **pixels, uint width, uint height) {
    bool in_triangle {false};
    const uint start_x = std::min(x1, std::min(x2, x3));
    
    sort_points(&x1, &y1, &x2, &y2, &x3, &y3);
    draw_borders(pixels, height, width);

    for (uint y = y1 + 1; y < height; y++) {
        for (uint x = start_x; x < width; x++) {
            if (in_triangle)
                pixels[y][x] = get_alpha_blend_color(pixels[y][x], color);
            
            if (pixels[y][x + 1] == color) {
                in_triangle = !in_triangle;
                
                if (!in_triangle)
                    break;
                else
                    x++;
            }
        }
    }
}
