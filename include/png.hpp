#pragma once

#if defined(__WIN32__) || defined(__WIN64__)
    #include <winsock2.h> 
#else
    #include <arpa/inet.h>
#endif

#include <cstdint>
#include <cstring>
#include <iostream>
#include <utility>

#include <zlib.h>

const int dimensions_pos    = 16;
const int signature_size    = 8;

typedef struct {
    size_t height;
    size_t width;

    size_t ihdr_pos;
    size_t ihdr_size;

    size_t gama_pos;
    size_t gama_size;

    size_t chrm_pos;
    size_t chrm_size;

    size_t phys_pos;
    size_t phys_size;

    size_t time_pos;
    size_t time_size;
    
    size_t bkgd_pos;
    size_t bkgd_size;

    size_t idat_pos;
    size_t idat_size;
} PNG_DATA;

void decode_png(std::istream &file, uint32_t pixels[], size_t height, size_t width) {
    z_streamp idat;
    inflate(idat, Z_NO_FLUSH);
}

void encode_png() {
    
}

void extract_signature(std::istream &file, int signature[]) {
    file.seekg(0);
    for (int i = 0; i < signature_size; i++)
        file.read(reinterpret_cast<char*>(signature + i), sizeof(unsigned char));
}

void get_png_dimensions(std::istream &file, size_t *height, size_t *width) {
    file.seekg(dimensions_pos);

    file.read((char *)width, 4);
    file.read((char *)height, 4);

    *width = ntohl(*width);
    *height = ntohl(*height);
}

bool is_valid_png(std::istream &file) {
    const int expected_signature[signature_size] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

    int signature[signature_size];
    extract_signature(file, signature);

    return memcmp(expected_signature, signature, signature_size) == 0;
}

bool parse_png(PNG_DATA *png, std::istream &file) {
    if (!is_valid_png(file))
        return false;

    png->ihdr_pos = 8;
    png->ihdr_size = 13;
    get_png_dimensions(file, &png->height, &png->width);

    int pos = file.tellg();
    char buffer[4];
    while (file.read(buffer, sizeof(buffer))) {

        pos++;
    }
}

bool read_png(std::istream &file, uint32_t pixels[], size_t *height_ptr, size_t *width_ptr) {
    PNG_DATA png;
    if (!parse_png(&png, file))
        return false;

    decode_png(file, pixels, png.height, png.width);
    return true;
}

void save_png(std::ostream &file, uint32_t pixels[], size_t height, size_t width) {

}