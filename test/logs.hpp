#include <format>
#include <iostream>

#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define RESET   "\033[0m"
#define YELLOW  "\033[33m"

namespace logs {
    enum Type {
        FAILURE,
        INFO,
        NEW_FILE,
        SUCCESS
    };

    template<typename... Args>
    void log(logs::Type lt, const std::format_string<Args...> fmt, Args&&... args) {
        std::string color {};

        switch (lt) {
            case logs::Type::FAILURE:
                color = RED;
                break;

            case logs::Type::INFO:
                color = RESET;
                break;

            case logs::Type::NEW_FILE:
                color = YELLOW;
                break;

            case logs::Type::SUCCESS:
                color = GREEN;
                break;
        }

        std::cout << color << std::vformat(fmt.get(), std::make_format_args(args...)) << RESET;
    }

    inline void failure_logs(const std::string &function_name, const std::string &ext) {
        logs::log(logs::Type::FAILURE, "[X {}]  {} differs from old version\n", ext, function_name);
    }

    inline void final_log(int tests, int failed) {
        std::cout << GREEN << tests - failed << RESET << " tests succeeded, " << RED << failed << RESET << " tests failed\n" << RESET;
    }
    
    inline void info_logs(const std::string& msg) {
        logs::log(logs::Type::INFO, "{}\n", msg);
    }

    inline void newfile_logs(const std::string &function_name, const std::string &ext) {
        logs::log(logs::Type::NEW_FILE, "[? {}] {} file is new\n", ext, function_name);
    }

    inline void success_logs(const std::string &function_name, const std::string &ext) {
        logs::log(logs::Type::SUCCESS, "[✔ {}]  {}...PASSED\n", ext, function_name);
    }
}
