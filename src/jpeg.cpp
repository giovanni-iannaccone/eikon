#include <cstdint>
#include <fstream>
#include <utility>

#include "../include/jpeg.hpp"

void get_jpeg_dimensions(std::istream &file, uint *height, uint *width) {

}

bool read_jpeg(std::istream &file, uint32_t **pixels, uint *height_ptr, uint *width_ptr) {
    return true;
}

bool save_jpeg(std::ostream &file, uint32_t **pixels, uint height, uint width, void *args) {
    return true;
}
