#pragma once

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <eikon/eikon.hpp>

#include "cmds.hpp"
#include "data.hpp"
#include "utils.hpp"

#define BLUE    "\033[34m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define RESET   "\033[0m"
#define YELLOW  "\033[33m"

extern const uint8_t VERBOSE;
extern const uint8_t SAVE_ON_ERROR;

using cmdsMap = std::map<std::string, std::pair<std::function<Error (std::vector<std::string>)>, uint>>;

bool cmp_flag(const std::string &flag, const std::string &short_form, const std::string &long_form);
void log(std::string flag, Error err);
std::vector<std::string> split(std::string s, std::string delimiter);

int parse_args(std::vector<std::string> argv);

void find_files(std::vector<std::string> &argv, std::string &in, std::string &out);
FileType get_filetype(const std::string& file_name);

void get_dimensions(const std::string &file, uint *height, uint *width);
void get_new_file_dimensions(std::vector<std::string> &argv, uint *height, uint *width);

std::string get_timestamp();

void help();