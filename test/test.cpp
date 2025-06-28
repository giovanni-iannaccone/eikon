#include <fstream>
#include <iostream>
#include <string>

#include "../include/raccoon.hpp"

#define HEIGHT 800
#define WIDTH  800

#define RETURN_DEFER(fd, status_code)               \
    canvas->save_to_ppm(fd);                        \
    fd.close();                                     \
    return status_code

#define RUN_TEST(test_function, file) do {          \
    if (test_function(file)) {                      \
        std::cerr << "Error with " << file << "\n"; \
        return 1;                                   \
    }                                               \
} while (0)

static uint32_t pixels[HEIGHT * WIDTH];
static RaccoonCanvas* canvas = new RaccoonCanvas(pixels, HEIGHT, WIDTH);

auto test_circle(const std::string &file_name) -> int {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    canvas->fill(0xFF000000);
    canvas->circle(400, 400, 100.0, 0xFFFF0000);

    RETURN_DEFER(fd, 0);
}

auto test_copy(const std::string &file_name) -> int {
    std::ifstream input_file {file_name, std::ios::in};
    if (!input_file) return 1;

    canvas->read_ppm(input_file);
    input_file.close();

    std::ofstream output_file {"./outputs/copy.ppm", std::ios::out};
    if (!output_file) return 1;
    
    RETURN_DEFER(output_file, 0);
}

auto test_flip_rectangle(const std::string &file_name) -> int {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    canvas->fill(0xFF000000);
    canvas->rectangle(150, 200, 100, 200, 0xFFA1FF15);
    canvas->flip();

    RETURN_DEFER(fd, 0);
}

auto test_line(const std::string file_name) -> int {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    canvas->fill(0xFF000000);
    canvas->line(0, 0, canvas->width, canvas->height, 0xFFFF00FF);

    RETURN_DEFER(fd, 0);
}

auto test_overlap(const std::string &file_name) -> int {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    canvas->fill(0xFF000000);

    canvas->circle(400, 400, 100, 0xFFF0000);
    canvas->circle(300, 300, 75, 0xFF0000FF, 0.5);

    RETURN_DEFER(fd, 0);
}

auto test_rotate_rectangle(const std::string &file_name) -> int {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    canvas->fill(0xFF000000);
    canvas->rectangle(150, 200, 100, 200, 0xFFA1FF15);
    canvas->rotate();

    RETURN_DEFER(fd, 0);
}

auto test_saturation(const std::string &file_name) -> int {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    canvas->fill(0xFF000000);
    canvas->circle(400, 400, 100.0, 0xFF00FF00);
    canvas->saturation(10);

    RETURN_DEFER(fd, 0);
}

auto test_stretch(const std::string &file_name) -> int {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    canvas->fill(0xFF000000);
    canvas->circle(400, 400, 100.0, 0xFFFFFFFF);
    canvas->stretch(3);

    canvas->save_to_ppm(fd);

    free(canvas->pixels);
    canvas->pixels = pixels;
    canvas->height = HEIGHT;
    canvas->width = WIDTH;

    fd.close();
    return 0;
}

auto test_text(const std::string &file_name) -> int {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    canvas->fill(0xFF000000);
    canvas->text(std::string{"hello, world!"}, 150, 200, 10, 0xFF00FF00, default_font);

    RETURN_DEFER(fd, 0);
}

auto test_triangle(const std::string &file_name) -> int {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    canvas->fill(0xFF000000);
    canvas->triangle(100, 100, 600, 200, 400, 500, 0xFFEE00FF);

    RETURN_DEFER(fd, 0);
}

auto main() -> int {
    RUN_TEST(test_circle, "./outputs/circle.ppm");
    RUN_TEST(test_copy, "./outputs/circle.ppm");
    RUN_TEST(test_flip_rectangle, "./outputs/flipped_rectangle.ppm");
    RUN_TEST(test_line, "./outputs/line.ppm");
    RUN_TEST(test_overlap, "./outputs/overlapped_cirles.ppm");
    RUN_TEST(test_rotate_rectangle, "./outputs/rotated_rectangle.ppm");
    RUN_TEST(test_saturation, "./outputs/saturation.ppm");
    RUN_TEST(test_stretch, "./outputs/stretch.ppm");
    RUN_TEST(test_text, "./outputs/text.ppm");
    RUN_TEST(test_triangle, "./outputs/triangle.ppm");

    return 0;
}