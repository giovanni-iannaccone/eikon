#include <bits/stdc++.h>
#include <cstdint>

#include "utils.hpp"

class BMPData {

public:
    uint height, width;
    uint32_t **pixels;
};

namespace bmp {

    const uint signature_size = 2;

    void extract_signature(std::istream &file, char *signature);
    bool is_valid_signature(std::istream &file);

    BMPData read_header(uint *height_ptr, uint *width_ptr);
    
    bool read(std::istream &file, uint32_t **pixels, uint *height_ptr, uint *width_ptr);
    bool save(std::ostream &file, uint32_t **pixels, uint height, uint width, void *args = nullptr);

    void write_header(std::ostream &file, uint height, uint width, void *args);
    void write_signature(std::ostream &file);
}