#include <fstream>
#include <iostream>
#include <string>

#include "../include/raccoon.hpp"

#define HEIGHT 800
#define WIDTH  800

#define RUN_TEST(test_function, file) do {          \
    if (test_function(pixels, file)) {              \
        std::cerr << "Error with " << file << "\n"; \
        return 1;                                   \
    }                                               \
} while (0)

auto test_circle(uint32_t *pixels, const std::string file_name) -> int {
    Canvas canvas {pixels, HEIGHT, WIDTH};

    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    fill(canvas, 0xFF000000);
    shapes::circle(canvas, 400, 400, 100.0, 0xFFFF0000);

    save_to_ppm(fd, canvas);
    fd.close();
    return 0;
}

auto test_flip_rectangle(uint32_t *pixels, const std::string file_name) -> int {
    Canvas canvas {pixels, HEIGHT, WIDTH};

    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    fill(canvas, 0xFF000000);
    shapes::rectangle(canvas, 150, 200, 100, 200, 0xFFA1FF15);
    effects::flip_ppm(canvas);

    save_to_ppm(fd, canvas);
    fd.close();
    return 0;
}

auto test_rotate_rectangle(uint32_t *pixels, const std::string file_name) -> int {
    Canvas canvas {pixels, HEIGHT, WIDTH};

    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    fill(canvas, 0xFF000000);
    shapes::rectangle(canvas, 150, 200, 100, 200, 0xFFA1FF15);
    effects::rotate_ppm(canvas);

    save_to_ppm(fd, canvas);
    fd.close();
    return 0;
}

int main() {
    uint32_t pixels[HEIGHT * WIDTH];

    RUN_TEST(test_circle, "./outputs/circle.ppm");
    RUN_TEST(test_flip_rectangle, "./outputs/flipped_rectangle.ppm");
    RUN_TEST(test_rotate_rectangle, "./outputs/rotated_rectangle");

    return 0;
}