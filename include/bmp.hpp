#pragma once

#include <cstdint>
#include <cstring>
#include <fstream>

#include "utils.hpp"

class BMPData {

public:
    bmp::Compression compression;

};

namespace bmp {

    enum Compression {
        NO_COMPRESSION,
        RLE
    };

    const uint signature_size = 2;

    void extract_signature(std::istream &file, char *signature);
    bool is_valid_signature(std::istream &file);

    void read_header(std::istream &file, uint *height_ptr, uint *width_ptr);
    BMPData read_info_header(std::istream &file);

    void read_raw_data(std::istream &file, uint32_t **pixels, const uint height, const uint width);
    void read_rle_data(std::istream &file, uint32_t **pixels, const uint height, const uint width);
    
    bool read(std::istream &file, uint32_t **pixels, uint *height_ptr, uint *width_ptr);
    bool save(std::ostream &file, uint32_t **pixels, uint height, uint width, void *args = nullptr);

    void write_raw_data(std::ostream &file, uint32_t **pixels, uint height, uint width);
    void write_rle_data(std::ostream &file, uint32_t **pixels, uint height, uint width);

    void write_header(std::ostream &file, uint height, uint width);
    void write_info_header(std::ostream &file, BMPData *header);
    void write_signature(std::ostream &file);
}