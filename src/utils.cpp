#include "../include/utils.hpp"

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

void get_byte(std::istream &file, char *dst) {
    file.read(dst, sizeof(unsigned char));
}

uint32_t get_hex(uint8_t r, uint8_t g, uint8_t b) {
    return (((0xFF << 8) | r) << 8 | g) << 8 | b;
}

uint8_t get_pixel_brightness(uint32_t pixel) {
    uint8_t r {}, g {}, b {};
    get_rgb(pixel, &r, &g, &b);

    return tmax(r, g, b);
}

void get_rgb(uint32_t pixel, uint8_t *r, uint8_t *g, uint8_t *b) {
    *b = (pixel >> (8 * 0)) & 0xFF;
    *g = (pixel >> (8 * 1)) & 0xFF;
    *r = (pixel >> (8 * 2)) & 0xFF;
}

void hsi_2_rgb(uint H, float S, float I, uint8_t *R, uint8_t *G, uint8_t *B) {
    H = H % 360;

    float x = 2 * I * S;
    float y = cos(H * (M_PI / 180.0));
    float z = cos((60 - H) * (M_PI / 180.0));

    if (H == 0) {
        *R = I + x;
        *G = *B = I - I * S;
    } else if (H < 120) {
        *R = I + x * y / z;
        *G = I + x * (1 - y / z);
        *B = I - x;
    } else if (H == 120) {
        *R = I - x;
        *G = I + x;
        *B = I - x;
    } else if (H < 240) {
        H = H - 120;
        *R = I - x;
        *G = I + x * y / z;
        *B = I + x * (1 - y / z);
    } else if (H == 240) {
        *R = I - x;
        *G = I - x;
        *B = I + x;
    } else {
        H = H - 240;
        *R = I + x * (1 - y / z);
        *G = I - x;
        *B = I + x * y / z;
    }

    *R = fmin(fmax(*R * 255, 0), 255);
    *G = fmin(fmax(*G * 255, 0), 255);
    *B = fmin(fmax(*B * 255, 0), 255);
}

void hsv_2_rgb(uint H, float S, float V, uint8_t *R, uint8_t *G, uint8_t *B) {
    float c = V * S;
    float fHPrime = fmod(H / 60.0, 6);
    float fX = c * (1 - fabs(fmod(fHPrime, 2) - 1));
    float fM = V - c;
    
    if(0 <= fHPrime && fHPrime < 1) {
        *R = c;
        *G = fX;
        *B = 0;
    } else if(1 <= fHPrime && fHPrime < 2) {
        *R = fX;
        *G = c;
        *B = 0;
    } else if(2 <= fHPrime && fHPrime < 3) {
        *R = 0;
        *G = c;
        *B = fX;
    } else if(3 <= fHPrime && fHPrime < 4) {
        *R = 0;
        *G = fX;
        *B = c;
    } else if(4 <= fHPrime && fHPrime < 5) {
        *R = fX;
        *G = 0;
        *B = c;
    } else if(5 <= fHPrime && fHPrime < 6) {
        *R = c;
        *G = 0;
        *B = fX;
    } else {
        *R = 0;
        *G = 0;
        *B = 0;
    }
    
    *R += fM;
    *G += fM;
    *B += fM;
}

void rgb_2_hsi(uint8_t R, uint8_t G, uint8_t B, uint *H, float *S, float *I) {
    float r = R / 255.0f;
    float g = G / 255.0f;
    float b = B / 255.0f;

    *I = (r + g + b) / 3.0f;

    float min = tmin(r, g, b);
    float max = tmax(r, g, b);
    float delta = max - min;

    *S = (max == 0) ? 0 : (1 - (min / max));

    if (delta == 0) {
        *H = 0;
    } else {
        if (max == r)
            *H = 60 * fmod(((g - b) / delta), 6);
        else if (max == g)
            *H = 60 * (((b - r) / delta) + 2);
        else
            *H = 60 * (((r - g) / delta) + 4);
    }
}

void rgb_2_hsv(uint8_t R, uint8_t G, uint8_t B, uint *H, float *S, float *V) {
    float r = R / 255.0f;
    float g = G / 255.0f;
    float b = B / 255.0f;

    double cmax = tmax(r, g, b);
    double cmin = tmin(r, g, b);
    double diff = cmax - cmin;
    *H = -1, *S = -1;

    if (cmax == cmin)
        *H = 0;

    else if (cmax == r)
        *H = fmod(60 * ((g - b) / diff) + 360, 360);

    else if (cmax == g)
        *H = fmod(60 * ((b - r) / diff) + 120, 360);

    else if (cmax == b)
        *H = fmod(60 * ((r - g) / diff) + 240, 360);

    if (cmax == 0)
        *S = 0;
    else
        *S = (diff / cmax) * 100;

    *V = cmax * 100;
}
