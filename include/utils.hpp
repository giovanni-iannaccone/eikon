#pragma once

#include <bits/stdc++.h>
#include <cstdint>
#include <cmath>

char get_byte(std::istream &file);

uint32_t get_hex(uint8_t r, uint8_t g, uint8_t b);
void get_rgb(uint32_t pixel, uint8_t *r, uint8_t *g, uint8_t *b);

uint32_t get_alpha_blend_color(uint32_t c1, uint32_t c2);
uint8_t get_pixel_brightness(uint32_t pixel);

void hsi_2_rgb(uint H, float S, float I, uint8_t *R, uint8_t *G, uint8_t *B);
void rgb_2_hsi(uint8_t R, uint8_t G, uint8_t B, uint *H, float *S, float *I);

void hsv_2_rgb(uint H, float S, float V, uint8_t *R, uint8_t *G, uint8_t *B);
void rgb_2_hsv(uint8_t R, uint8_t G, uint8_t B, uint *H, float *S, float *v);

void free_pixels(uint32_t **pixels, uint height);

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
