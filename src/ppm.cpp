#include "../include/ppm.hpp"
#include "../include/utils.hpp"

const uint ppm::signature_size = 2;

void ppm::extract_signature(std::istream &file, uint8_t signature[]) {
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
    uint8_t signature[ppm::signature_size];
    ppm::extract_signature(file, signature);

    return memcmp(signature, "P6", ppm::signature_size) == 0;
}

ppm::Error ppm::read(std::istream &file, uint32_t **pixels, uint *height_ptr, uint *width_ptr) {
    if (!ppm::is_valid_signature(file))
        return ppm::Error::INVALID_SIGNATURE;
    
    uint8_t r {}, g {}, b {};
    ppm::get_dimensions(file, height_ptr, width_ptr);

    for (uint  y = 0; y < *height_ptr; y++)
        for (uint x = 0; x < *width_ptr; x++) {
            file >> r >> g >> b;
            pixels[y][x] = get_hex(r, g, b);
        }

    return ppm::Error::NO_ERROR;
}

ppm::Error ppm::save(std::ostream &file, uint32_t **pixels, uint height, uint width, void *args) {
    ppm::write_signature(file);

    if (height == 0 || width == 0)
        return ppm::Error::INVALID_SIZE;

    ppm::write_header(file, height, width);

    uint8_t r {}, g {}, b {};

    for (uint  y = 0; y < height; y++)
        for (uint x = 0; x < width; x++) {
            get_rgb(pixels[y][x], r, g, b);
            file << r << g << b;
        }

    return ppm::Error::NO_ERROR;
}

void ppm::write_header(std::ostream &file, uint height, uint width) {
    file << width << " " << height << "\n255\n";
}

void ppm::write_signature(std::ostream &file) {
    file << "P6\n";
}
