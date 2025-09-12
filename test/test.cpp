#include <map>
#include <string>
#include <utility>

#include "test_framework.hpp"
#include "test_functions.hpp"

void register_isolated(Test& test) {
    std::map<const std::string, IsolatedEnv> tests = {
        {"stretch", IsolatedEnv{800, 1600, stretch}}
    };

    for (const auto &[name, env]: tests)
        test.register_isolated(name, env);
}

void register_tests(Test& test) {
    std::map<const std::string, test_function> tests = {
        {"area", area},
        {"blur", blur},
        {"brightness", brightness},
        {"circle", circle},
        {"contrast", contrast},
        {"ellipse", ellipse},
        {"equalze", equalize},
        {"flip_rectangle", flip_rectangle},
        {"flop_rectangle", flop_rectangle},
        {"gray_scale", gray_scale},
        {"line", line},
        {"negate", negate},
        {"overlap", overlap},
        {"raise", raise_},
        {"roll", roll},
        {"rotate_rectangle", rotate_rectangle},
        {"saturation", saturation},
        {"sepia", sepia},
        {"text", text},
        {"triangle", triangle}
    };

    for (const auto &[name, func]: tests)
        test.register_test(name, func);
}

int main() {
    Test test;
    register_tests(test);
    register_isolated(test);
    
    std::cout << "========== [TEST PPM] ==========" << std::endl;
    test.run("ppm", Resource::INITIALIZE);

    std::cout << "\n\n========== [TEST BMP] ==========" << std::endl;
    test.run("bmp", Resource::INITIALIZE);

    return 0;
}