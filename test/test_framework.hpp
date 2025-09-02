#pragma once

#include <string>
#include <utility>

#include <eikon/eikon.hpp>

#define GREEN   "\e[32m"
#define RED     "\e[31m"
#define RESET   "\e[0m"
#define YELLOW  "\e[33m"

std::map<std::string, std::function<int (const std::string &)>> tests;

bool cmp_pixels(uint32_t *p1, uint32_t *p2) {
    for (uint i = 0; i < HEIGHT * WIDTH; i++)
        if ((p1[i] & 0xFFFFFF) != (p2[i] & 0xFFFFFF))
            return false;

    return true;
}

const std::string get_path(const std::string &file_name, const std::string &ext) {
    return "./outputs/" + ext + "/" + file_name + "." + ext;
}

void read_old_image(
    const std::string &file_name, uint32_t *pixels, uint height, uint width
) {
    EikonCanvas *canvas = new EikonCanvas(pixels, height, width);
    canvas->read(file_name);
    delete canvas;
}

bool register_function(std::string name, std::function<int (const std::string &)> func) {
    if (tests.count(name))
        return false;

    tests.insert({name, func});
    return true;
}

bool run_test(
    std::function<int (const std::string &)> test_function,
    const std::string &test_function_name,
    const std::string &ext
) {
    const std::string file_name = get_path(test_function_name, ext);

    if (!std::filesystem::exists(file_name)) {
        test_function(file_name);
        std::cout << YELLOW << "[?]::[" << ext << "]::[ " << test_function_name << " file is new ]" << std::endl << RESET;
        return true;
    }

    uint32_t *tmp_pixels = new uint32_t[HEIGHT * WIDTH];
    read_old_image(file_name, tmp_pixels, HEIGHT, WIDTH);

    test_function(file_name);
    if (cmp_pixels(test::pixels, tmp_pixels))
        std::cout << GREEN << "[✔]::[" << ext << "]::[ " << test_function_name << " ]" << std::endl << RESET;
    else
        std::cout << RED << "[X]::[" << ext << "]::[ " << test_function_name << " different from old version ]" << std::endl << RESET;

    delete[] tmp_pixels;
    return true;
}