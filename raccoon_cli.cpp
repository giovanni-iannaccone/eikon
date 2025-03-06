#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "./include/raccoon.hpp"

#define ATOI(str) std::atoi(str.c_str())

#define BLUE    "\033[34m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define RESET   "\033[0m"
#define YELLOW  "\033[33m"

static auto ask_to_save(std::ofstream &file, Canvas &canvas) -> void;
static auto parse_cmd(Canvas &canvas, std::string cmd, unsigned int *idx) -> int;
static auto read_and_show_file_dimensions(Canvas &canvas, std::string file_name) -> int;
static auto run(Canvas &canvas, std::string file_name) -> void;
static inline auto show_banner() -> void;
static inline auto show_help() -> void;
static auto split(std::string s, std::string delimiter) -> std::vector<std::string>;

static auto ask_to_save(std::ofstream &file, Canvas &canvas) -> void {
    char answer {};
    
    std::cout << "Save modifications ? [y/n] ";
    std::cin >> answer;

    if (answer == 'y' || answer == 'Y')
        save_to_ppm(file, canvas);
}

static auto parse_cmd(std::ofstream &file, Canvas &canvas, std::string cmd, unsigned int *idx) -> int { 
    std::vector<std::string> args { split(cmd, " ") }; 
    
    if (args[0] == "clear") {
        system("clear");
    
    } else if (args[0] == "circle" && args.size() >= 5) { 
        shapes::circle(canvas, ATOI(args[1]), ATOI(args[2]), ATOI(args[3]), ATOI(args[4])); 

    } else if (args[0] == "fill" && args.size() >= 2) { 
        fill(canvas, ATOI(args[1])); 

    } else if (args[0] == "flip") { 
        effects::flip_ppm(canvas); 

    } else if (args[0] == "help") { 
        show_help(); 

    } else if (args[0] == "rectangle" && args.size() >= 6) {
        shapes::rectangle(canvas, ATOI(args[1]), ATOI(args[2]), ATOI(args[3]), ATOI(args[4]), ATOI(args[5])); 

    } else if (args[0] == "rotate") { 
        effects::rotate_ppm(canvas);

    } else if (args[0] == "text" && args.size() >= 6) { 
        shapes::text(canvas, args[1], ATOI(args[2]), ATOI(args[3]), ATOI(args[4]), ATOI(args[5])); 
    
    } else if (args[0] == "save") {
        save_to_ppm(file, canvas);

    } else if (args[0] != "exit") {
        return 1; 
    } 
    
    (*idx)++;
    return 0; 
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
    unsigned int cmd_idx {0};

    std::string cmd {};
    std::ofstream output_file {file_name, std::ios::out};

    do {
        std::cout << GREEN << "raccoon@ppm:~" << BLUE << file_name << RESET << "$ ";
        std::getline(std::cin, cmd);
        
        if (parse_cmd(output_file, canvas, cmd, &cmd_idx))
            std::cout << RED << "Unable to parse this command\n" << RESET;

    } while (cmd != "exit");

    if (cmd_idx)
        ask_to_save(output_file, canvas);

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
                << "exit                      exit the program\n"
                << "fill color                fill the ppm with a color\n"
                << "flip                      flip the ppm\n"
                << "rotate                    rotate the ppmof 90 degrees\n"
                << "circle x y r c            draw a circle of color c and radius r in with center in P(x;y)\n"
                << "rectangle x y h b c       draw a rectangle of color c and base b, height h in position P(x;y)\n"
                << "text \"word\" x y fs c    write \"word\" of color c in position P(x;y) with font size fs\n"
                << "clear                     clear screen\n"
                << "save                      save the file\n";
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

auto main() -> int {
    Canvas canvas {};
    std::string file_name {};

    show_banner();

    std::cout << "Type the file you want to edit: ";
    std::getline(std::cin, file_name);

    if (read_and_show_file_dimensions(canvas, file_name)) {
        std::cout << "Unable to open the file";
        return 1;
    }
    
    std::cout << "Type help to show the menu\n";
    run(canvas, file_name);

    std::cout << "Bye bye :)\n";
    return 0;
}
