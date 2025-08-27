#pragma once

#include <cstdint>
#include <fstream>
#include <utility>

namespace ppm {
    void get_dimensions(std::istream &file, uint *height, uint *width);

    bool read(std::istream &file, uint32_t **pixels, uint *height_ptr, uint *width_ptr);
    bool save(std::ostream &file, uint32_t **pixels, uint height, uint width, void *args = nullptr);
}