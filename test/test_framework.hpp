#pragma once

#include <string>
#include <utility>

#include <eikon/eikon.hpp>

#include "logs.hpp"

#define DEBUG

const uint HEIGHT = 800;
const uint WIDTH  = 800;

typedef std::function<int (EikonCanvas&, const std::string &)> test_function;

inline uint32_t mask(uint32_t pixel) {
    return pixel & 0x00FFFFFF;
}

inline bool cmp_canvas(const EikonCanvas &new_canvas, const EikonCanvas &old_canvas) {
    auto [new_height, new_width] = new_canvas.size();
    auto [old_height, old_width] = old_canvas.size();

    if (old_height != new_height || old_width != new_width)
        return false;
    
    for (uint y = 0; y < new_height; y++)
        for (uint x = 0; x < new_width; x++)
            if (mask(new_canvas.at(x, y)) != mask(old_canvas.at(x, y))) {
#ifdef DEBUG
                std::cout << "Differences found at (" << x << "; " << y << ")" << std::endl;
                std::cout << std::hex << new_canvas.at(x, y) << " != " << old_canvas.at(x, y) << std::dec << std::endl;
#endif
                return false;
            }
    
    return true;
}

inline std::filesystem::path get_path(const std::string& function_name, const std::string& ext) {
    return std::filesystem::path("outputs") / ext / (function_name + "." + ext);
}

inline void log(const std::string &function_name, const std::string &ext, bool success) {
    if (success)
        logs::failure_logs(function_name, ext);
    else
        logs::success_logs(function_name, ext);
}

class TestEnv {
private:
    test_function func;

    bool compare_files(EikonCanvas &new_canvas, const std::string &file_name) {
        EikonCanvas old_canvas {file_name};

        func(new_canvas, file_name);
        return cmp_canvas(new_canvas, old_canvas);
    }
    
    bool new_file(EikonCanvas &canvas, const std::string &file_name, const std::string &ext) {
        func(canvas, file_name);
        logs::newfile_logs(file_name, ext);

        return true;
    }

public:
    TestEnv(test_function &func)
        : func(func) {}
    
    bool run_test(const std::string& function_name, const std::string& ext) {

        const std::filesystem::path file_name = get_path(function_name, ext);
        EikonCanvas new_canvas {HEIGHT, WIDTH};

        if (std::filesystem::exists(file_name))
            return compare_files(new_canvas, file_name);
        else
            return new_file(new_canvas, file_name, ext);
    }
};

class Test {
private:
    std::map<std::string, TestEnv> tests;

public:
    bool register_test(std::string name, test_function func) {
        if (tests.count(name))
            return false;

        tests.insert({name, TestEnv{func}});
        return true;
    }

    void run(const std::string &ext) {
        int failed = 0;
        bool success {};
        
        for (auto &[function_name, env]: tests) {
            success = !env.run_test(function_name, ext);
            log(function_name, ext, success);
            failed += success;
        }

        logs::final_log(tests.size(), failed);
    }
};
