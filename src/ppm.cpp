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

ppm::Error ppm::read(std::istream &file, PixelBuffer &pixels) {
    if (!ppm::is_valid_signature(file))
        return ppm::Error::INVALID_SIGNATURE;
    
    uint8_t r {}, g {}, b {};
    ppm::get_dimensions(file, &pixels.height, &pixels.width);

    for (uint  y = 0; y < pixels.height; y++)
        for (uint x = 0; x < pixels.width; x++) {
            file >> r >> g >> b;
            pixels[y][x] = get_hex(r, g, b);
        }

    return ppm::Error::NO_ERROR;
}

ppm::Error ppm::save(std::ostream &file, const PixelBuffer &pixels, void *args) {
    ppm::write_signature(file);

    if (pixels.height == 0 || pixels.width == 0)
        return ppm::Error::INVALID_SIZE;

    ppm::write_header(file, pixels.height, pixels.width);

    uint8_t r {}, g {}, b {};

    for (uint  y = 0; y < pixels.height; y++)
        for (uint x = 0; x < pixels.width; x++) {
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
