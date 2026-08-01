#include <cmath>

#include "../include/utils.hpp"

namespace eikon::utils {

void free_pixels(uint32_t **pixels, uint height)
{
    for (uint y = 0; y < height; y++)
        delete[] pixels[y];

    delete[] pixels;
}

hsi::operator rgb() const noexcept
{
    float r, g, b;
    uint H = this->h % 360;

    float x = 2 * this->i * this->s;
    float y = cos(H * (M_PI / 180.0));
    float z = cos((60 - H) * (M_PI / 180.0));

    if (H == 0) {
        r = this->i + x;
        g = b = this->i - this->i * this->s;
    } else if (H < 120) {
        r = this->i + x * y / z;
        g = this->i + x * (1 - y / z);
        b = this->i - x;
    } else if (H == 120) {
        r = this->i - x;
        g = this->i + x;
        b = this->i - x;
    } else if (H < 240) {
        H = H - 120;
        r = this->i - x;
        g = this->i + x * y / z;
        b = this->i + x * (1 - y / z);
    } else if (H == 240) {
        r = this->i - x;
        g = this->i - x;
        b = this->i + x;
    } else {
        H = H - 240;
        r = this->i + x * (1 - y / z);
        g = this->i - x;
        b = this->i + x * y / z;
    }

    return {
        (uint8_t)fmin(fmax(r * 255, 0), 255),
        (uint8_t)fmin(fmax(g * 255, 0), 255),
        (uint8_t)fmin(fmax(b * 255, 0), 255)
    };
}

hsv::operator rgb() const noexcept
{
    float c = this->v * this->s;
    float fHPrime = fmod(this->h / 60.0, 6);
    float fX = c * (1 - fabs(fmod(fHPrime, 2) - 1));
    float fM = this->v - c;

    uint8_t R, G, B;
    
    if (0 <= fHPrime && fHPrime < 1) {
        R = c;
        G = fX;
        B = 0;
    } else if (1 <= fHPrime && fHPrime < 2) {
        R = fX;
        G = c;
        B = 0;
    } else if (2 <= fHPrime && fHPrime < 3) {
        R = 0;
        G = c;
        B = fX;
    } else if (3 <= fHPrime && fHPrime < 4) {
        R = 0;
        G = fX;
        B = c;
    } else if (4 <= fHPrime && fHPrime < 5) {
        R = fX;
        G = 0;
        B = c;
    } else if (5 <= fHPrime && fHPrime < 6) {
        R = c;
        G = 0;
        B = fX;
    } else {
        R = 0;
        G = 0;
        B = 0;
    }
    
    return {
        (uint8_t)(R + fM),
        (uint8_t)(G + fM),
        (uint8_t)(B + fM)
    };
}

rgb::operator hsi() const noexcept
{
    float r = this->r / 255.0f;
    float g = this->g / 255.0f;
    float b = this->b / 255.0f;

    uint H;
    float S, I;
    
    I = (r + g + b) / 3.0f;

    float minimum = std::min({r, g, b});
    float maximum = std::max({r, g, b});
    float delta = maximum - minimum;

    S = (maximum == 0) ? 0 : (1 - (minimum / maximum));

    if (delta == 0) {
        H = 0;
    } else {
        if (maximum == r)
            H = 60 * fmod(((g - b) / delta), 6);
        else if (maximum == g)
            H = 60 * (((b - r) / delta) + 2);
        else
            H = 60 * (((r - g) / delta) + 4);
    }

    return {H, S, I};
}

rgb::operator hsv() const noexcept
{
    float r = this->r / 255.0f;
    float g = this->g / 255.0f;
    float b = this->b / 255.0f;

    float cmax = std::max({r, g, b});
    float cmin = std::min({r, g, b});
    float diff = cmax - cmin;

    uint H = -1;
    float S = -1;
    float V;
    
    if (cmax == cmin)
        H = 0;

    else if (cmax == r)
        H = fmod(60 * ((g - b) / diff) + 360, 360);

    else if (cmax == g)
        H = fmod(60 * ((b - r) / diff) + 120, 360);

    else if (cmax == b)
        H = fmod(60 * ((r - g) / diff) + 240, 360);

    if (cmax == 0)
        S = 0;
    else
        S = (diff / cmax) * 100;

    V = cmax * 100;
    return {H, S, V};
}
    
} // namespace utils
