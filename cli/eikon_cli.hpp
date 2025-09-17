#pragma once

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <eikon/eikon.hpp>

#define ATOI_DEC(str) std::stoul(str, nullptr, 10)
#define ATOI_HEX(str) std::stoul(str, nullptr, 16)

#define BLUE    "\033[34m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define RESET   "\033[0m"
#define YELLOW  "\033[33m"

enum Error {
    FEW_ARGUMENTS,
    GENERIC_ERROR,
    NO_ERROR,
    UNKNOWN_FLAG
};

Error ascii(std::vector<std::string> args);
Error fill(std::vector<std::string> args);
Error flip(std::vector<std::string>);
Error rotate(std::vector<std::string>);
Error stretch(std::vector<std::string> args);

Error circle(std::vector<std::string> args);
Error line(std::vector<std::string> args);
Error rectangle(std::vector<std::string> args);
Error text(std::vector<std::string> args);
Error triangle(std::vector<std::string> args);

Error hue(std::vector<std::string> args);
Error saturation(std::vector<std::string> args);
Error value(std::vector<std::string> args);

void get_dimensions(const std::string &file, uint *height, uint *width);
FileType get_filetype(const std::string& file_name);

void help();

std::vector<std::string> split(std::string s, std::string delimiter);