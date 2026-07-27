#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

#include <eikon/eikon.hpp>

#include "cmds.hpp"

const uint8_t VERBOSE       = 0b00000001;
const uint8_t SAVE_ON_ERROR = 0b00000010;

const std::string RED_TEXT      = "\033[31m";
const std::string RESET_TEXT    = "\033[0m";

using uint = unsigned int;
using cmdsMap = std::unordered_map<std::string, std::pair<std::function<Error (eikon::Canvas &, std::vector<std::string>&)>, uint>>;

bool cmp_flag(const std::string &flag, const std::string &short_form, const std::string &long_form);
void log(std::string flag, Error err);

int parse_args(std::vector<std::string> argv);

void find_files(std::vector<std::string> &argv, std::string &in, std::string &out);
eikon::files::Type get_filetype(const std::string& file_name);
void get_new_file_dimensions(std::vector<std::string> &argv, uint &height, uint &width);

std::string get_timestamp();

[[noreturn]] void help();
