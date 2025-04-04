#include "../include/raccoon.hpp"

auto get_ppm_dimensions(std::ifstream &file, size_t *height, size_t *width) -> void {
    std::string format {};
    uint8_t buffer;

    file.seekg(0);
    file >> format >> *width >> *height >> buffer >> buffer >> buffer;
}

auto mix_colors(uint32_t first_color, uint32_t second_color, float second_opacity) -> uint32_t {
    uint8_t r1 = (first_color >> (8 * 0)) & 0xFF;
    uint8_t g1 = (first_color >> (8 * 1)) & 0xFF;
    uint8_t b1 = (first_color >> (8 * 2)) & 0xFF;

    uint8_t r2 = (second_color >> (8 * 0)) & 0xFF;
    uint8_t g2 = (second_color >> (8 * 1)) & 0xFF;
    uint8_t b2 = (second_color >> (8 * 2)) & 0xFF;

    uint8_t nr = (r1 + r2 * second_opacity) / (1 + second_opacity);
    uint8_t ng = (g1 + g2 * second_opacity) / (1 + second_opacity);
    uint8_t nb = (b1 + b2 * second_opacity) / (1 + second_opacity);

    return obtain_hex(nr, ng, nb);
}

inline auto obtain_hex(uint8_t r, uint8_t g, uint8_t b) -> uint32_t {
    return ((0xFF00 | b) << 8 | g) << 8 | r;
}

static inline auto sort_points(size_t *x1, size_t *y1, size_t *x2, size_t *y2, size_t *x3, size_t *y3) -> void {
    if (*y1 > *y2) { std::swap(*y1, *y2); std::swap(*x1, *x2); }
    if (*y1 > *y3) { std::swap(*y1, *y3); std::swap(*x1, *x3); }
    if (*y2 > *y3) { std::swap(*y2, *y3); std::swap(*x2, *x3); }
}

auto RaccoonCanvas::reverse_matrix() -> void {
    for (size_t y = 0; y < this->height; y++)
        for(size_t x = 0; x < this->height/2; x++)
            std::swap(this->pixels[y*this->height + x], 
                this->pixels[y*this->height + this->height - x - 1]
            );
}

auto RaccoonCanvas::transpose_matrix() -> void {
    for (size_t y = 0; y < this->height; y++) 
        for (size_t x = y + 1; x < this->width; x++)
            std::swap(this->pixels[y*this->width + x], 
                this->pixels[x*this->width + y]
            );
}

auto RaccoonCanvas::fill(uint32_t color) -> void {
    for (size_t i = 0; i < this->height * this->width; i++)
        this->pixels[i] = color;
}

auto RaccoonCanvas::read_ppm(std::ifstream &file) -> void {
    uint8_t b {}, g {}, r {};
    
    get_ppm_dimensions(file, &this->height, &this->width);

    for (size_t i = 0; i < this->height * this->width; i++) {
        file >> r >> g >> b;
        this->pixels[i] = obtain_hex(r, g, b);
    }
}

auto RaccoonCanvas::save_to_ppm(std::ofstream &file) -> void {
    file << "P6\n" << this->width << " " << this->height << "\n255\n";

    for (size_t  i = 0; i < this->height * this->width; i++) {
        uint32_t pixel = this->pixels[i];
        
        uint8_t r = (pixel >> (8 * 0)) & 0xFF;
        uint8_t g = (pixel >> (8 * 1)) & 0xFF;
        uint8_t b = (pixel >> (8 * 2)) & 0xFF;

        file << r << g << b;
    }
}

auto RaccoonCanvas::ascii(size_t scale) -> void {
    for (size_t y = 0; y < this->height; y += scale) {
        for (size_t x = 0; x < this->width; x += scale) {
            if (this->pixels[y*this->width + x] == 0xFF000000)
                std::cout << " ";
            else 
                std::cout << static_cast<char>(this->pixels[y*this->width + y]);
        }
        
        std::cout << "\n";
    }
}

auto RaccoonCanvas::flip() -> void {
    for (size_t y = 0; y < this->height; y++)
        for (size_t x = 0; x < this->width / 2; x++)
            std::swap(
                this->pixels[y*this->width + x],
                this->pixels[(y + 1)*this->width - x - 1]
            );
}

auto RaccoonCanvas::rotate() -> int {
    if (this->width != this->height)
        return 1;

    this->transpose_matrix();
    this->reverse_matrix();

    return 0;
}

auto RaccoonCanvas::stretch(unsigned int size) -> void {
    uint32_t *new_pixels = (uint32_t *)malloc(this->width * this->height * size * sizeof(uint32_t));
    
    for (size_t i = 0; i < this->width * this->height; i++)
        for (unsigned int j = 0; j < size; j++)
            new_pixels[i*size + j] = this->pixels[i];

    this->pixels = new_pixels;
    this->width *= size;
}

auto RaccoonCanvas::circle(size_t xc, size_t yc, float radius, uint32_t color, float opacity) -> void {
    float radius_squared {radius * radius};
    size_t dist {};
    
    for (size_t y = yc - radius; y <= yc + radius; y++) {
        
        dist = xc - radius;
        while(radius_squared < (y - yc) * (y - yc) + (dist - xc) * (dist - xc))
            dist++;
        
        for (size_t x = dist; x <= 2*xc - dist; x++) 
            this->pixels[y * this->width + x] = opacity != 1
                ? mix_colors(this->pixels[y * this->width + x], color, opacity)
                : color;
    }
}

auto RaccoonCanvas::line(size_t x1, size_t y1, size_t x2, size_t y2, uint32_t color) -> void {
    int dx = abs((int)x2 - (int)x1);
    int dy = abs((int)y2 - (int)y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (x1 != x2 || y1 != y2) {
        this->pixels[y1 * this->width + x1] = color;

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

auto RaccoonCanvas::rectangle(size_t x1, size_t y1, size_t h, size_t b, uint32_t color, float opacity) -> void {

    for (size_t y = y1; y < y1 + h; y++)
        for (size_t x = x1; x < x1 + b; x++) 
            this->pixels[y * this->width + x] = opacity != 1
                ? mix_colors(this->pixels[y * this->width + x], color, opacity)
                : color;
}

auto RaccoonCanvas::text(const std::string word, size_t x1, size_t y1, size_t font_size, uint32_t color, Font &font) -> void {
    std::vector<std::vector<bool>> glyph {};
    int gx {}, gy {};

    for (size_t i = 0; i < word.length(); i++) {
        gx = x1 + i * font.width * font_size;
        gy = y1;
        glyph = font.glyphs->at(word[i]);

        for (int dy = 0; dy < font.height; dy++) {
            for (int dx = 0; dx < font.width; dx++) {
                int px = gx + dx*font_size;
                int py = gy + dy*font_size;
                
                if (0 <= px && px < this->width && 0 <= py && py < this->height)
                    if (glyph[dy][dx])
                        this->rectangle(px, py, font_size, font_size, color);
            }
        }
    }
}

auto RaccoonCanvas::triangle(size_t x1, size_t y1, size_t x2, size_t y2, size_t x3, size_t y3, uint32_t color, float opacity) -> void {    
    bool in_triangle {false};
    const size_t start_x = std::min(x1, std::min(x2, x3));

    sort_points(&x1, &y1, &x2, &y2, &x3, &y3);

    this->line(x1, y1, x2, y2, color);
    this->line(x1, y1, x3, y3, color);
    this->line(x2, y2, x3, y3, color);

    for (size_t y = y1 + 1; y < this->height; y++) {
        for (size_t x = start_x; x < this->width; x++) {
            if (in_triangle)
                this->pixels[y*this->width + x] = opacity != 1
                    ? mix_colors(this->pixels[y * this->width + x], color, opacity)
                    : color;

            if (this->pixels[y*this->width + x + 1] == color) {
                in_triangle = !in_triangle;
                
                if (!in_triangle)
                    break;
                else
                    x++;
            }
        }
    }
}
