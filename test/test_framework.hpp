#pragma once

#include <string>
#include <utility>

#include <eikon/eikon.hpp>

#include "logs.hpp"

const uint HEIGHT = 800;
const uint WIDTH  = 800;

typedef std::function<int (EikonCanvas&, const std::string &)> test_function;

inline uint32_t mask(uint32_t pixel) {
    return pixel & 0xFFFFFF;
}

inline bool cmp_canvas(EikonCanvas& new_canvas, EikonCanvas& old_canvas) {
    auto [height, width] = new_canvas.size();
    
    for (uint y = 0; y < height; y++)
        for (uint x = 0; x < width; x++)
            if (mask(new_canvas.at(x, y)) != mask(old_canvas.at(x, y)))
                return false;

    return true;
}

inline std::filesystem::path get_path(const std::string& file_name, const std::string& ext) {
    return std::filesystem::path("outputs") / ext / (file_name + "." + ext);
}

inline void log(const std::string &test_name, const std::string &ext, bool success) {
    if (success)
        logs::failure_logs(test_name, ext);
    else
        logs::success_logs(test_name, ext);
}

class TestEnv {

private:
    test_function func;

public:

    TestEnv(test_function func)
    : func(func) {}

    bool run_test(const std::string& test_function_name, const std::string& ext) {

        auto file_name = get_path(test_function_name, ext);
        EikonCanvas new_canvas {HEIGHT, WIDTH};

        if (!std::filesystem::exists(file_name)) {
            func(new_canvas, file_name);
            logs::newfile_logs(test_function_name, ext);

            return true;
        }

        EikonCanvas old_canvas {file_name};

        func(new_canvas, file_name.string());
        return cmp_canvas(new_canvas, old_canvas);
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
        
        for (auto &[name, env]: tests) {
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
