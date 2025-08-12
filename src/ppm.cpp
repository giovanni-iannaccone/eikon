#include <cstdint>
#include <fstream>
#include <utility>

#include "../include/ppm.hpp"
#include "../include/utils.hpp"

void get_ppm_dimensions(std::istream &file, size_t *height, size_t *width) {
    std::string format {};
    uint8_t buffer;

    file.seekg(0);
    file >> format >> *width >> *height >> buffer >> buffer >> buffer;
}

bool read_ppm(std::istream &file, uint32_t pixels[], size_t *height_ptr, size_t *width_ptr) {
    uint8_t b {}, g {}, r {};
    
    get_ppm_dimensions(file, height_ptr, width_ptr);

    for (size_t i = 0; i < (*height_ptr) * (*width_ptr); i++) {
        file >> r >> g >> b;
        pixels[i] = get_hex(r, g, b);
    }

    return true;
}

bool save_ppm(std::ostream &file, uint32_t pixels[], size_t height, size_t width, void *args) {
    file << "P6\n" << width << " " << height << "\n255\n";

    for (size_t  i = 0; i < height * width; i++) {
        uint32_t pixel = pixels[i];
        
        uint8_t r = (pixel >> (8 * 0)) & 0xFF;
        uint8_t g = (pixel >> (8 * 1)) & 0xFF;
        uint8_t b = (pixel >> (8 * 2)) & 0xFF;

        file << r << g << b;
    }

    return true;
}
