#include <algorithm>
#include <fstream>
#include <iostream>
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

static auto ask_to_save(std::ofstream &file, Canvas &canvas, unsigned int unsaved_changes) -> void;
static auto parse_cmd(Canvas &canvas, std::string cmd, unsigned int *idx) -> int;
static auto print_colors(uint32_t *pixels, size_t size) -> void;
static auto read_and_show_file_dimensions(Canvas &canvas, std::string file_name) -> int;
static auto run(Canvas &canvas, std::string file_name) -> void;
static inline auto show_banner() -> void;
static inline auto show_help() -> void;
static auto split(std::string s, std::string delimiter) -> std::vector<std::string>;

static auto ask_to_save(std::ofstream &file, Canvas &canvas, unsigned int unsaved_changes) -> void {
    char answer {};
    
    std::cout << "You have " << unsaved_changes << " unsaved changes, save ? [y/n] ";
    std::cin >> answer;

    if (answer == 'y' || answer == 'Y')
        save_to_ppm(file, canvas);
}

static auto parse_cmd(std::ofstream &file, Canvas &canvas, std::string cmd, unsigned int *unsaved_changes) -> int { 
    std::vector<std::string> args { split(cmd, " ") }; 
    std::string function { args[0] };

    if (function == "clear") {
        system("clear");
        return 0;
    
    } else if (function == "help") {
        show_help();
        return 0;
    
    } else if (function == "ls") {
        print_colors(canvas.pixels, canvas.height * canvas.width);
        return 0;
    
    } else if (function == "save") {
        save_to_ppm(file, canvas);
        file.seekp(0);
        *unsaved_changes = 0;
        return 0;
    
    } else if (function == "ascii" && args.size() >= 2) {
        effects::ascii(canvas, ATOI_DEC(args[1]));

    } else if (function == "fill" && args.size() >= 2) {
        fill(canvas, ATOI_HEX(args[1]));
    
    } else if (function == "flip") {
        effects::flip_ppm(canvas);

    } else if (function == "rotate") {
        effects::rotate_ppm(canvas);
    
    } else if (function == "circle" && args.size() >= 5) {
        shapes::circle(canvas, ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_HEX(args[4])); 

    } else if (function == "line" && args.size() >= 6) {
        shapes::line(canvas, ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_DEC(args[4]), ATOI_HEX(args[5]));

    } else if (function == "rectangle" && args.size() >= 6) {
        shapes::rectangle(canvas, ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_DEC(args[4]), ATOI_HEX(args[5])); 

    } else if (function == "text" && args.size() >= 6) {
        shapes::text(canvas, args[1], ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_DEC(args[4]), ATOI_HEX(args[5])); 
    
    } else if (function == "triangle" && args.size() >= 8) {
        shapes::triangle(canvas, ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_DEC(args[4]), ATOI_DEC(args[5]), ATOI_DEC(args[6]), ATOI_HEX(args[7])); 
    
    } else {
        return 1;
    }
    
    (*unsaved_changes)++;
    return 0; 
}

static auto print_colors(uint32_t *pixels, size_t size) -> void {
    std::vector<uint32_t> colors {};

    for (int i = 0; i < size; i++)
        if (std::find(colors.begin(), colors.end(), pixels[i]) == colors.end())
            colors.insert(colors.begin(), pixels[i]);
    
    for (uint32_t color: colors)
        std::cout << std::hex << color << "\t";

    std::cout << std::endl;
}

static auto read_and_show_file_dimensions(Canvas &canvas, std::string file_name) -> int {
    std::ifstream input_file {file_name, std::ios::in};
    if (!input_file.is_open())
        return 1;

    get_ppm_dimensions(input_file, canvas);
    uint32_t *pixels = (uint32_t*)malloc(canvas.width * canvas.height * sizeof(uint32_t));
    canvas.pixels = pixels;

    read_ppm(input_file, canvas);
    
    std::cout << "File dimensions are " << canvas.height << "x" << canvas.width << "\n";
    input_file.close();
    return 0;
}

static auto run(Canvas &canvas, std::string file_name) -> void {
    unsigned int unsaved_changes {0};

    std::string cmd {};
    std::ofstream output_file {file_name, std::ios::out};

    do {
        std::cout << GREEN << "raccoon@ppm:" << BLUE << file_name << RESET << "$ ";
        std::getline(std::cin, cmd);
        
        if (parse_cmd(output_file, canvas, cmd, &unsaved_changes) && cmd != "exit")
            std::cout << RED << "Unable to parse this command\n" << RESET;

    } while (cmd != "exit");

    if (unsaved_changes)
        ask_to_save(output_file, canvas, unsaved_changes);

    output_file.close();
}

static inline auto show_banner() -> void {
    std::cout << std::endl << std::endl << YELLOW;

    std::cout   << "\t██████╗  █████╗  ██████╗ ██████╗ ██████╗  ██████╗ ███╗   ██╗\n"
                << "\t██╔══██╗██╔══██╗██╔════╝██╔════╝██╔═══██╗██╔═══██╗████╗  ██║\n"
                << "\t██████╔╝███████║██║     ██║     ██║   ██║██║   ██║██╔██╗ ██║\n"
                << "\t██╔══██╗██╔══██║██║     ██║     ██║   ██║██║   ██║██║╚██╗██║\n"
                << "\t██║  ██║██║  ██║╚██████╗╚██████╗╚██████╔╝╚██████╔╝██║ ╚████║\n"
                << "\t╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═══╝\n" << RESET;
}

static inline auto show_help() -> void {
    std::cout   << "Raccoon CLI: simple utility tool using raccoon library\n\n"
                << "------------ [ BASE UTILS ] ------------\n"
                << "clear                           clear screen\n"
                << "exit                            exit the program\n"
                << "help                            show this menu\n"
                << "ls                              list colors in the ppm file\n"
                << "save                            save the file\n"
                << "------------ [ EFFECTS ] ------------\n"
                << "ascii scale                     print the ppm using ascii chars in scale 1:scale\n"
                << "fill color                      fill the ppm with a color\n"
                << "flip                            flip the ppm\n"
                << "rotate                          rotate the ppm of 90 degrees\n"
                << "------------ [ SHAPES ] ------------\n"
                << "circle x y r c                  draw a circle of color c and radius r in with center in P(x;y)\n"
                << "line x1 y1 x2 y2 c              draw a line passing through P(x1;y1) and Q(x2;y2) of color c\n"
                << "rectangle x y h b c             draw a rectangle of color c and base b, height h in position P(x;y)\n"
                << "text \"word\" x y fs c            write \"word\" of color c in position P(x;y) with font size fs\n"
                << "triangle x1 y1 x2 y2 x3 y3 c    triangle with veritices in P(x1;y1), Q(x2;y2), R(x3;y3) of color c\n";               
} 

static auto split(std::string s, std::string delimiter) -> std::vector<std::string> {
    size_t delim_len {delimiter.length()};
    size_t pos_start {0}, pos_end {0};

    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

auto main(int argc, char *argv[]) -> int {
    Canvas canvas {};
    std::string file_name {};

    show_banner();

    if (argc == 1) {
        std::cout << "Type the file you want to edit: ";
        std::getline(std::cin, file_name);
    } else {
        file_name = argv[1];
    }

    if (read_and_show_file_dimensions(canvas, file_name)) {
        std::cout << "Unable to open the file\n";
        return 1;
    }
    
    std::cout << "Type help to show the menu\n";
    run(canvas, file_name);

    std::cout << "Bye bye :)\n";

    free(canvas.pixels);
    return 0;
}
