#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <eikon/eikon.hpp>
#include "eikon_cli.hpp"

const uint8_t SAVE_ON_ERROR = 0b00000001;

static uint8_t flags = 0;

static EikonCanvas *canvas;
static uint height, width;

auto cmds = CmdsMap{{
    {"--ascii",       {ascii, 1}},
    {"--fill",        {fill, 1}},
    {"--flip",        {flip, 0}},
    {"--rotate",      {rotate, 0}},
    {"--stretch",     {stretch, 1}},
    {"--hue",         {hue, 1}},
    {"--saturation",  {saturation, 1}},
    {"--value",       {value, 1}},
    {"--circle",      {circle, 4}},
    {"--line",        {line, 5}},
    {"--rectangle",   {rectangle, 5}},
    {"--text",        {text, 5}},
    {"--triangle",    {triangle, 7}},
}};

std::map<std::string, std::function<void (void)>> generic_flags = {
    {"--save-on-error", [](){flags |= SAVE_ON_ERROR;}},
};

Error ascii(std::vector<std::string> args) {   
    canvas->ascii(ATOI_DEC(args[0]));
    return Error::FEW_ARGUMENTS;
}

Error circle(std::vector<std::string> args) {
    Circle circle {ATOI_DEC(args[0]), ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_HEX(args[3])}; 
    canvas->draw(circle);
    return Error::NO_ERROR;
}

bool cmp_flag(const std::string &flag, const std::string &short_form, const std::string &long_form) {
    return flag == short_form || flag == long_form;
}

Error fill(std::vector<std::string> args) {
    canvas->fill(ATOI_HEX(args[0]));
    return Error::NO_ERROR;
}

void find_files(std::vector<std::string> &argv, std::string &in, std::string &out) {
    for (auto it = argv.begin(); it != argv.end();)
        if (cmp_flag(*it, "-o", "--out")) {
            out = *(it + 1);

            argv.erase(it);
            argv.erase(it);

        } else if (cmp_flag(*it, "-i", "--in")) {
            in = *(it + 1);

            argv.erase(it);
            argv.erase(it);

        } else {
            it++;
        }
}

Error flip(std::vector<std::string>) {
    canvas->flip();
    return Error::NO_ERROR;
}

void get_dimensions(const std::string &file_name, uint *height, uint *width) {
    FileType ft = get_filetype(file_name);
    std::ifstream file {file_name, std::ios::in};

    switch (ft) {
        case BMP:
            bmp::get_dimensions(file, height, width);
            break;

        case PNG:
            png::get_dimensions(file, height, width);
            break;

        case PPM:
            ppm::get_dimensions(file, height, width);
            break;
    }

    file.close();
}

FileType get_filetype(const std::string& file_name) {
    std::vector<std::string> file { split(file_name, ".") };
    std::string ext {file[file.size() - 1]};

    if (ext == "bmp")
        return FileType::BMP;
    else if (ext == "png")
        return FileType::PNG;
    else
        return FileType::PPM;
}

void get_new_file_dimensions(std::vector<std::string> &argv, uint *height, uint *width) {
    
    for (auto it = argv.begin(); it != argv.end();) {
        if (cmp_flag(*it, "-s", "--size")) {
            argv.erase(it);

            *height = ATOI_DEC(*(it));
            argv.erase(it);

            *width  = ATOI_DEC(*(it));
            argv.erase(it);
            
            return;
        } else {
            it++;
        }
    }
}

std::string get_timestamp(time_t t) {
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y-%H-%M-%S");
    return oss.str();
}

void help() {
    std::cout << program_invocation_name << " usage:" << std::endl
        << "-h | --help     show this menu" << std::endl
        << "-i | --in       read image" << std::endl
        << "-o | --out      save output to a different image" << std::endl
        << "-s | --size     specify new file size (e.g., 800 800)" << std::endl << std::endl

        << "if -o is not specified, the image will be saved to -i" << std::endl
        << "if -i is not specified, the image will be created" << std::endl
        << "if none of them is specified, the new image will be saved to a file called by [timestamp].bmp" << std::endl
        << "if -i is not specified, give -s to set new image's dimensions (n m)" << std::endl << std::endl

        << "-------------------------------- [ GENERIC FLAGS] --------------------------------" << std::endl
        << "--save-on-error     if during flag parsing any error is found, save the file anyway" << std::endl << std::endl


        << "----------------------------------- [ SHAPES ] -----------------------------------" << std::endl
        << "--circle [r cx cy color]           draw a circle of radius r, center (cx; cy) and color color" << std::endl
        << "--ellipse [a b cx cy color]        draw an ellipse with axes a and b and center (cx; cy) of color color" << std::endl
        << "--line [x1 y1 x2 y2 color]         draw a line from (x1; y1) to (x2; y2) of color color" << std::endl
        << "--triangle [x1 y1 x2 y2 x3 y3 c]   draw a triangle with vertices in (x1; y1), (x2; y2), (x3; y3) of color c" << std::endl
        << "--text   [\"word\" x y fs c]       write \"word\" starting from (x; y) of color c with font size fs" << std::endl << std::endl

        << "----------------------------------- [ EFFECTS ] ----------------------------------" << std::endl
        << "--blur r            apply box blur with radius r" << std::endl
        << "--raise b           give the image 3d effect with raise of border b" << std::endl << std::endl

        << "-------------------------------- [ ENHANCEMENTS ] --------------------------------" << std::endl
        << "--brightness p      multiply image's brightness by p" << std::endl
        << "--contrast p        mutliply image's contrast by p" << std::endl
        << "--hue p             multiply hue by p" << std::endl
        << "--saturation p      multiply saturation by p" << std::endl
        << "--value p           multiply value by p" << std::endl
        << "--grayscale         convert image to grayscale" << std::endl
        << "--negate            obtain image's negative" << std::endl << std::endl

        << "------------------------------------- [ FX ] -------------------------------------" << std::endl
        << "--sepia             give the image a sepia effect" << std::endl
        << "--solarize p        solarize the image by p%" << std::endl << std::endl

        << "------------------------------- [ TRASFORMATIONS ] -------------------------------" << std::endl
        << "--flip                      flip the image" << std::endl
        << "--flop                      flop the image" << std::endl
        << "--padding t r b l c         pad the image on top (t), right (r), bottom (b), left (l) with color c" << std::endl
        << "--roll c                    move the image by c columns" << std::endl
        << "--rotate                    rotate the image" << std::endl
        << "--stretch s                 stretch the image by s" << std::endl;
}

Error hue(std::vector<std::string> args) {
    canvas->hue(ATOI_DEC(args[1]));
    return Error::NO_ERROR;
}

Error line(std::vector<std::string> args) {
    Line line {ATOI_DEC(args[0]), ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_HEX(args[4])};
    canvas->draw(line);
    return Error::NO_ERROR;
}

void log(std::string flag, Error err) {
    switch (err) {
        case Error::FEW_ARGUMENTS:
            std::cout << RED << "Too few arguments to flag " << flag << RESET << std::endl;
            break;
        
        case Error::GENERIC_ERROR:
            std::cout << RED << "Generic error occured in " << flag << RESET << std::endl;
            break;

        case Error::INVALID_DIMENSIONS:
            std::cout << RED << "Invalid dimenions " << flag << RESET << std::endl;
            break;

        case Error::UNKNOWN_FLAG:
            std::cout << RED << "Unknown flag: " << flag << RESET << std::endl;
            break;
        
        default:
            break;
    }
}

int parse_args(std::vector<std::string> argv) {
    uint failed = 0;
    Error err = Error::NO_ERROR;

    for (uint i = 0; i < argv.size(); i++)

        if (cmds.find(argv.at(i)) != cmds.end()) {
            auto [func, inc] = cmds[argv[i]];

            if (argv.size() - i < inc) {
                err = Error::FEW_ARGUMENTS;
            
            } else {
                std::vector<std::string> subvec(argv.begin() + i + 1, argv.begin() + i + inc + 1);
                err = func(subvec);
            }

            if (err != Error::NO_ERROR) {
                log(argv.at(i), err);
                failed++;
            }

            i += inc;
            
        } else if (generic_flags.find(argv.at(i)) != generic_flags.end()) {
            generic_flags[argv.at(i)]();

        } else {
            log(argv.at(i), Error::UNKNOWN_FLAG);
        }

    return failed;
}

Error rectangle(std::vector<std::string> args) {    
    Rectangle rec {ATOI_DEC(args[0]), ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_HEX(args[4])}; 
    canvas->draw(rec);
    return Error::NO_ERROR;
}

Error rotate(std::vector<std::string>) {
    return canvas->rotate() != nullptr
        ? Error::NO_ERROR
        : Error::GENERIC_ERROR;
}

Error saturation(std::vector<std::string> args) {
    canvas->saturation(ATOI_DEC(args[0]));
    return Error::NO_ERROR;
}

Error stretch(std::vector<std::string> args) {
    uint32_t **new_pixels;

    uint mul = ATOI_DEC(args[0]);
    canvas->stretch(mul, &new_pixels);

    free_pixels(new_pixels, height);

    width *= mul;
    return Error::NO_ERROR;
}

Error text(std::vector<std::string> args) {
    Text text {args[0], ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_HEX(args[4]), default_font}; 
    canvas->draw(text);
    return Error::NO_ERROR;
}

Error triangle(std::vector<std::string> args) {
    Triangle triangle {ATOI_DEC(args[0]), ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_DEC(args[4]), ATOI_DEC(args[5]), ATOI_HEX(args[6])}; 
    canvas->draw(triangle);
    return Error::NO_ERROR;
}

Error value(std::vector<std::string> args) {
    canvas->value(ATOI_DEC(args[0]));
    return Error::NO_ERROR;
}

int main(int argc, char *argv[]) {
    time_t t = std::time(nullptr);

    std::vector<std::string> arguments(argv + 1, argv + argc);
    if (cmp_flag(arguments.at(0), "-h", "--help")) {
        help();
        return 0;
    }

    std::string out {}, in {};
    find_files(arguments, in, out);

    if (in.empty()) {
        if (out.empty()) 
            out = get_timestamp(t) + ".bmp";
    
        get_new_file_dimensions(arguments, &height, &width);

        uint32_t **pixels = allocate_pixels(height, width);
        canvas = new EikonCanvas(pixels, height, width);

    } else {
        if (out.empty())
            out = in;

        canvas = new EikonCanvas(in, nullptr, &height, &width);
    }
    
    if (height == 0 || width == 0) {
        log(std::to_string(height) + "x" + std::to_string(width), Error::INVALID_DIMENSIONS);
        return 1;
    }

    if (parse_args(arguments) == 0 || (flags & SAVE_ON_ERROR))
        canvas->save(out);

    canvas->free_all();
    return 0;
}
