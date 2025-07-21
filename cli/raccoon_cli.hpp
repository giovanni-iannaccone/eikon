#pragma once

#include <bits/stdc++.h>
#include <fstream>
#include <string>
#include <vector>

#include "./include/raccoon.hpp"

#define ATOI_DEC(str) std::stoul(str, nullptr, 10)
#define ATOI_HEX(str) std::stoul(str, nullptr, 16)

#define BLUE    "\033[34m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define RESET   "\033[0m"
#define YELLOW  "\033[33m"

int ascii(std::vector<std::string> args);
int fill(std::vector<std::string> args);
int flip(std::vector<std::string>);
int rotate(std::vector<std::string>);
int stretch(std::vector<std::string> args);

int circle(std::vector<std::string> args);
int line(std::vector<std::string> args);
int rectangle(std::vector<std::string> args);
int text(std::vector<std::string> args);
int triangle(std::vector<std::string> args);

int hue(std::vector<std::string> args);
int saturation(std::vector<std::string> args);
int value(std::vector<std::string> args);

filetype get_filetype(const std::string& file_name);
const std::string& get_filetype_name(filetype ft);

void ask_to_save(std::ofstream &file, unsigned int unsaved_changes);
void get_dimensions(std::ifstream& file, size_t *height, size_t *width, filetype ft);
int read_and_show_file_dimensions(std::string file_name);

int parse_cmd(std::string cmd, std::string file_name, unsigned int *unsaved_changes);
void run(std::string file_name);

static inline void show_banner();
static inline void show_help();

std::vector<std::string> split(std::string s, std::string delimiter);