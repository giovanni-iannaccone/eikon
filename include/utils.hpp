#pragma once

#include <bits/stdc++.h>
#include <cstdint>

uint32_t get_hex(uint8_t r, uint8_t g, uint8_t b);
void get_rgb(uint32_t pixel, uint8_t *r, uint8_t *g, uint8_t *b);

uint32_t get_alpha_blend_color(uint32_t c1, uint32_t c2);
uint8_t get_pixel_brightness(uint32_t pixel);

void hsv_2_rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b);
void rgb_2_hsv(uint8_t r, uint8_t g, uint8_t b, float *h, float *s, float *v);

template <typename T>
bool in(const T& element, const std::vector<T>& vec) {
    return std::find(vec.begin(), vec.end(), element) != vec.end();
}

void sort_points(uint *x1, uint *y1, uint *x2, uint *y2, uint *x3, uint *y3);
