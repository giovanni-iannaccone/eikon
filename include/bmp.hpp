#pragma once

#include <cstdint>
#include <cstring>
#include <fstream>

#include "utils.hpp"

class BMPData;

namespace bmp {

    enum Compression {
        NO_COMPRESSION,
        RLE
    };

    extern const uint signature_size;

    void extract_signature(std::istream &file, char *signature);
    bool is_valid_signature(std::istream &file);

    void read_header(std::istream &file);
    BMPData read_info_header(std::istream &file, uint *height_ptr, uint *width_ptr);

    void read_raw_data(std::istream &file, uint32_t **pixels, const uint height, const uint width);
    void read_rle_data(std::istream &file, uint32_t **pixels, const uint height, const uint width);
    
    bool read(std::istream &file, uint32_t **pixels, uint *height_ptr, uint *width_ptr);
    bool save(std::ostream &file, uint32_t **pixels, uint height, uint width, void *args = nullptr);

    void write_raw_data(std::ostream &file, uint32_t **pixels, uint height, uint width);
    void write_rle_data(std::ostream &file, uint32_t **pixels, uint height, uint width);

    void write_header(std::ostream &file, uint height, uint width);
    void write_info_header(std::ostream &file, uint height, uint width, BMPData *header);
    void write_signature(std::ostream &file);
}

class BMPData {

public:
    uint32_t size;
    uint16_t planes;
    uint16_t bit_count;

    bmp::Compression compression;
    uint32_t image_size;
    
    uint32_t x_pixels_per_meter;
    uint32_t y_pixels_per_meter;
};
