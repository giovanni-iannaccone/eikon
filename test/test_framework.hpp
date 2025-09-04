#pragma once

#include <string>
#include <utility>

#include <eikon/eikon.hpp>

#define GREEN   "\e[32m"
#define RED     "\e[31m"
#define RESET   "\e[0m"
#define YELLOW  "\e[33m"

#define HEIGHT 800
#define WIDTH  800

typedef std::function<int (EikonCanvas*, const std::string &)> test_function;
typedef std::function<uint32_t** (EikonCanvas*, const std::string &)> test_isolated;

enum Resource {
    INITIALIZE,
    DONT_INITIALIZE
};

typedef struct {
    uint height;
    uint width;
    test_isolated func;
} isolated_env; 

bool cmp_isolated_pixels(uint32_t **p1, uint32_t *p2, uint height, uint width) {
    for (uint y = 0; y < height; y++)
        for (uint x = 0; x < width; x++)
            if ((p1[y][x] & 0xFFFFFF) != (p2[y*width + x] & 0xFFFFFF))
                return false;

    return true;
}

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

class Test {

private:
    std::map<std::string, test_function> tests;
    std::map<std::string, isolated_env> isolated;

    uint32_t *pixels;
    EikonCanvas *canvas;

protected:
    static Test *instance;

    Test()
    : pixels(nullptr),
    canvas(nullptr) {};

public:
    Test(Test &other) = delete;
    void operator=(const Test &) = delete;

    static Test *get_instance() {
        if(instance == nullptr)
            instance = new Test();
    
        return instance;
    }

    void init_resources() {
        if (pixels != nullptr)
            delete[] pixels;

        if (canvas != nullptr)
            delete canvas;

        pixels = new uint32_t[HEIGHT * WIDTH];
        canvas = new EikonCanvas(pixels, HEIGHT, WIDTH);
    }

    bool register_isolated(std::string name, isolated_env env) {
        if (isolated.count(name))
            return false;

        isolated.insert({name, env});
        return true;
    }

    bool register_test(std::string name, test_function func) {
        if (tests.count(name))
            return false;

        tests.insert({name, func});
        return true;
    }

    void run(const std::string &ext, const Resource resources_opt = Resource::DONT_INITIALIZE) {
        int failed = 0;

        if (resources_opt == Resource::INITIALIZE)
            init_resources();

        for (const auto &[name, func]: tests)
            failed += !run_test(func, name, ext);

        for (const auto &[name, env]: isolated)
            failed += !run_isolated_test(env, name, ext);
        
        if (failed == 0)
            std::cout << GREEN << "[+] No test failed" << std::endl << RESET;
        else 
            std::cout << RED << "[-] " << failed << " tests failed" << std::endl << RESET;
    }

    bool run_isolated_test(
        isolated_env env,
        const std::string &test_function_name,
        const std::string &ext
    ) {
        const std::string file_name = get_path(test_function_name, ext);

        if (!std::filesystem::exists(file_name)) {
            env.func(canvas, file_name);
            std::cout << YELLOW << "[?]::[" << ext << "]::[ " << test_function_name << " file is new ]" << std::endl << RESET;
            return true;
        }

        uint32_t *tmp_pixels = new uint32_t[env.height * env.width];
        read_old_image(file_name, tmp_pixels, env.height, env.width);

        uint32_t *isolated_pixels = new uint32_t[HEIGHT * WIDTH];
        EikonCanvas *isolated_canvas = new EikonCanvas(isolated_pixels, HEIGHT, WIDTH);

        uint32_t **result_pixels = env.func(isolated_canvas, file_name);
        if (cmp_isolated_pixels(result_pixels, tmp_pixels, env.height, env.width))
            std::cout << GREEN << "[✔]::[" << ext << "]::[ " << test_function_name << " ]" << std::endl << RESET;
        else
            std::cout << RED << "[X]::[" << ext << "]::[ " << test_function_name << " different from old version ]" << std::endl << RESET;

        delete[] isolated_pixels;
        delete[] tmp_pixels;

        delete isolated_canvas;
        return true;
    }

    bool run_test(
        test_function func,
        const std::string &test_function_name,
        const std::string &ext
    ) {
        const std::string file_name = get_path(test_function_name, ext);

        if (!std::filesystem::exists(file_name)) {
            func(canvas, file_name);
            std::cout << YELLOW << "[?]::[" << ext << "]::[ " << test_function_name << " file is new ]" << std::endl << RESET;
            return true;
        }

        uint32_t *tmp_pixels = new uint32_t[HEIGHT * WIDTH];
        read_old_image(file_name, tmp_pixels, HEIGHT, WIDTH);

        func(canvas, file_name);
        if (cmp_pixels(pixels, tmp_pixels))
            std::cout << GREEN << "[✔]::[" << ext << "]::[ " << test_function_name << " ]" << std::endl << RESET;
        else
            std::cout << RED << "[X]::[" << ext << "]::[ " << test_function_name << " different from old version ]" << std::endl << RESET;

        delete[] tmp_pixels;
        return true;
    }
};

Test *Test::instance = nullptr;