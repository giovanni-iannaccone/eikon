#include <cstdint>
#include <fstream>
#include <string>

enum Directions {
    left,
    right
};

void fill(uint32_t *pixels, size_t height, size_t width, uint32_t color) {
    for (size_t i = 0; i < height*width; i++)
        pixels[i] = color;
}

void save_to_ppm(std::ofstream &file, uint32_t *pixels, size_t height, size_t width) {
    file << "P6\n" << width << " " << height << "\n255\n";

    for (size_t i = 0; i < height * width; i++) {
        uint32_t pixel = pixels[i];
        
        uint8_t r = (pixel >> (8 * 0)) & 0xFF;
        uint8_t g = (pixel >> (8 * 1)) & 0xFF;
        uint8_t b = (pixel >> (8 * 2)) & 0xFF;

        file.put(r);
        file.put(g);
        file.put(b);
    }
}

/*int rotate_ppm(std::ifstream original, std::ofstream rotated, size_t height, size_t width, Directions dir) {
    
}*/