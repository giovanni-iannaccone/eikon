#pragma once

#include <filesystem>
#include <print>
#include <string>
#include <string_view>

#include <eikon/eikon.hpp>

#include "logs.hpp"
#include "utils.hpp"

using test_function = eikon::Canvas (*)();

struct PixelDiff
{
    uint x = 0;
    uint y = 0;
    uint32_t actual = 0;
    uint32_t expected = 0;

    constexpr operator bool() const noexcept
    {
        return x || y || actual || expected;
    }
};

static inline
PixelDiff cmp_pixels(
    const eikon::Canvas &actual, const eikon::Canvas &expected
) noexcept
{
    const auto [height, width] = actual.size();

    for (uint y = 0; y < height; y++) {
        for (uint x = 0; x < width; x++) {
            const auto a = actual.at(x, y);
            const auto e = expected.at(x, y);

            if (maskedcmp(a, e))
                return PixelDiff{x, y, a, e};
        }
    }

    return {};
}

static inline
bool cmp_canvas(
    const eikon::Canvas &actual, const eikon::Canvas &expected
) noexcept
{
    if (actual.size() != expected.size()) [[unlikely]] {
        std::println("Different dimensions {} - {} | {} - {}",
            actual.height(), actual.width(), expected.height(), expected.width()
        );

        return false;
    }
    
    if (auto diff = cmp_pixels(actual, expected)) {
        std::println("Difference found at ({}, {})", diff.x, diff.y);
        std::println("Expected: {:#010X}\nActual:   {:#010X}", diff.expected, diff.actual);
        return false;
    }
    
    return true;
}

[[nodiscard]] static inline
const std::filesystem::path get_path(std::string_view function_name, std::string_view ext) noexcept
{
    return std::filesystem::path("outputs") /
        std::string(ext) / (std::string(function_name) + "." + std::string(ext));
}

static inline
void log(std::string_view function_name, bool success) noexcept
{
    (success)
        ? logs::success_logs(function_name)
        : logs::failure_logs(function_name);
}

template <FixedString Name, auto Function>
struct Test
{
    static constexpr auto name = Name;
    static constexpr auto function = Function;

    static bool run(std::string_view ext)
    {
        auto path = get_path(Name.view(), ext);

        if (!std::filesystem::exists(path)) {
            auto canvas = Function();
            canvas.save(path);
            logs::newfile_logs(path.string());
            return true;
        }

        eikon::Canvas expected {path};
        eikon::Canvas actual = Function();
        
        bool ok = cmp_canvas(actual, expected);
        log(Name.view(), ok);

        actual.save(path);
        return ok;
    }
};

template <typename... Tests>
struct TestSuite
{
    static void run(std::string_view ext)
    {
        uint failed = 0;
        ((failed += !Tests::run(ext)), ... );

        logs::final_log(sizeof...(Tests), failed);
    }
};

void run_tests(const std::string &ext);
