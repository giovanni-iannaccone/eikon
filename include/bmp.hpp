#pragma once

#include <cstdint>
#include <cstring>
#include <fstream>

#include "pixels.hpp"

class BMPData {

public:
    uint16_t planes;
    uint16_t bit_count;

    uint compression;
    uint32_t image_size;
    
    uint32_t x_pixels_per_meter;
    uint32_t y_pixels_per_meter;

    uint32_t clr_used;
    uint32_t clr_important;
};

namespace bmp {

    enum Compression {
        NO_COMPRESSION,
        RLE
    };

    enum Error: int {
        NO_ERROR,
        INVALID_COMPRESSION,
        INVALID_SIGNATURE,
        INVALID_SIZE
    };

    constexpr uint signature_size = 2;

    void extract_signature(std::istream &file, uint8_t signature[]);
    bool is_valid_signature(std::istream &file);

    void get_dimensions(std::istream &file, uint *height, uint *width);

    void read_header(std::istream &file);
    BMPData read_info_header(std::istream &file, uint *height_ptr, uint *width_ptr);

    void read_raw_data(std::istream &file, PixelBuffer &pixels);
    void read_rle_data(std::istream &file, PixelBuffer &pixels);
    
    bmp::Error read(std::istream &file, PixelBuffer &pixels);
    bmp::Error save(std::ostream &file, const PixelBuffer &pixels, void *args = nullptr);

    void write_raw_data(std::ostream &file, const PixelBuffer &pixels);
    void write_rle_data(std::ostream &file, const PixelBuffer &pixels);

    void write_header(std::ostream &file, uint height, uint width);
    void write_info_header(std::ostream &file, uint height, uint width, BMPData *header);
    void write_signature(std::ostream &file);
}
