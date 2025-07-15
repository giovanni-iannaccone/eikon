#include <fstream>
#include <iostream>
#include <string>

#include "test_utils.hpp"
#include "../include/raccoon.hpp"

#define HEIGHT 800
#define WIDTH  800

static uint32_t pixels[HEIGHT * WIDTH];
static RaccoonCanvas* canvas = new RaccoonCanvas(pixels, HEIGHT, WIDTH);

int test_circle(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    auto circle = new Circle(100.0, 400, 400, 0xFFFF0000);

    canvas->fill(0xFF000000)
        ->draw(circle);

    return defer(canvas, fd, 0, circle);
}

int test_copy(const std::string &file_name) {
    std::ifstream input_file {file_name, std::ios::in};
    if (!input_file) return 1;

    canvas->read_ppm(input_file);
    input_file.close();

    std::ofstream output_file {"./outputs/copy.ppm", std::ios::out};
    if (!output_file) return 1;
    
    return defer(canvas, output_file, 0, nullptr);
}

int test_flip_rectangle(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    auto rec = new Rectangle(150, 200, 100, 200, 0xFFA1FF15);

    canvas->fill(0xFF000000)
        ->draw(rec)
        ->flip();

    return defer(canvas, fd, 0, rec);
}

int test_line(const std::string file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    auto l = new Line(0, 0, WIDTH, HEIGHT, 0xFFFF00FF);

    canvas->fill(0xFF000000)
        ->draw(l);

    return defer(canvas, fd, 0, l);
}

int test_overlap(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    canvas->fill(0xFF000000);

    auto circle = new Circle(100, 400, 400, 0xFFF0000);
    canvas->draw(circle);
    delete circle;

    circle = new Circle(75, 300, 300, 0xFF0000FF, 0.5);
    canvas->draw(circle);

    return defer(canvas, fd, 0, circle);
}

int test_rotate_rectangle(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    auto rec = new Rectangle(150, 200, 100, 200, 0xFFA1FF15);
    canvas->fill(0xFF000000)
        ->draw(rec)
        ->rotate();

    return defer(canvas, fd, 0, rec);
}

int test_saturation(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    auto circle = new Circle(100.0, 400, 400, 0xFF00FF00);

    canvas->fill(0xFF000000)
        ->draw(circle)
        ->saturation(10);

    return defer(canvas, fd, 0, circle);
}

int test_stretch(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    canvas->fill(0xFF000000);

    auto circle = new Circle(100.0, 400, 400, 0xFFFFFFFF);
    canvas->draw(circle)
        ->stretch(3);

    return defer(canvas, fd, 0, circle);
}

int test_text(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    std::string word = "hello, world!";
    auto text = new Text(&word, 150, 200, 10, 0xFF00FF00, &default_font);

    canvas->fill(0xFF000000)
        ->draw(text);

    return defer(canvas, fd, 0, text);
}

int test_triangle(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    auto triangle = new Triangle(100, 100, 600, 200, 400, 500, 0xFFEE00FF);

    canvas->fill(0xFF000000)
        ->draw(triangle);

    return defer(canvas, fd, 0, triangle);
}

int main() {
    RUN_TEST(test_circle, "./outputs/circle.ppm");
    RUN_TEST(test_copy, "./outputs/circle.ppm");
    RUN_TEST(test_flip_rectangle, "./outputs/flipped_rectangle.ppm");
    RUN_TEST(test_line, "./outputs/line.ppm");
    RUN_TEST(test_overlap, "./outputs/overlapped_cirles.ppm");
    RUN_TEST(test_rotate_rectangle, "./outputs/rotated_rectangle.ppm");
    RUN_TEST(test_saturation, "./outputs/saturation.ppm");
    RUN_TEST(test_text, "./outputs/text.ppm");
    RUN_TEST(test_triangle, "./outputs/triangle.ppm");
    
    RUN_TEST(test_stretch, "./outputs/stretch.ppm");

    delete canvas;
    return 0;
}