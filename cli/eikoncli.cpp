#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <utility>

#include <eikon/eikon.hpp>

#include "eikoncli.hpp"

static uint8_t flags = 0;

auto cmds = cmdsMap{{
    {"--add-noise",     std::make_pair(cmd::add_noise, 1)},
    {"--ascii",         std::make_pair(cmd::ascii, 1)},
    {"--blur",          std::make_pair(cmd::blur, 1)},
    {"--brightness",    std::make_pair(cmd::brightness, 1)},
    {"--chop",          std::make_pair(cmd::chop, 1)},
    {"--circle",        std::make_pair(cmd::circle, 4)},
    {"--crop",          std::make_pair(cmd::crop, 1)},
    {"--contrast",      std::make_pair(cmd::contrast, 1)},
    {"--ellipse",       std::make_pair(cmd::ellipse, 5)},
    {"--fill",          std::make_pair(cmd::fill, 1)},
    {"--flip",          std::make_pair(cmd::flip, 0)},
    {"--flop",          std::make_pair(cmd::flop, 0)},
    {"--grayscale",     std::make_pair(cmd::gray_scale, 0)},
    {"--hue",           std::make_pair(cmd::hue, 1)},
    {"--isolate",       std::make_pair(cmd::isolate, 1)},
    {"--line",          std::make_pair(cmd::line, 5)},
    {"--negate",        std::make_pair(cmd::negate, 0)},
    {"--padding",       std::make_pair(cmd::padding, 5)},
    {"--raise",         std::make_pair(cmd::raise, 1)},
    {"--rectangle",     std::make_pair(cmd::rectangle, 5)},
    {"--rotate",        std::make_pair(cmd::rotate, 0)},
    {"--saturation",    std::make_pair(cmd::saturation, 1)},
    {"--sepia",         std::make_pair(cmd::sepia, 0)},
    {"--solarize",      std::make_pair(cmd::solarize, 1)},
    {"--stretch",       std::make_pair(cmd::stretch, 1)},
    {"--text",          std::make_pair(cmd::text, 5)},
    {"--triangle",      std::make_pair(cmd::triangle, 7)},
    {"--value",         std::make_pair(cmd::value, 1)},
}};

std::map<std::string, std::function<void (void)>> generic_flags = {
    {"--verbose",       [](){flags |= VERBOSE;      }},
    {"--save-on-error", [](){flags |= SAVE_ON_ERROR;}},
};

bool cmp_flag(const std::string &flag, const std::string &short_form, const std::string &long_form) {
    return flag == short_form || flag == long_form;
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

std::string get_timestamp() {
    time_t t = std::time(nullptr);
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
        << "if -i is not specified, give -s to set new image's dimensions (h w)" << std::endl << std::endl

        << "-------------------------------- [ GENERIC FLAGS] --------------------------------" << std::endl
        << "--verbose           increase verbosity" << std::endl
        << "--save-on-error     if during flag parsing any error is found, save the file anyway" << std::endl << std::endl

        << "----------------------------------- [ SHAPES ] -----------------------------------" << std::endl
        << "--circle r cx cy color              draw a circle of radius r, center (cx; cy) and specified color" << std::endl
        << "--ellipse a b cx cy color           draw an ellipse with axes a and b and center (cx; cy) of specified color" << std::endl
        << "--line x1 y1 x2 y2 color            draw a line from (x1; y1) to (x2; y2) of specified color" << std::endl
        << "--rectangle x y h b color           draw a rectangle starting from (x; y) with height h and base b of specified color" << std::endl
        << "--triangle x1 y1 x2 y2 x3 y3 c      draw a triangle with vertices in (x1; y1), (x2; y2), (x3; y3) of color c" << std::endl
        << "--text \"word\" x y fs c              write \"word\" starting from (x; y) of color c with font size fs" << std::endl << std::endl

        << "----------------------------------- [ EFFECTS ] ----------------------------------" << std::endl
        << "--add-noise i       add noise with i intensity" << std::endl
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
        << "--isolate c         isolate an rgb channel: set c to r for red, g for green and b for blue" << std::endl
        << "--sepia             give the image a sepia effect" << std::endl
        << "--solarize p        solarize the image by p%" << std::endl << std::endl

        << "------------------------------- [ TRASFORMATIONS ] -------------------------------" << std::endl
        << "--chop c                    chop the image to column c" << std::endl
        << "--crop r                    crop the image to row r" << std::endl
        << "--flip                      flip the image" << std::endl
        << "--flop                      flop the image" << std::endl
        << "--padding t r b l c         pad the image on top (t), right (r), bottom (b), left (l) with color c" << std::endl
        << "--roll c                    move the image by c columns" << std::endl
        << "--rotate                    rotate the image" << std::endl
        << "--stretch s                 stretch the image by s" << std::endl;
}

void log(std::string flag, Error err) {
    switch (err) {
        case Error::FEW_ARGUMENTS:
            std::cout << RED_TEXT << "Too few arguments to flag " << flag << RESET_TEXT << std::endl;
            break;
        
        case Error::GENERIC_ERROR:
            std::cout << RED_TEXT << "Generic error occured in " << flag << RESET_TEXT << std::endl;
            break;

        case Error::INVALID_DIMENSIONS:
            std::cout << RED_TEXT << "Invalid dimenions " << flag << RESET_TEXT << std::endl;
            break;

        case Error::UNKNOWN_FLAG:
            std::cout << RED_TEXT << "Unknown flag: " << flag << RESET_TEXT << std::endl;
            break;
    }
}

int parse_args(std::vector<std::string> argv) {
    uint failed = 0;
    Error err = Error::NO_ERROR;

    for (uint i = 0; i < argv.size(); i++)

        if (cmds.find(argv.at(i)) != cmds.end()) {
            auto [func, inc] = cmds[argv[i]];

            if (argv.size() - i - 1 < inc) {
                err = Error::FEW_ARGUMENTS;
            
            } else {
                if (flags & VERBOSE)
                    std::cout << "Running " << argv.at(i) << " with " << inc << " flags";

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

int main(int argc, char *argv[]) {
    if (argc == 1) {
        help();
        return 0;
    }

    std::vector<std::string> arguments(argv + 1, argv + argc);
    if (cmp_flag(arguments.at(0), "-h", "--help")) {
        help();
        return 0;
    }

    std::string out {}, in {};
    find_files(arguments, in, out);

    if (in.empty()) {
        if (out.empty())
            out = get_timestamp() + ".bmp";
    
        get_new_file_dimensions(arguments, &data::height, &data::width);

        uint32_t **pixels = allocate_pixels(data::height, data::width);
        data::canvas = new EikonCanvas(pixels, data::height, data::width);

    } else {
        if (out.empty())
            out = in;

        data::canvas = new EikonCanvas(in, nullptr, &data::height, &data::width);
    }
    
    if (data::height == 0 || data::width == 0) {
        log(std::to_string(data::height) + "x" + std::to_string(data::width), Error::INVALID_DIMENSIONS);
        return 1;
    }

    if (parse_args(arguments) == 0 || (flags & SAVE_ON_ERROR))
        data::canvas->save(out);

    data::canvas->free_all();
    return 0;
}
