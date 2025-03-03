#include "../include/matrix_utils.hpp"
#include "../include/raccoon.hpp"

auto fill(Canvas &canvas, uint32_t color) -> void {
    for (size_t i = 0; i < canvas.height * canvas.width; i++)
        canvas.pixels[i] = color;
}

auto get_ppm_dimensions(std::ifstream &file, Canvas &canvas) -> void {
    std::string line {};

    std::getline(file, line);
    file >> canvas.width >> canvas.height;
}

auto read_from_ppm(std::ifstream &file, Canvas &canvas) -> void {
    uint8_t r {}, g {}, b {};
    uint32_t pixel {};
    std::string line {};

    get_ppm_dimensions(file, canvas);

    for (size_t i = 0; i < canvas.height * canvas.width; i++) {
        file >> r >> g >> b;
        
        pixel = 0xFF;
        pixel = (pixel << 8) | b;
        pixel = (pixel << 8) | g;
        pixel = (pixel << 8) | r;

        canvas.pixels[i] = pixel;
    }
}

auto save_to_ppm(std::ofstream &file, Canvas &canvas) -> void {
    file << "P6\n" << canvas.width << " " << canvas.height << "\n255\n";

    for (size_t i = 0; i < canvas.height * canvas.width; i++) {
        uint32_t pixel = canvas.pixels[i];
        
        uint8_t r = (pixel >> (8 * 0)) & 0xFF;
        uint8_t g = (pixel >> (8 * 1)) & 0xFF;
        uint8_t b = (pixel >> (8 * 2)) & 0xFF;

        file.put(r);
        file.put(g);
        file.put(b);
    }
}

auto effects::flip_ppm(Canvas &canvas) -> void {
    for (size_t y = 0; y < canvas.height; y++)
        for (size_t x = 0; x < canvas.width / 2; x++)
            std::swap(canvas.pixels[y*canvas.width + x], 
                canvas.pixels[y*canvas.width + canvas.width - x]
            );
}

auto effects::rotate_ppm(Canvas &canvas) -> int {
    if (canvas.width != canvas.width)
        return 1;

    transpose_matrix(canvas);
    reverse_matrix(canvas);

    return 0;
}

auto shapes::circle(Canvas &canvas, size_t xc, size_t yc, float radius, uint32_t color) -> void {
    float dist_squared {};
    float radius_squared = radius * radius;

    for (size_t y = 0; y < canvas.height; y++) {
        for (size_t x = 0; x < canvas.width; x++) {
            dist_squared = (y - yc) * (y - yc) + (x - xc) * (x - xc);
            if (dist_squared <= radius_squared) {
                canvas.pixels[y * canvas.width + x] = color;
            }
        }
    }
}

auto shapes::rectangle(Canvas &canvas, size_t x1, size_t y1, size_t h, size_t b, uint32_t color) -> void {
    for (size_t y = y1; y < y1 + h; y++)
        for (size_t x = x1; x < x1 + b; x++)
            canvas.pixels[y * canvas.width + x] = color;
}

auto shapes::triangle(Canvas &canvas, size_t x1, size_t y1, size_t x2, size_t y2, size_t x3, size_t y3, uint32_t color) -> void {
    
}