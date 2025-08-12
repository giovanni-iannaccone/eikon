#pragma once

#include <cstdint>
#include <fstream>
#include <utility>

void get_jpeg_dimensions(std::istream &file, size_t *height, size_t *width);

bool read_jpeg(std::istream &file, uint32_t pixels[], size_t *height_ptr, size_t *width_ptr);
bool save_jpeg(std::ostream &file, uint32_t pixels[], size_t height, size_t width, void *args = nullptr);
