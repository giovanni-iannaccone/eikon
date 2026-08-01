#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <eikon/eikon.hpp>

#include "../include/cmds.hpp"
#include "../include/utils.hpp"

const uint8_t VERBOSE       = 0b00000001;
const uint8_t SAVE_ON_ERROR = 0b00000010;

const std::string RED_TEXT      = "\033[31m";
const std::string RESET_TEXT    = "\033[0m";

using cmd_func = std::function<Error (eikon::Canvas &, std::vector<std::string>&)>;

using cmdsMap =
    std::unordered_map<
        std::string,
        std::pair<cmd_func, uint>
    >;

static uint8_t flags = 0;

static const auto cmds = cmdsMap{{
    {"--add-noise",     {cmd::add_noise, 1}},
    {"--ascii",         {cmd::ascii, 1}},
    {"--blur",          {cmd::blur, 1}},
    {"--brightness",    {cmd::brightness, 1}},
    {"--chop",          {cmd::chop, 1}},
    {"--circle",        {cmd::circle, 4}},
    {"--crop",          {cmd::crop, 1}},
    {"--contrast",      {cmd::contrast, 1}},
    {"--ellipse",       {cmd::ellipse, 5}},
    {"--fill",          {cmd::fill, 1}},
    {"--flip",          {cmd::flip, 0}},
    {"--flop",          {cmd::flop, 0}},
    {"--grayscale",     {cmd::gray_scale, 0}},
    {"--hue",           {cmd::hue, 1}},
    {"--isolate",       {cmd::isolate, 1}},
    {"--line",          {cmd::line, 5}},
    {"--negate",        {cmd::negate, 0}},
    {"--padding",       {cmd::padding, 5}},
    {"--raise",         {cmd::raise, 1}},
    {"--rectangle",     {cmd::rectangle, 5}},
    {"--rotate",        {cmd::rotate, 0}},
    {"--saturation",    {cmd::saturation, 1}},
    {"--sepia",         {cmd::sepia, 0}},
    {"--solarize",      {cmd::solarize, 1}},
    {"--stretch",       {cmd::stretch, 1}},
    {"--text",          {cmd::text, 5}},
    {"--triangle",      {cmd::triangle, 7}},
    {"--value",         {cmd::value, 1}}
}};

static const std::unordered_map<std::string, std::function<void (void)>>
generic_flags = {
    {"--verbose",       [](){flags |= VERBOSE;      }},
    {"--save-on-error", [](){flags |= SAVE_ON_ERROR;}},
};

static inline bool cmp_flag(
    const std::string &flag,
    const std::string &short_form,
    const std::string &long_form) noexcept
{
    return flag == short_form || flag == long_form;
}

static inline std::pair<std::string, std::string>
find_files(std::vector<std::string>& args)
{
    std::string input;
    std::string output;

    for (auto it = args.begin(); it != args.end();) {
        std::string* target = nullptr;

        if (cmp_flag(*it, "-i", "--in"))
            target = &input;
        else if (cmp_flag(*it, "-o", "--out"))
            target = &output;

        if (!target) {
            ++it;
            continue;
        }

        *target = *std::next(it);

        it = args.erase(it);
        args.erase(it);
    }

    return {input, output};
}

static inline std::pair<uint, uint>
get_new_file_dimensions(std::vector<std::string> &argv)
{
    uint height, width;
    
    for (auto it = argv.begin(); it != argv.end();) {
        if (!cmp_flag(*it, "-s", "--size")) [[likely]] {
            it = std::next(it);
            continue;
        }

        argv.erase(it);

        height = ATOI_DEC(*(it));
        argv.erase(it);
        
        width  = ATOI_DEC(*(it));
        argv.erase(it);
        
        return {height, width};
    }

    return {0, 0};
}

static inline std::string get_timestamp() noexcept
{
    time_t t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y-%H-%M-%S");
    return oss.str();
}

[[noreturn]]
static void help() noexcept {
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

    std::exit(EXIT_FAILURE);
}

static void log(std::string flag, Error err) noexcept {
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

    default:
        break;
    }
}

static inline std::vector<std::string>
parse_cli(int argc, char* argv[]) noexcept {
    if (argc < 2)
        help();

    std::vector<std::string> args(argv + 1, argv + argc);

    if (cmp_flag(args.front(), "-h", "--help"))
        help();

    return args;
}

static inline
eikon::Canvas create_canvas(
    std::vector<std::string>& args,
    const std::string& input,
    std::string& output) noexcept
{
    if (input.empty()) {
        if (output.empty())
            output = get_timestamp() + ".bmp";

        auto [height, width] = get_new_file_dimensions(args);
        return {height, width};
    }

    if (output.empty())
        output = input;

    return {input};
}

static inline bool validate_canvas(const eikon::Canvas& canvas) noexcept
{
    if (canvas.height() != 0 && canvas.width() != 0)
        return true;

    log(
        std::to_string(canvas.height()) + "x" +
        std::to_string(canvas.width()),
        Error::INVALID_DIMENSIONS
    );

    return false;
}

static inline
void save_canvas(
    const eikon::Canvas& canvas, const std::string& output, int errors
) {
    if (errors == 0 || (flags & SAVE_ON_ERROR))
        canvas.save(output);
}

static bool try_run_generic_flag(const std::string& arg) noexcept
{
    auto it = generic_flags.find(arg);

    if (it == generic_flags.end())
        return false;

    it->second();
    return true;
}

static bool try_run_command(
    const std::vector<std::string>& args,
    eikon::Canvas& canvas,
    size_t& index,
    int& failed) noexcept
{
    auto it = cmds.find(args[index]);

    if (it == cmds.end())
        return false;

    auto& [func, argc] = it->second;

    if (args.size() - index - 1 < argc) {
        log(args[index], Error::FEW_ARGUMENTS);
        ++failed;
        return true;
    }

    if (flags & VERBOSE)
        std::cout << "Running " << args[index]
                  << " with " << argc
                  << " arguments\n";

    std::vector<std::string> params(
        args.begin() + index + 1,
        args.begin() + index + argc + 1
    );

    if (Error err = func(canvas, params); err != Error::NO_ERROR) {
        log(args[index], err);
        ++failed;
    }

    index += argc;
    return true;
}

static int parse_args(
    const std::vector<std::string>& args, eikon::Canvas& canvas
) noexcept
{
    int failed = 0;

    for (size_t i = 0; i < args.size(); ++i) {
        if (try_run_command(args, canvas, i, failed))
            continue;

        if (try_run_generic_flag(args[i]))
            continue;

        log(args[i], Error::UNKNOWN_FLAG);
    }

    return failed;
}

int main(int argc, char* argv[]) {
    auto arguments = parse_cli(argc, argv);

    auto [input, output] = find_files(arguments);
    auto canvas = create_canvas(arguments, input, output);

    if (!validate_canvas(canvas))
        return 1;

    const int errors = parse_args(arguments, canvas);

    save_canvas(canvas, output, errors);
    return 0;
}
