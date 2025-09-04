#pragma once

#include <cstdint>
#include <cstring>
#include <fstream>
#include <utility>

namespace ppm {
    extern const uint signature_size;

    void extract_signature(std::istream &file, char *signature);
    bool is_valid_signature(std::istream &file);

    void get_dimensions(std::istream &file, uint *height, uint *width);

    void read_header(std::istream &file, uint *height_ptr, uint *width_ptr);

    bool read(std::istream &file, uint32_t **pixels, uint *height_ptr, uint *width_ptr);
    bool save(std::ostream &file, uint32_t **pixels, uint height, uint width, void *args = nullptr);

    void write_header(std::ostream &file, uint height, uint width);
    void write_signature(std::ostream &file);
}