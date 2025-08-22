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

bool read_ppm(std::istream &file, uint32_t **pixels, size_t *height_ptr, size_t *width_ptr) {
    uint8_t b {}, g {}, r {};
    
    get_ppm_dimensions(file, height_ptr, width_ptr);

    for (size_t  y = 0; y < *height_ptr; y++)
        for (size_t x = 0; x < *width_ptr; x++) {
            file >> r >> g >> b;
            pixels[y][x] = get_hex(r, g, b);
        }

    return true;
}

bool save_ppm(std::ostream &file, uint32_t **pixels, size_t height, size_t width, void *args) {
    file << "P6\n" << width << " " << height << "\n255\n";
    uint8_t r {}, g {}, b {};

    for (size_t  y = 0; y < height; y++)
        for (size_t x = 0; x < width; x++) {
            get_rgb(pixels[y][x], &r, &g, &b);
            file << r << g << b;
        }

    return true;
}
