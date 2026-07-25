#pragma once

#include <filesystem>
#include <print>
#include <string>
#include <string_view>
#include <utility>

#include <eikon/eikon.hpp>
#include <eikon/utils.hpp>

#include "logs.hpp"

using test_function = std::function<eikon::Canvas ()>;

FORCE_INLINE constexpr bool maskedcmp(uint32_t a, uint32_t b) noexcept {
    return (a & 0x00FFFFFF) != (b &0x00FFFFFF);
}

static inline bool cmp_canvas(const eikon::Canvas &new_canvas, const eikon::Canvas &old_canvas) {
    auto && [new_height, new_width] = new_canvas.size();
    auto && [old_height, old_width] = old_canvas.size();

    if (old_height != new_height || old_width != new_width) [[unlikely]] {
        std::println("Different dimensions {} - {} | {} - {}", old_height, old_width, new_height, new_width);
        return false;
    }
    
    for (uint y = 0; y < new_height; y++) {
        for (uint x = 0; x < new_width; x++) {
            if (maskedcmp(new_canvas.at(x, y), old_canvas.at(x, y))) {
                std::println("Differences found at ({}, {})", x, y);
                std::println("{:#010X} != {:#010X}", old_canvas.at(y, x), new_canvas.at(y, x));
                return false;
            }
        }
    }
    
    return true;
}

static inline std::filesystem::path get_path(const std::string &function_name, const std::string &ext) noexcept {
    return std::filesystem::path("outputs") / ext / (function_name + "." + ext);
}

static inline void log(std::string_view function_name, bool success) noexcept {
    (success)
        ? logs::success_logs(function_name)
        : logs::failure_logs(function_name);
}

class TestEnv {
private:
    test_function func;

    inline bool compare_files(const std::string &file_name) const {
        eikon::Canvas old_canvas {file_name};
        eikon::Canvas new_canvas = func();
        
        new_canvas.save(file_name);
        return cmp_canvas(new_canvas, old_canvas);
    }
    
    inline bool new_file(const std::string &file_name) const {
        eikon::Canvas canvas = func();
        canvas.save(file_name);
        logs::newfile_logs(file_name);

        return true;
    }

public:
    TestEnv(test_function &func)
        : func(func) {}
    
    inline bool run_test(const std::string &function_name, const std::string &ext) const {
        const std::filesystem::path file_name = get_path(function_name, ext);

        if (std::filesystem::exists(file_name))
            return compare_files(file_name);
        
        return new_file(file_name);
    }
};

class Test {

private:
    std::map<std::string, TestEnv> tests;

public:
    bool register_test(const std::string name, test_function func) noexcept {
        if (tests.count(name))
            return false;

        tests.insert({name, TestEnv{func}});
        return true;
    }

    void run(const std::string &ext) {
        int failed = 0;
        bool success;
        
        for (auto && [function_name, env]: tests) {
            success = env.run_test(function_name, ext);
            log(function_name, success);
            failed += !success;
        }

        logs::final_log(tests.size(), failed);
    }
};
