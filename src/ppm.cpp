#include "../include/ppm.hpp"
#include "../include/utils.hpp"

const uint ppm::signature_size = 2;

void ppm::extract_signature(std::istream &file, char *signature) {
    file.seekg(0);
    for (uint i = 0; i < ppm::signature_size; i++)
        file >> signature[i];
}

void ppm::get_dimensions(std::istream &file, uint *height, uint *width) {
    file.seekg(3);
    uint8_t buffer;
    file >> *width >> *height >> buffer >> buffer >> buffer;
}

bool ppm::is_valid_signature(std::istream &file) {
    char signature[ppm::signature_size];
    ppm::extract_signature(file, signature);

    return memcmp(signature, "P6", ppm::signature_size) == 0;
}

bool ppm::read(std::istream &file, uint32_t **pixels, uint *height_ptr, uint *width_ptr) {
    if (!ppm::is_valid_signature(file))
        return false;
    
    uint8_t r {}, g {}, b {};
    ppm::get_dimensions(file, height_ptr, width_ptr);

    for (uint  y = 0; y < *height_ptr; y++)
        for (uint x = 0; x < *width_ptr; x++) {
            file >> r >> g >> b;
            pixels[y][x] = get_hex(r, g, b);
        }

    return true;
}

bool ppm::save(std::ostream &file, uint32_t **pixels, uint height, uint width, void *args) {
    ppm::write_signature(file);
    ppm::write_header(file, height, width);

    uint8_t r {}, g {}, b {};

    for (uint  y = 0; y < height; y++)
        for (uint x = 0; x < width; x++) {
            get_rgb(pixels[y][x], r, g, b);
            file << r << g << b;
        }

    return true;
}

void ppm::write_header(std::ostream &file, uint height, uint width) {
    file << width << " " << height << "\n255\n";
}

void ppm::write_signature(std::ostream &file) {
    file << "P6\n";
}
