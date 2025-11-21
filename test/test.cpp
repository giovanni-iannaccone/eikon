#include <map>
#include <string>

#include "test_framework.hpp"
#include "test_functions.hpp"

void register_tests(Test& test) {
    std::map<const std::string, test_function> tests = {
        {"area", area},
        {"blur", blur},
        {"brightness", brightness},
        {"chop", chop},
        {"circle", circle},
        {"contrast", contrast},
        {"crop", crop},
        {"ellipse", ellipse},
        {"equalze", equalize},
        {"flip_rectangle", flip_rectangle},
        {"flop_rectangle", flop_rectangle},
        {"gray_scale", gray_scale},
        {"isolate", isolate},
        {"line", line},
        {"negate", negate},
        {"overlap", overlap},
        {"padding", padding},
        {"raise", raise_},
        {"roll", roll},
        {"rotate_rectangle", rotate_rectangle},
        {"saturation", saturation},
        {"sepia", sepia},
        {"solarize", solarize},
        {"stretch", stretch},
        {"text", text},
        {"triangle", triangle}
    };

    for (const auto &[name, func]: tests)
        test.register_test(name, func);
}

int main() {
    Test test;
    register_tests(test);

    std::cout << "========== [TEST BMP] ==========" << std::endl;
    test.run("bmp");

    std::cout << "\n\n========== [TEST PPM] ==========" << std::endl;
    test.run("ppm");

    std::cout << "\n\n========== [TEST PNG] ==========" << std::endl;
    test.run("png");

    std::cout << std::endl;
    return 0;
}
