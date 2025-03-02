#include "../include/raccoon.hpp"

auto fill(Canvas &canvas, uint32_t color) -> void {
    for (size_t i = 0; i < canvas.height * canvas.width; i++)
        canvas.pixels[i] = color;
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

auto effects::rotate_ppm(Canvas &canvas) -> void {

}

auto shapes::rectangle(Canvas &canvas, size_t x1, size_t y1, size_t h, size_t b, uint32_t color) -> void {
    for (size_t y = y1; y < y1 + h; y++)
        for (size_t x = x1; x < x1 + b; x++)
            canvas.pixels[y * canvas.width + x] = color;
}