#pragma once

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <eikon/eikon.hpp>

#define ATOI_DEC(str) uint(std::stoul(str, nullptr, 10))
#define ATOI_HEX(str) uint(std::stoul(str, nullptr, 16))

#define BLUE    "\033[34m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define RESET   "\033[0m"
#define YELLOW  "\033[33m"

enum Error {
    FEW_ARGUMENTS,
    GENERIC_ERROR,
    INVALID_DIMENSIONS,
    NO_ERROR,
    UNKNOWN_FLAG
};

using CmdsMap = std::map<std::string, std::pair<std::function<Error (std::vector<std::string>)>, uint>>;

bool cmp_flag(const std::string &flag, const std::string &short_form, const std::string &long_form);
void log(std::string flag, Error err);
std::vector<std::string> split(std::string s, std::string delimiter);

int parse_args(std::vector<std::string> argv);

void find_files(std::vector<std::string> &argv, std::string &in, std::string &out);
FileType get_filetype(const std::string& file_name);

void get_dimensions(const std::string &file, uint *height, uint *width);
void get_new_file_dimensions(std::vector<std::string> &argv, uint *height, uint *width);

std::string get_timestamp(time_t t);

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

void help();