#include <fstream>
#include <iostream>
#include <string>

#include "../include/raccoon.hpp"

#define HEIGHT 800
#define WIDTH  800

#define RETURN_DEFER(fd, canvas, status_code)       \
    save_to_ppm(fd, canvas);                        \
    fd.close();                                     \
    return status_code

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

    RETURN_DEFER(fd, canvas, 0);
}

auto test_copy(uint32_t *pixels, const std::string file_name) -> int {
    Canvas canvas {pixels, HEIGHT, WIDTH};

    std::ifstream input_file {file_name, std::ios::in};
    if (!input_file) return 1;

    read_ppm(input_file, canvas);
    input_file.close();

    std::ofstream output_file {"./outputs/copy.ppm", std::ios::out};
    if (!output_file) return 1;
    
    RETURN_DEFER(output_file, canvas, 0);
}

auto test_flip_rectangle(uint32_t *pixels, const std::string file_name) -> int {
    Canvas canvas {pixels, HEIGHT, WIDTH};

    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    fill(canvas, 0xFF000000);
    shapes::rectangle(canvas, 150, 200, 100, 200, 0xFFA1FF15);
    effects::flip_ppm(canvas);

    RETURN_DEFER(fd, canvas, 0);
}

auto test_line(uint32_t *pixels, const std::string file_name) -> int {
    Canvas canvas {pixels, HEIGHT, WIDTH};

    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    fill(canvas, 0xFF000000);
    shapes::line(canvas, 0, 0, canvas.width, canvas.height, 0xFFFF00FF);

    RETURN_DEFER(fd, canvas, 0);
}

auto test_overlap(uint32_t *pixels, const std::string file_name) -> int {
    Canvas canvas {pixels, HEIGHT, WIDTH};

    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    fill(canvas, 0xFF000000);

    shapes::circle(canvas, 400, 400, 100, 0xFFF0000);
    shapes::circle(canvas, 300, 300, 75, 0xFF0000FF, 0.5);

    RETURN_DEFER(fd, canvas, 0);
}

auto test_rotate_rectangle(uint32_t *pixels, const std::string file_name) -> int {
    Canvas canvas {pixels, HEIGHT, WIDTH};

    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    fill(canvas, 0xFF000000);
    shapes::rectangle(canvas, 150, 200, 100, 200, 0xFFA1FF15);
    effects::rotate_ppm(canvas);

    RETURN_DEFER(fd, canvas, 0);
}

auto test_text(uint32_t *pixels, const std::string file_name) -> int {
    Canvas canvas {pixels, HEIGHT, WIDTH};

    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    fill(canvas, 0xFF000000);
    shapes::text(canvas, std::string{"hello, world!"}, 150, 200, 10, 0xFF00FF00, default_font);

    RETURN_DEFER(fd, canvas, 0);
}

auto test_triangle(uint32_t *pixels, const std::string file_name) -> int {
    Canvas canvas {pixels, HEIGHT, WIDTH};

    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    fill(canvas, 0xFF000000);
    shapes::triangle(canvas, 100, 100, 600, 200, 400, 500, 0xFFEE00FF);

    RETURN_DEFER(fd, canvas, 0);
}

auto main() -> int {
    uint32_t pixels[HEIGHT * WIDTH];

    RUN_TEST(test_circle, "./outputs/circle.ppm");
    RUN_TEST(test_copy, "./outputs/circle.ppm");
    RUN_TEST(test_flip_rectangle, "./outputs/flipped_rectangle.ppm");
    RUN_TEST(test_line, "./outputs/line.ppm");
    RUN_TEST(test_overlap, "./outputs/overlapped_cirles.ppm");
    RUN_TEST(test_rotate_rectangle, "./outputs/rotated_rectangle.ppm");
    RUN_TEST(test_text, "./outputs/text.ppm");
    RUN_TEST(test_triangle, "./outputs/triangle.ppm");

    return 0;
}