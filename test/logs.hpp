#pragma once

#include <format>
#include <iostream>
#include <string_view>

namespace logs {
    
    using logType = std::string_view;

    namespace Type {
        constexpr std::string FAILURE  = "\033[31m";
        constexpr std::string INFO     = "\033[0m" ;
        constexpr std::string NEW_FILE = "\033[33m";
        constexpr std::string SUCCESS  = "\033[32m";
    }

    template<typename ...Args>
    static constexpr void log(logType lt, const std::format_string<Args...> fmt, Args&&... args) noexcept {
        std::cout << lt << std::vformat(fmt.get(), std::make_format_args(args...)) << logs::Type::INFO;
    }

    constexpr void failure_logs(std::string_view function_name) noexcept {
        logs::log(logs::Type::FAILURE, "{} differs from old version\n", function_name);
    }

    constexpr void final_log(int tests, int failed) noexcept {
        std::cout << logs::Type::SUCCESS << tests - failed << logs::Type::INFO << " tests succeeded, "
                  << logs::Type::FAILURE << failed << logs::Type::INFO << " tests failed\n" << logs::Type::INFO;
    }
    
    constexpr void info_logs(std::string_view msg) noexcept {
        logs::log(logs::Type::INFO, "{}\n", msg);
    }

    constexpr void newfile_logs(std::string_view function_name) noexcept {
        logs::log(logs::Type::NEW_FILE, "{} file is new\n", function_name);
    }

    constexpr void success_logs(std::string_view function_name) noexcept {
        logs::log(logs::Type::SUCCESS, "{}... PASSED\n", function_name);
    }
}
