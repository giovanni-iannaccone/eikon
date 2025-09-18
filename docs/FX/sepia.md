# Sepia

The sepia effect is a color transformation that gives an image a warm, brownish tone reminiscent of vintage photographs. It simulates the chemical aging of traditional black-and-white photos, adding a nostalgic and artistic feel.

For each pixel the algorithm extracts the RGB components and apply the sepia transformation:
- New Red = 0.393 * R + 0.769 * G + 0.189 * B
- New Green = 0.349 * R + 0.686 * G + 0.168 * B
- New Blue = 0.272 * R + 0.534 * G + 0.131 * B
    
Then it converts the new RGB values back to hexadecimal format and update the pixel.

```cpp
EikonCanvas *sepia() {
    uint8_t r {}, g {}, b {};

    for (uint y = 0; y < this->height; y++)
        for (uint x = 0; x < this->width; x++) {
            get_rgb(this->pixels[y][x], &r, &g, &b);

            this->pixels[y][x] = get_hex(
                0.393 * r + 0.769 * g + 0.189 * b,
                0.349 * r + 0.686 * g + 0.168 * b,
                0.272 * r + 0.534 * g + 0.131 * b
            );
        }

    return this;
}

```