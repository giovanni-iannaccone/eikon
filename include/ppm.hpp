#pragma once

#include <cstdint>
#include <cstring>
#include <fstream>

#include "pixels.hpp"

namespace ppm {
    enum Error: int {
        NO_ERROR,
        INVALID_SIGNATURE,
        INVALID_SIZE ,
    };

    extern const uint signature_size;

    void extract_signature(std::istream &file, uint8_t signature[]);
    bool is_valid_signature(std::istream &file);

    void get_dimensions(std::istream &file, uint *height, uint *width);

    void read_header(std::istream &file, uint *height_ptr, uint *width_ptr);

    ppm::Error read(std::istream &file, PixelBuffer &pixels);
    ppm::Error save(std::ostream &file, const PixelBuffer &pixels, void *args = nullptr);

    void write_header(std::ostream &file, uint height, uint width);
    void write_signature(std::ostream &file);
}
