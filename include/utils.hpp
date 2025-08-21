#pragma once

#include <bits/stdc++.h>
#include <cstdint>

uint32_t get_hex(uint8_t r, uint8_t g, uint8_t b);
uint8_t get_pixel_brightness(uint32_t pixel);
void get_rgb(uint32_t pixel, uint8_t *r, uint8_t *g, uint8_t *b);

void hsv_2_rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b);
void rgb_2_hsv(uint8_t r, uint8_t g, uint8_t b, float *h, float *s, float *v);

template <typename T>
bool in(const T& element, const std::vector<T>& vec) {
    return std::find(vec.begin(), vec.end(), element) != vec.end();
}

uint32_t mix_colors(uint32_t first_color, uint32_t second_color, float second_opacity);

void sort_points(size_t *x1, size_t *y1, size_t *x2, size_t *y2, size_t *x3, size_t *y3);
