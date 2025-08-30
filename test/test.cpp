#include <fstream>
#include <iostream>
#include <string>

#include <eikon/eikon.hpp>

#include "test_utils.hpp"

#define HEIGHT 800
#define WIDTH  800

static uint32_t pixels[HEIGHT * WIDTH];
static EikonCanvas *canvas = new EikonCanvas(pixels, HEIGHT, WIDTH);

int test_area(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    Circle circle {100.0, 400, 400, 0xFF0000FF};

    canvas->fill(0xFF000000)
        ->draw(circle)
        ->area(300, 300, 100, 100)
        ->flop();
    
    canvas->save(fd, FileType::PPM);
    
    fd.close();
    return 0;
}

int test_brightness(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};

    canvas->fill(0xFF000000)
        ->draw(rec)
        ->brightness(1.5)
        ->save(fd, FileType::PPM);

    fd.close();
    return 0;
}

int test_circle(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    Circle circle {100.0, 400, 400, 0xFF0000FF};

    canvas->fill(0xFF000000)
        ->draw(circle)
        ->save(fd, FileType::PPM);

    fd.close();
    return 0;
}

int test_contrast(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    Circle circle {100.0, 400, 400, 0xFF00007F};

    canvas->fill(0xFF0F0F0F)
        ->draw(circle)
        ->contrast(1.5)
        ->save(fd, FileType::PPM);

    fd.close();
    return 0;
}

int test_flip_rectangle(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};

    canvas->fill(0xFF000000)
        ->draw(rec)
        ->flip()
        ->save(fd, FileType::PPM);

    fd.close();
    return 0;
}

int test_flop_rectangle(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};

    canvas->fill(0xFF000000)
        ->draw(rec)
        ->flop()
        ->save(fd, FileType::PPM);

    fd.close();
    return 0;
}

int test_gray_scale(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    Rectangle rec {150, 200, 100, 200, 0xFFFF00AB};

    canvas->fill(0xFF15FFA1)
        ->draw(rec)
        ->gray_scale()
        ->save(fd, FileType::PPM);

    fd.close();
    return 0;
}

int test_line(const std::string file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    Line l {0, 0, WIDTH, HEIGHT, 0xFFFF00FF};

    canvas->fill(0xFF000000)
        ->draw(l)
        ->save(fd, FileType::PPM);

    fd.close();
    return 0;
}

int test_negate(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    canvas->fill(0xFF000000);

    Rectangle r1 = {100, 200, 200, 300, 0xFFFF00FF};
    Circle c1 = {75, 300, 300, 0x7FFF0000};
    Circle c2 = {150, 400, 400, 0xFF00FF00};

    canvas->draw(r1)
        ->draw(c1)
        ->draw(c2)
        ->negate()
        ->save(fd, FileType::PPM);
    
    fd.close();
    return 0;
}

int test_overlap(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    canvas->fill(0xFF000000);

    Circle circle {100, 400, 400, 0xFF0000FF};
    canvas->draw(circle);

    circle = {75, 300, 300, 0x7FFF0000};
    canvas->draw(circle);

    canvas->save(fd, FileType::PPM);
    
    fd.close();
    return 0;
}

int test_roll(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    Rectangle rec {50, 200, 100, 200, 0xFF15FFA1};
    canvas->fill(0xFF000000)
        ->draw(rec)
        ->roll(100)
        ->save(fd, FileType::PPM);

    fd.close();
    return 0;
}

int test_rotate_rectangle(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};
    canvas->fill(0xFF000000)
        ->draw(rec)
        ->rotate()
        ->save(fd, FileType::PPM);

    fd.close();
    return 0;
}

int test_saturation(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    Circle circle {100.0, 400, 400, 0xFF00FF00};

    canvas->fill(0xFF000000)
        ->draw(circle)
        ->saturation(1.5)
        ->save(fd, FileType::PPM);

    fd.close();
    return 0;
}

int test_sepia(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    Rectangle rec {150, 200, 100, 200, 0xFFFF00AB};
    Circle circle {100, 400, 400, 0xFFFF0000};

    canvas->fill(0xFF15FFA1)
        ->draw(rec)
        ->draw(circle)
        ->sepia()
        ->save(fd, FileType::PPM);

    fd.close();
    return 0;
}

int test_stretch(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    uint32_t **new_pixels {};
    canvas->fill(0xFF000000);

    Circle circle {100.0, 400, 400, 0xFFFFFFFF};
    canvas->draw(circle)
        ->stretch(3, &new_pixels)
        ->save(fd, FileType::PPM);

    free_pixels(new_pixels, HEIGHT);

    fd.close();
    return 0;
}

int test_text(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    Text txt {"hello, world!", 150, 200, 10, 0xFF00FF00};

    canvas->fill(0xFF000000)
        ->draw(txt)
        ->save(fd, FileType::PPM);

    fd.close();
    return 0;
}

int test_triangle(const std::string &file_name) {
    std::ofstream fd {file_name, std::ios::out};
    if (!fd) return 1;

    Triangle t1 {100, 100, 600, 200, 400, 500, 0xFFFF00EE};

    canvas->fill(0xFF000000)
        ->draw(t1)
        ->save(fd, FileType::PPM);
        
    fd.close();
    return 0;
}

int main() {
    RUN_TEST(test_area, "./outputs/area.ppm");
    RUN_TEST(test_brightness, "./outputs/brightness.ppm");
    RUN_TEST(test_circle, "./outputs/circle.ppm");
    RUN_TEST(test_contrast, "./outputs/contrast.ppm");
    RUN_TEST(test_flip_rectangle, "./outputs/flipped_rectangle.ppm");
    RUN_TEST(test_flop_rectangle, "./outputs/flopped_rectangle.ppm");
    RUN_TEST(test_gray_scale, "./outputs/gray_scale.ppm");
    RUN_TEST(test_line, "./outputs/line.ppm");
    RUN_TEST(test_negate, "./outputs/negate.ppm");
    RUN_TEST(test_overlap, "./outputs/overlapped_cirles.ppm");
    RUN_TEST(test_roll, "./outputs/roll.ppm");
    RUN_TEST(test_rotate_rectangle, "./outputs/rotated_rectangle.ppm");
    RUN_TEST(test_saturation, "./outputs/saturation.ppm");
    RUN_TEST(test_sepia, "./outputs/sepia.ppm");
    RUN_TEST(test_text, "./outputs/text.ppm");
    RUN_TEST(test_triangle, "./outputs/triangle.ppm");
    
    RUN_TEST(test_stretch, "./outputs/stretch.ppm");

    delete canvas;
    return 0;
}