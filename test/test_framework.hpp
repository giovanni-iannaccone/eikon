#pragma once

#include <string>
#include <utility>

#include <eikon/eikon.hpp>

#include "./logs.hpp"

#define HEIGHT 800
#define WIDTH  800

typedef std::function<int (EikonCanvas*, const std::string &)> test_function;
typedef std::function<uint32_t** (EikonCanvas*, const std::string &)> test_isolated;

enum Resource {
    INITIALIZE,
    DONT_INITIALIZE
};

bool cmp_pixels(uint32_t *p1, uint32_t *p2) {
    for (uint i = 0; i < HEIGHT * WIDTH; i++)
        if ((p1[i] & 0xFFFFFF) != (p2[i] & 0xFFFFFF))
            return false;

    return true;
}

std::filesystem::path get_path(const std::string& file_name, const std::string& ext) {
    return std::filesystem::path("outputs") / ext / (file_name + "." + ext);
}

void log(const std::string &test_name, const std::string &ext, bool success) {
    if (success)
        logs::failure_logs(test_name, ext);
    else
        logs::success_logs(test_name, ext);
}

void read_old_image(
    const std::string &file_name, uint32_t *pixels, uint height, uint width
) {
    EikonCanvas canvas {pixels, height, width};
    canvas.read(file_name);
}

class IsolatedEnv {

private:
    uint height;
    uint width;
    test_isolated func;

public:
    IsolatedEnv(uint height, uint width, test_isolated func)
    : height(height), width(width), func(func) {};

    bool cmp_pixels(uint32_t **p1, uint32_t *p2) {
        for (uint y = 0; y < height; y++)
            for (uint x = 0; x < width; x++)
                if ((p1[y][x] & 0xFFFFFF) != (p2[y*width + x] & 0xFFFFFF))
                    return false;

        return true;
    }

    bool run_test(const std::string& test_function_name, const std::string& ext) {
        
        auto file_name = get_path(test_function_name, ext);

        auto pixels = std::make_unique<uint32_t[]>(HEIGHT * WIDTH);
        auto canvas = std::make_unique<EikonCanvas>(pixels.get(), HEIGHT, WIDTH);

        uint32_t** result_pixels = nullptr;

        if (!std::filesystem::exists(file_name)) {
            result_pixels = func(canvas.get(), file_name);
            logs::newfile_logs(test_function_name, ext);

            free_pixels(result_pixels, height);
            return true;
        }

        auto tmp_pixels = std::make_unique<uint32_t[]>(height * width);
        read_old_image(file_name.string(), tmp_pixels.get(), height, width);

        result_pixels = func(canvas.get(), file_name.string());
        bool success = cmp_pixels(result_pixels, tmp_pixels.get());

        free_pixels(result_pixels, height);
        return success;
    }

};

class TestEnv {

private:
    uint height;
    uint width;
    test_function func;

public:

    TestEnv(uint height, uint width, test_function func)
    : height(height), width(width), func(func) {}

    bool run_test(const std::string& test_function_name, const std::string& ext) {

        auto file_name = get_path(test_function_name, ext);

        auto pixels = std::make_unique<uint32_t[]>(height * width);
        auto canvas = std::make_unique<EikonCanvas>(pixels.get(), height, width);

        if (!std::filesystem::exists(file_name)) {
            func(canvas.get(), file_name);
            logs::newfile_logs(test_function_name, ext);

            return true;
        }

        auto tmp_pixels = std::make_unique<uint32_t[]>(height * width);
        read_old_image(file_name, tmp_pixels.get(), height, width);

        func(canvas.get(), file_name.string());
        return cmp_pixels(pixels.get(), tmp_pixels.get());
    }
};

class Test {

private:
    std::map<std::string, TestEnv> tests;
    std::map<std::string, IsolatedEnv> isolated;

    std::unique_ptr<uint32_t[]> pixels;
    std::unique_ptr<EikonCanvas> canvas;

public:
    
    void init_resources() {
        pixels = std::make_unique<uint32_t[]>(HEIGHT * WIDTH);
        canvas = std::make_unique<EikonCanvas>(pixels.get(), HEIGHT, WIDTH);
    }

    bool register_isolated(std::string name, IsolatedEnv env) {
        if (isolated.count(name))
            return false;

        isolated.insert({name, env});
        return true;
    }

    bool register_test(std::string name, test_function func) {
        if (tests.count(name))
            return false;

        tests.insert({name, TestEnv{HEIGHT, WIDTH, func}});
        return true;
    }

    void run(const std::string &ext, const Resource resources_opt = Resource::DONT_INITIALIZE) {
        int failed = 0;
        bool success {};

        if (resources_opt == Resource::INITIALIZE)
            init_resources();

        for (auto &[name, env]: tests) {
            success = !env.run_test(name, ext);
            log(name, ext, success);
            failed += success;
        }

        logs::info_logs("\nIsolated env tests:");
        for (auto &[name, env]: isolated) {
            success = !env.run_test(name, ext);
            log(name, ext, success);
            failed += success;
        }
        
        if (failed == 0)
            logs::log(logs::Type::SUCCESS, "[+] No test failed");
        else 
            logs::log(logs::Type::FAILURE, "[-] {} tests failed", failed);
    }
};
