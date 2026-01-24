#include <cmath>

#include "../include/utils.hpp"

namespace eikon {

namespace utils {

void free_pixels(uint32_t **pixels, uint height) {
    for (uint y = 0; y < height; y++)
        delete[] pixels[y];

    delete[] pixels;
}

char get_byte(std::istream &file) {
    char byte {};
    file.read(&byte, sizeof(char));
    return byte;
}

uint8_t get_pixel_brightness(uint32_t pixel) noexcept {
    uint8_t r {}, g {}, b {};
    get_rgb(pixel, r, g, b);

    return max(r, g, b);
}

void hsi_2_rgb(uint H, float S, float I, uint8_t *R, uint8_t *G, uint8_t *B) {
    H = H % 360;

    float x = 2 * I * S;
    float y = cos(H * (M_PI / 180.0));
    float z = cos((60 - H) * (M_PI / 180.0));

    float r {}, g {}, b {};

    if (H == 0) {
        r = I + x;
        g = b = I - I * S;
    } else if (H < 120) {
        r = I + x * y / z;
        g = I + x * (1 - y / z);
        b = I - x;
    } else if (H == 120) {
        r = I - x;
        g = I + x;
        b = I - x;
    } else if (H < 240) {
        H = H - 120;
        r = I - x;
        g = I + x * y / z;
        b = I + x * (1 - y / z);
    } else if (H == 240) {
        r = I - x;
        g = I - x;
        b = I + x;
    } else {
        H = H - 240;
        r = I + x * (1 - y / z);
        g = I - x;
        b = I + x * y / z;
    }

    *R = fmin(fmax(r * 255, 0), 255);
    *G = fmin(fmax(g * 255, 0), 255);
    *B = fmin(fmax(b * 255, 0), 255);
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

void increase_brightness(uint32_t &pixel, float inc) noexcept {
    uint8_t r {}, g {}, b {};
    get_rgb(pixel, r, g, b);

    r = std::min(255.0f, r * inc);
    g = std::min(255.0f, g * inc);
    b = std::min(255.0f, b * inc);

    pixel = get_hex(r, g, b);
}

void rgb_2_hsi(uint8_t R, uint8_t G, uint8_t B, uint *H, float *S, float *I) {
    float r = R / 255.0f;
    float g = G / 255.0f;
    float b = B / 255.0f;

    *I = (r + g + b) / 3.0f;

    float minimum = min(r, g, b);
    float maximum = max(r, g, b);
    float delta = maximum - minimum;

    *S = (maximum == 0) ? 0 : (1 - (minimum / maximum));

    if (delta == 0) {
        *H = 0;
    } else {
        if (maximum == r)
            *H = 60 * fmod(((g - b) / delta), 6);
        else if (maximum == g)
            *H = 60 * (((b - r) / delta) + 2);
        else
            *H = 60 * (((r - g) / delta) + 4);
    }
}

void rgb_2_hsv(uint8_t R, uint8_t G, uint8_t B, uint *H, float *S, float *V) {
    float r = R / 255.0f;
    float g = G / 255.0f;
    float b = B / 255.0f;

    double cmax = max(r, g, b);
    double cmin = min(r, g, b);
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

void skip_bytes(std::istream &file, uint bytes) {
    uint curr = file.tellg();
    file.seekg(curr + bytes);
}

void write_byte(std::ostream &file, const char data) {
    file.write(&data, sizeof(data));
}

void write_repeated(std::ostream &file, uint32_t color, uint8_t reps) {
    uint8_t r {}, g {}, b {};
    get_rgb(color, r, g, b);

    for (uint8_t i = 0; i < reps; i++) {
        write_byte(file, reps);

        write_byte(file, b);
        write_byte(file, g);
        write_byte(file, r);
    }
}
    
} // namespace utils

} // namespace eikon
