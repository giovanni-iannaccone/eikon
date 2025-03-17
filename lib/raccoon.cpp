#include "../include/matrix_utils.hpp"
#include "../include/raccoon.hpp"

auto fill(Canvas &canvas, uint32_t color) -> void {
    for (size_t i = 0; i < canvas.height * canvas.width; i++)
        canvas.pixels[i] = color;
}

auto get_ppm_dimensions(std::ifstream &file, Canvas &canvas) -> void {
    std::string format {};
    uint8_t buffer;

    file.seekg(0);
    file >> format >> canvas.width >> canvas.height >> buffer >> buffer >> buffer;
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

auto new_canvas(size_t height, size_t width) -> Canvas {
    uint32_t *pixels = (uint32_t*)malloc(height * width * sizeof(uint32_t));
    return Canvas{pixels, height, width};
}

inline auto obtain_hex(uint8_t r, uint8_t g, uint8_t b) -> uint32_t {
    return ((0xFF00 | b) << 8 | g) << 8 | r;
}

auto read_ppm(std::ifstream &file, Canvas &canvas) -> void {
    uint8_t b {}, g {}, r {};
    std::string line {};

    get_ppm_dimensions(file, canvas);

    for (size_t i = 0; i < canvas.height * canvas.width; i++) {
        file >> r >> g >> b;
        canvas.pixels[i] = obtain_hex(r, g, b);
    }
}

auto save_to_ppm(std::ofstream &file, Canvas &canvas) -> void {
    file << "P6\n" << canvas.width << " " << canvas.height << "\n255\n";

    for (size_t  i = 0; i < canvas.height * canvas.width; i++) {
        uint32_t pixel = canvas.pixels[i];
        
        uint8_t r = (pixel >> (8 * 0)) & 0xFF;
        uint8_t g = (pixel >> (8 * 1)) & 0xFF;
        uint8_t b = (pixel >> (8 * 2)) & 0xFF;

        file << r << g << b;
    }
}

static inline auto sort_points(size_t *x1, size_t *y1, size_t *x2, size_t *y2, size_t *x3, size_t *y3) -> void {
    if (*y1 > *y2) { std::swap(*y1, *y2); std::swap(*x1, *x2); }
    if (*y1 > *y3) { std::swap(*y1, *y3); std::swap(*x1, *x3); }
    if (*y2 > *y3) { std::swap(*y2, *y3); std::swap(*x2, *x3); }
}

auto effects::ascii(Canvas &canvas, size_t scale) -> void {
    for (size_t y = 0; y < canvas.height; y += scale) {
        for (size_t x = 0; x < canvas.width; x += scale) {
            if (canvas.pixels[y*canvas.width + x] == 0xFF000000)
                std::cout << " ";
            else 
                std::cout << static_cast<char>(canvas.pixels[y*canvas.width + y]);
        }
        
        std::cout << "\n";
    }
}

auto effects::flip_ppm(Canvas &canvas) -> void {
    for (size_t y = 0; y < canvas.height; y++)
        for (size_t x = 0; x < canvas.width / 2; x++)
            std::swap(
                canvas.pixels[y*canvas.width + x],
                canvas.pixels[(y + 1)*canvas.width - x - 1]
            );
}

auto effects::rotate_ppm(Canvas &canvas) -> int {
    if (canvas.width != canvas.height)
        return 1;

    transpose_matrix(canvas);
    reverse_matrix(canvas);

    return 0;
}

auto effects::stretch_ppm(Canvas &canvas, unsigned int size) -> void {
    uint32_t *new_pixels = (uint32_t *)malloc(canvas.width * canvas.height * size * sizeof(uint32_t));
    
    for (size_t i = 0; i < canvas.width * canvas.height; i++)
        for (unsigned int j = 0; j < size; j++)
            new_pixels[i*size + j] = canvas.pixels[i];

    canvas.pixels = new_pixels;
    canvas.width *= size;
}

auto shapes::circle(Canvas &canvas, size_t xc, size_t yc, float radius, uint32_t color, float opacity) -> void {
    float radius_squared {radius * radius};
    size_t dist {};
    
    for (size_t y = yc - radius; y <= yc + radius; y++) {
        
        dist = xc - radius;
        while(radius_squared < (y - yc) * (y - yc) + (dist - xc) * (dist - xc))
            dist++;
        
        for (size_t x = dist; x <= 2*xc - dist; x++) 
            canvas.pixels[y * canvas.width + x] = opacity != 1
                ? mix_colors(canvas.pixels[y * canvas.width + x], color, opacity)
                : color;
    }
}

auto shapes::line(Canvas &canvas, size_t x1, size_t y1, size_t x2, size_t y2, uint32_t color) -> void {
    int dx = abs((int)x2 - (int)x1);
    int dy = abs((int)y2 - (int)y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (x1 != x2 || y1 != y2) {
        canvas.pixels[y1 * canvas.width + x1] = color;

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

auto shapes::rectangle(Canvas &canvas, size_t x1, size_t y1, size_t h, size_t b, uint32_t color, float opacity) -> void {

    for (size_t y = y1; y < y1 + h; y++)
        for (size_t x = x1; x < x1 + b; x++) 
            canvas.pixels[y * canvas.width + x] = opacity != 1
                ? mix_colors(canvas.pixels[y * canvas.width + x], color, opacity)
                : color;
}

auto shapes::text(Canvas &canvas, const std::string word, size_t x1, size_t y1, size_t font_size, uint32_t color, Font &font) -> void {
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
                
                if (0 <= px && px < canvas.width && 0 <= py && py < canvas.height)
                    if (glyph[dy][dx])
                        shapes::rectangle(canvas, px, py, font_size, font_size, color);
            }
        }
    }
}

auto shapes::triangle(Canvas &canvas, size_t x1, size_t y1, size_t x2, size_t y2, size_t x3, size_t y3, uint32_t color, float opacity) -> void {    
    bool in_triangle {false};
    const size_t start_x = std::min(x1, std::min(x2, x3));

    sort_points(&x1, &y1, &x2, &y2, &x3, &y3);

    shapes::line(canvas, x1, y1, x2, y2, color);
    shapes::line(canvas, x1, y1, x3, y3, color);
    shapes::line(canvas, x2, y2, x3, y3, color);

    for (size_t y = y1 + 1; y < canvas.height; y++) {
        for (size_t x = start_x; x < canvas.width; x++) {
            if (in_triangle)
                canvas.pixels[y*canvas.width + x] = opacity != 1
                    ? mix_colors(canvas.pixels[y * canvas.width + x], color, opacity)
                    : color;

            if (canvas.pixels[y*canvas.width + x + 1] == color) {
                in_triangle = !in_triangle;
                
                if (!in_triangle)
                    break;
                else
                    x++;
            }
        }
    }
}