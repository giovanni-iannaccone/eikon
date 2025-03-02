#include <fstream>
#include <iostream>

#include "../include/raccoon.hpp"

#define HEIGHT 800
#define WIDTH  700

int main() {
    uint32_t pixels[HEIGHT * WIDTH];
    Canvas canvas {pixels, HEIGHT, WIDTH};

    std::ofstream fd {"./outputs/output.ppm", std::ios::out};
    if (!fd) {
        std::cerr << "Error with file";
        return 1;
    }

    fill(canvas, 0xFF000000);
    shapes::rectangle(canvas, 150, 200, 300, 400, 0xFFA1FF15);
    effects::flip_ppm(canvas);

    save_to_ppm(fd, canvas);
    fd.close();

    return 0;
}