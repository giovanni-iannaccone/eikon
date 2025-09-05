#pragma once

#include <cstdint>
#include <cmath>
#include <fstream>
#include <map>
#include <type_traits>
#include <vector>

enum FileType {
    BMP,
    PNG,
    PPM
};

FileType detect_filetype(const std::string &file_name);

void free_pixels(uint32_t **pixels, uint height);

char get_byte(std::istream &file);
char write_byte(std::ostream &file, char data);

uint32_t get_hex(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
void get_rgb(uint32_t pixel, uint8_t *r, uint8_t *g, uint8_t *b);

uint32_t get_alpha_blend_color(uint32_t c1, uint32_t c2);
uint8_t get_pixel_brightness(uint32_t pixel);

void hsi_2_rgb(uint H, float S, float I, uint8_t *R, uint8_t *G, uint8_t *B);
void rgb_2_hsi(uint8_t R, uint8_t G, uint8_t B, uint *H, float *S, float *I);

void hsv_2_rgb(uint H, float S, float V, uint8_t *R, uint8_t *G, uint8_t *B);
void rgb_2_hsv(uint8_t R, uint8_t G, uint8_t B, uint *H, float *S, float *v);

void write_repeated(std::ostream &file, uint32_t color, uint8_t reps);

template <typename T>
bool in(const T& element, const std::vector<T>& vec) {
    return std::find(vec.begin(), vec.end(), element) != vec.end();
}

template <typename T>
constexpr const T &tmax(const T &a, const T &b, const T &c) {
    return std::max(a, std::max(b, c));
}

template <typename T>
constexpr const T &tmin(const T &a, const T &b, const T &c) {
    return std::min(a, std::min(b, c));
}

template <typename T>
T BE_get_bytes(std::istream &file) {
    T result = 0;
    char dst {};

    for (size_t i = 0; i < sizeof(T); i++) {
        file.read(&dst, sizeof(char));
        result <<= 8;
        result |= dst;
    }

    return result;
}

template <typename T>
T LE_get_bytes(std::istream &file) {
    T result = 0;
    char dst {};

    for (size_t i = 0; i < sizeof(T); i++) {
        file.read(&dst, sizeof(char));
        dst <<= 8;
        result |= dst;
    }

    return result;
}

template <typename T>
void BE_write_as_bytes(std::ostream &file, T data) {
    if constexpr (std::endian::native == std::endian::big) {
        file.write(reinterpret_cast<const char *>(&data), sizeof(data));

    } else {
        
    }
}

template <typename T>
void LE_write_as_bytes(std::ostream &file, T data) {
    if constexpr (std::endian::native == std::endian::big) {
        
    } else {
        file.write(reinterpret_cast<const char *>(&data), sizeof(data));
    }
}