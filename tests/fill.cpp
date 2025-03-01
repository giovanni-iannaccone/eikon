#include <fstream>
#include <iostream>

#include "../lib/draw.cpp"

#define HEIGHT 800
#define WIDTH  700

int main() {
    uint32_t pixels[HEIGHT * WIDTH];

    std::ofstream fd {"output.ppm", std::ios::out};
    if (!fd) {
        std::cerr << "Error with file";
        return 1;
    }

    fill(pixels, HEIGHT, WIDTH, 0xFFA1FF15);
    save_to_ppm(fd, pixels, HEIGHT, WIDTH);
    fd.close();

    return 0;
}