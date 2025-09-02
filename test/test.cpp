#include <map>
#include <string>
#include <utility>

#include "test_functions.hpp"
#include "test_framework.hpp"

void init_resources() {
    if (test::pixels != nullptr)
        delete[] test::pixels;

    if (test::canvas != nullptr)
        delete test::canvas;

    test::pixels = new uint32_t[HEIGHT * WIDTH];
    test::canvas = new EikonCanvas(test::pixels, HEIGHT, WIDTH);
}

void bmp_run_tests() {
    init_resources();

    for (const auto &[name, func]: tests)
        run_test(func, name, "bmp");
}

void ppm_run_tests() {
    init_resources();

    for (const auto &[name, func]: tests)
        run_test(func, name, "ppm");
}

void register_tests() {
    register_function("area", test::area);
    register_function("brightness", test::brightness);
    register_function("circle", test::circle);
    register_function("contrast", test::contrast);
    register_function("flip_rectangle", test::flip_rectangle);
    register_function("flop_rectangle", test::flop_rectangle);
    register_function("gray_scale", test::gray_scale);
    register_function("line", test::line);
    register_function("negate", test::negate);
    register_function("overlap", test::overlap);
    register_function("roll", test::roll);
    register_function("rotate_rectangle", test::rotate_rectangle);
    register_function("saturation", test::saturation);
    register_function("sepia", test::sepia);
    register_function("text", test::text);
    register_function("triangle", test::triangle);
}

int main() {
    register_tests();

    std::cout << GREEN << "---------- [TEST PPM] ----------" << std::endl << RESET;
    ppm_run_tests();

    std::cout << GREEN << "\n\n---------- [TEST BMP] ----------" << std::endl << RESET;
    bmp_run_tests();

    return 0;
}