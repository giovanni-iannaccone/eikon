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

    void write_logs(logs::Type lt, const std::string &msg) {
        switch (lt) {
            case logs::Type::FAILURE:
                std::cout << RED << msg << RESET << std::endl;
                break;

            case logs::Type::INFO:
                std::cout << msg << std::endl;
                break;

            case logs::Type::NEW_FILE:
                std::cout << YELLOW << msg << RESET << std::endl;
                break;

            case logs::Type::SUCCESS:
                std::cout << GREEN << msg << RESET << std::endl;
                break;
        }
    }

    void failure_logs(const std::string &function_name, const std::string &ext) {
        write_logs(logs::Type::FAILURE, "[X]::[" + ext + "]::[ " + function_name + " different from old version ]");
    }

    void info_logs(const std::string& msg) {
        write_logs(logs::Type::INFO, msg);
    }

    void newfile_logs(const std::string &function_name, const std::string &ext) {
        write_logs(logs::Type::NEW_FILE, "[?]::[" + ext + "]::[ " + function_name + " file is new ]");
    }

    void success_logs(const std::string &function_name, const std::string &ext) {
        write_logs(logs::Type::SUCCESS, "[✔]::[" + ext + "]::[ " + function_name + " ]");
    }
}