#include "../include/bmp.hpp"

void bmp::extract_signature(std::istream &file, char *signature) {
    file.seekg(0);
    for (int i = 0; i < bmp::signature_size; i++)
        file.read(reinterpret_cast<char*>(signature + i), sizeof(unsigned char));
}

bool bmp::is_valid_signature(std::istream &file) {
    const char expected_signature[bmp::signature_size] = {0x42, 0x4D};

    char signature[bmp::signature_size];
    bmp::extract_signature(file, signature);

    return memcmp(expected_signature, signature, bmp::signature_size) == 0;
}

bool bmp::read(std::istream &file, uint32_t **pixels, uint *height_ptr, uint *width_ptr) {
    if (!bmp::is_valid_signature(file))
        return false;

    bmp::read_header(height_ptr, width_ptr);

    uint8_t r {}, g {}, b {};

    for (uint y = 0; y < *height_ptr; y++)
        for (uint x = 0; x < *width_ptr; x++) {
            b = get_byte(file);
            g = get_byte(file);
            r = get_byte(file);

            pixels[y][x] = get_hex(r, g, b);
        }

    return true;
}

BMPData bmp::read_header(uint *height_ptr, uint *width_ptr) {
    
}

bool bmp::save(std::ostream &file, uint32_t **pixels, uint height, uint width, void *args) {
    bmp::write_signature(file);
    bmp::write_header(file, height, width, args);

    uint8_t r {}, g {}, b {};

    for (uint y = 0; y < height; y++)
        for (uint x = 0; x < width; x++) {
            get_rgb(pixels[y][x], &r, &g, &b);

            file.write(reinterpret_cast<const char *>(&b), sizeof(b));
            file.write(reinterpret_cast<const char *>(&g), sizeof(g));
            file.write(reinterpret_cast<const char *>(&r), sizeof(r));
        }
}

void bmp::write_header(std::ostream &file, uint height, uint width, void *args) {
    if (args != nullptr) {
        BMPData *header = (BMPData *)args;
        // save header data to file
    } else {
        // save default data to file
    }
}

void bmp::write_signature(std::ostream &file) {
    const char signature[bmp::signature_size] = {0x42, 0x4D};

    for (uint i = 0; i < bmp::signature_size; i++)
        file.write(signature + i, sizeof(signature + i));
}