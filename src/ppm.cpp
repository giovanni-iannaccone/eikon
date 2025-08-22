#include <cstdint>
#include <fstream>
#include <utility>

#include "../include/ppm.hpp"
#include "../include/utils.hpp"

void get_ppm_dimensions(std::istream &file, uint *height, uint *width) {
    std::string format {};
    uint8_t buffer;

    file.seekg(0);
    file >> format >> *width >> *height >> buffer >> buffer >> buffer;
}

bool read_ppm(std::istream &file, uint32_t **pixels, uint *height_ptr, uint *width_ptr) {
    uint8_t b {}, g {}, r {};
    
    get_ppm_dimensions(file, height_ptr, width_ptr);

    for (uint  y = 0; y < *height_ptr; y++)
        for (uint x = 0; x < *width_ptr; x++) {
            file >> r >> g >> b;
            pixels[y][x] = get_hex(r, g, b);
        }

    return true;
}

bool save_ppm(std::ostream &file, uint32_t **pixels, uint height, uint width, void *args) {
    file << "P6\n" << width << " " << height << "\n255\n";
    uint8_t r {}, g {}, b {};

    for (uint  y = 0; y < height; y++)
        for (uint x = 0; x < width; x++) {
            get_rgb(pixels[y][x], &r, &g, &b);
            file << r << g << b;
        }

    return true;
}
