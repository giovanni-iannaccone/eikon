#include "../include/utils.hpp"

uint32_t get_hex(uint8_t r, uint8_t g, uint8_t b) {
    return (((0xFF << 8) | b) << 8 | g) << 8 | r;
}

uint32_t get_alpha_blend_color(uint32_t c1, uint32_t c2) {
    uint8_t r1 {}, g1 {}, b1 {};
    get_rgb(c1, &r1, &g1, &b1);

    uint8_t a = (c2 >> (8 * 3)) & 0xFF;
    uint8_t r2 {}, g2 {}, b2 {};
    get_rgb(c2, &r2, &g2, &b2);

    uint8_t nr = (a * r2 + (255 - a) * r1) / 255;
    uint8_t ng = (a * g2 + (255 - a) * g1) / 255;
    uint8_t nb = (a * b2 + (255 - a) * b1) / 255;

    return get_hex(nr, ng, nb);
}

uint8_t get_pixel_brightness(uint32_t pixel) {
    uint8_t r {}, g {}, b {};
    get_rgb(pixel, &r, &g, &b);

    return std::max(
        r,
        std::max(g, b)
    );
}

void get_rgb(uint32_t pixel, uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = (pixel >> (8 * 0)) & 0xFF;
    *g = (pixel >> (8 * 1)) & 0xFF;
    *b = (pixel >> (8 * 2)) & 0xFF;
}

void hsv_2_rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b) {
    float c = v * s;
    float fHPrime = fmod(h / 60.0, 6);
    float fX = c * (1 - fabs(fmod(fHPrime, 2) - 1));
    float fM = v - c;
    
    if(0 <= fHPrime && fHPrime < 1) {
        *r = c;
        *g = fX;
        *b = 0;
    } else if(1 <= fHPrime && fHPrime < 2) {
        *r = fX;
        *g = c;
        *b = 0;
    } else if(2 <= fHPrime && fHPrime < 3) {
        *r = 0;
        *g = c;
        *b = fX;
    } else if(3 <= fHPrime && fHPrime < 4) {
        *r = 0;
        *g = fX;
        *b = c;
    } else if(4 <= fHPrime && fHPrime < 5) {
        *r = fX;
        *g = 0;
        *b = c;
    } else if(5 <= fHPrime && fHPrime < 6) {
        *r = c;
        *g = 0;
        *b = fX;
    } else {
        *r = 0;
        *g = 0;
        *b = 0;
    }
    
    *r += fM;
    *g += fM;
    *b += fM;
}

void rgb_2_hsv(uint8_t r, uint8_t g, uint8_t b, float *h, float *s, float *v) {
    r = r / 255.0;
    g = g / 255.0;
    b = b / 255.0;

    double cmax = std::max(r, std::max(g, b));
    double cmin = std::min(r, std::min(g, b));
    double diff = cmax - cmin;
    *h = -1, *s = -1;

    if (cmax == cmin)
        *h = 0;

    else if (cmax == r)
        *h = fmod(60 * ((g - b) / diff) + 360, 360);

    else if (cmax == g)
        *h = fmod(60 * ((b - r) / diff) + 120, 360);

    else if (cmax == b)
        *h = fmod(60 * ((r - g) / diff) + 240, 360);

    if (cmax == 0)
        *s = 0;
    else
        *s = (diff / cmax) * 100;

    *v = cmax * 100;
}

void sort_points(size_t *x1, size_t *y1, size_t *x2, size_t *y2, size_t *x3, size_t *y3) {
    if (*y1 > *y2) { std::swap(*y1, *y2); std::swap(*x1, *x2); }
    if (*y1 > *y3) { std::swap(*y1, *y3); std::swap(*x1, *x3); }
    if (*y2 > *y3) { std::swap(*y2, *y3); std::swap(*x2, *x3); }
}
