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
static uint32_t *pixels;
static uint height, width;

std::map<std::string, std::function<Error (std::vector<std::string>)>> cmds = {
    {"ascii",       ascii},
    {"fill",        fill},
    {"flip",        flip},
    {"rotate",      rotate},
    {"stretch",     stretch},
    {"hue",         hue},
    {"saturation",  saturation},
    {"value",       value},
    {"circle",      circle},
    {"line",        line},
    {"rectangle",   rectangle},
    {"text",        text},
    {"triangle",    triangle},
};

std::map<std::string, std::function<void (void)>> flags = {
    {"--save-on-error", [](){flags |= SAVE_ON_ERROR;}},
};

Error ascii(std::vector<std::string> args) {
    if (args.size() < 2)
        return Error::FEW_ARGUMENTS;
    
    canvas->ascii(ATOI_DEC(args[1]));
    return Error::FEW_ARGUMENTS;
}

Error circle(std::vector<std::string> args) {
    if (args.size() < 5)
        return Error::FEW_ARGUMENTS;

    Circle circle {ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_HEX(args[4])}; 
    canvas->draw(circle);
    return Error::NO_ERROR;
}

bool cmp_flag(const std::string &flag, const std::string &short_form, const std::string &long_form) {
    return flag == short_form || flag == long_form;
}

Error fill(std::vector<std::string> args) {
    if (args.size() < 2)
        return Error::NO_ERROR;

    canvas->fill(ATOI_HEX(args[1]));
    return Error::NO_ERROR;
}

void find_files(std::vector<std::string> &args, std::string &in, std::string &out) {
    bool found_out = false;
    bool found_in  = false;

    for (size_t i = 0; i < args.size(); i++)
        if (cmp_flag(args[i], "-o", "--out")) {
            out = args.at(i + 1);

            args.erase(args.begin() + i);
            args.erase(args.begin() + i);

            found_out = true;

        } else if (cmp_flag(args[i], "-i", "--in")) {
            in = args.at(i + 1);

            args.erase(args.begin() + i);
            args.erase(args.begin() + i);

            found_in = true;
        }
    
    if (!found_out && found_in) {
        out = in;
        found_out = true;
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

void get_new_file_dimensions(std::vector<std::string> argv, uint *height, uint *width) {
    
    for (size_t i = 0; i < argv.size(); i++)
        if (cmp_flag(argv.at(i), "-s", "--size")) {
            *height = ATOI_DEC(argv.at(i + 1));
            *width  = ATOI_DEC(argv.at(i + 2));

            argv.erase(argv.begin());
            argv.erase(argv.begin());
            argv.erase(argv.begin());
        }
}

void help() {
    std::cout << program_invocation_name << " usage:" << std::endl
        << program_invocation_name << " input_file [flags]" << std::endl
        << "-h | --help     show this menu" << std::endl
        << "-i | --in       read image" << std::endl
        << "-o | --out      save output to a different image" << std::endl
        << "-s | --size     specify new file size (e.g., 800 800)" << std::endl

        << "if -o is not specified, the image will be saved to -i" << std::endl
        << "if -i is not specified, the image will be created" << std::endl
        << "if none of them is specified, the new image will be saved to a file called by [timestamp].bmp" << std::endl
        << "if -i is not specified, give -s to set new image's dimensions (n m)"
        << "-------------------------------- [ GENERIC FLAGS] --------------------------------" << std::endl
        << "--save-on-error     if during flag parsing any error is found, save the file anyway" << std::endl
        << "----------------------------------- [ SHAPES ] -----------------------------------" << std::endl
        << "-c | --circle [r cx cy color]           draw a circle of radius r, center (cx; cy) and color color" << std::endl
        << "-e | --ellipse [a b cx cy color]        draw an ellipse with axes a and b and center (cx; cy) of color color" << std::endl
        << "-l | --line [x1 y1 x2 y2 color]         draw a line from (x1; y1) to (x2; y2) of color color" << std::endl
        << "-t | --triangle [x1 y1 x2 y2 x3 y3 c]   draw a triangle with vertices in (x1; y1), (x2; y2), (x3; y3) of color c" << std::endl
        << "-x | --text   [\"word\" x y fs]         write \"word\" starting from (x; y) with font size fs" << std::endl
        << "----------------------------------- [ EFFECTS ] ----------------------------------" << std::endl
        << "--blur r            apply box blur with radius r" << std::endl
        << "--raise b           give the image 3d effect with raise of border b" << std::endl
        << "-------------------------------- [ ENHANCEMENTS ] --------------------------------" << std::endl
        << "--brightness p      multiply image's brightness by p" << std::endl
        << "--contrast p        mutliply image's contrast by p" << std::endl
        << "--hue p             multiply hue by p" << std::endl
        << "--saturation p      multiply saturation by p" << std::endl
        << "--value p           multiply value by p" << std::endl
        << "--grayscale         convert image to grayscale" << std::endl
        << "--negate            obtain image's negative" << std::endl
        << "------------------------------------- [ FX ] -------------------------------------" << std::endl
        << "--sepia             give the image a sepia effect" << std::endl
        << "--solarize p        solarize the image by p%" << std::endl
        << "------------------------------- [ TRASFORMATIONS ] -------------------------------" << std::endl
        << "--flip                      flip the image" << std::endl
        << "--flop                      flop the image" << std::endl
        << "--padding t r b l c         pad the image on top (t), right (r), bottom (b), left (l) with color c" << std::endl
        << "--roll c                    move the image by c columns" << std::endl
        << "--rotate                    rotate the image" << std::endl
        << "--stretch s                 stretch the image by s" << std::endl;
}

Error hue(std::vector<std::string> args) {
    if (args.size() < 2) 
        return Error::FEW_ARGUMENTS;

    canvas->hue(ATOI_DEC(args[1]));
    return Error::NO_ERROR;
}

Error line(std::vector<std::string> args) {
    if (args.size() < 6)
        return Error::FEW_ARGUMENTS;

    Line line {ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_DEC(args[4]), ATOI_HEX(args[5])};
    canvas->draw(line);
    return Error::NO_ERROR;
}

void log(std::string flag, Error err) {
    switch (err) {
        case Error::FEW_ARGUMENTS:
            std::cout << RED << "Too few arguments to flag " << flag << RESET;
            break;
        
        case Error::GENERIC_ERROR:
            std::cout << RED << "Generic error occured in " << flag << RESET;
            break;

        case Error::UNKNOWN_FLAG:
            std::cout << RED << "Unknown flag: " << flag << RESET;
            break;
        
        default:
            break;
    }
}

int parse_args(std::vector<std::string> argv, std::string &out) {
    uint failed = 0;
    Error err = Error::NO_ERROR;

    for (uint i = 0; i < argv.size(); i++)

        if (cmds.find(argv.at(i)) != cmds.end()) {
            std::vector<std::string> subvec(argv.begin() + 1, argv.end());
            err = cmds[argv[i]](subvec);

            if (err != Error::NO_ERROR) {
                log(argv.at(i), err);
                failed++;
            }
            
        } else {
            log(argv.at(i), Error::UNKNOWN_FLAG);
        }

    return failed;
}

Error rectangle(std::vector<std::string> args) {
    if (args.size() < 6)
        return Error::FEW_ARGUMENTS;
    
    Rectangle rec {ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_DEC(args[4]), ATOI_HEX(args[5])}; 
    canvas->draw(rec);
    return Error::NO_ERROR;
}

Error rotate(std::vector<std::string>) {
    return canvas->rotate() != nullptr
        ? Error::NO_ERROR
        : Error::GENERIC_ERROR;
}

Error saturation(std::vector<std::string> args) {
    if (args.size() < 2)
        return Error::FEW_ARGUMENTS;

    canvas->saturation(ATOI_DEC(args[1]));
    return Error::NO_ERROR;
}

Error stretch(std::vector<std::string> args) {
    uint32_t **new_pixels;

    canvas->stretch(ATOI_DEC(args[1]), &new_pixels);
    free_pixels(new_pixels, 800);

    return Error::NO_ERROR;
}

Error text(std::vector<std::string> args) {
    if (args.size() < 6)
        return Error::FEW_ARGUMENTS;

    Text text {args[1], ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_DEC(args[4]), ATOI_HEX(args[5]), default_font}; 
    canvas->draw(text);
    return Error::NO_ERROR;
}

Error triangle(std::vector<std::string> args) {
    if (args.size() < 8)
        return Error::FEW_ARGUMENTS;

    Triangle triangle {ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_DEC(args[4]), ATOI_DEC(args[5]), ATOI_DEC(args[6]), ATOI_HEX(args[7])}; 
    canvas->draw(triangle);
    return Error::NO_ERROR;
}

Error value(std::vector<std::string> args) {
    if (args.size() < 2)
        return Error::FEW_ARGUMENTS;

    canvas->value(ATOI_DEC(args[1]));
    return Error::NO_ERROR;
}

int main(int argc, char *argv[]) {
    time_t t = std::time(nullptr);

    std::vector<std::string> arguments(argv + 1, argv + argc);
    if (cmp_flag(arguments.at(0), "-h", "--help"))
        help();

    std::string out {}, in {};
    find_files(arguments, in, out);

    if (in.empty()) {
        get_new_file_dimensions(arguments, &height, &width);

        pixels = new uint32_t[height * width];
        canvas = new EikonCanvas(pixels, height, width);

    } else {
        get_dimensions(in, &height, &width);

        pixels = new uint32_t[height * width];
        canvas = new EikonCanvas(pixels, height, width);

        canvas->read(in);
    }
    
    if (out.empty()) {
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%d-%m-%Y-%H-%M-%S.bmp");
        out = oss.str();
    }

    int failed = parse_args(arguments, out);

    if (!failed || (flags | SAVE_ON_ERROR))
        canvas->save(out);

    delete canvas;
    delete[] pixels;
    return 0;
}
