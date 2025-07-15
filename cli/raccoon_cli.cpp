#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "raccoon_cli.hpp"
#include "../include/raccoon.hpp"

unsigned int unsaved_changes = 0;
RaccoonCanvas *canvas;

std::map<std::string, std::function<int (std::vector<std::string>)>> cmds = {
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

int ascii(std::vector<std::string> args) {
    if (args.size() < 2)
        return 1;
    
    canvas->ascii(ATOI_DEC(args[1]));
    return 1;
}

void ask_to_save(std::ofstream &file, unsigned int unsaved_changes) {
    char answer {};
    
    std::cout << "You have " << unsaved_changes << " unsaved changes, save? [y/n] ";
    std::cin >> answer;

    if (answer == 'y' || answer == 'Y')
        canvas->save_to_ppm(file);
}

int circle(std::vector<std::string> args) {
    if (args.size() < 5)
        return 1;

    auto circle = new Circle(ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_HEX(args[4])); 
    canvas->draw(circle);
    delete circle;
    return 0;
}

void clear_screen() {
    system(
        #if defined(__WIN32__) || defined(__WIN64__)
            "cls"
        #else 
            "clear"
        #endif
    );
}

int fill(std::vector<std::string> args) {
    if (args.size() < 2)
        return 0;

    canvas->fill(ATOI_HEX(args[1]));
    return 0;
}

int flip(std::vector<std::string>) {
    canvas->flip();
    return 0;
}

int hue(std::vector<std::string> args) {
    if (args.size() < 2) 
        return 1;

    canvas->hue(ATOI_DEC(args[1]));
    return 0;
}

int line(std::vector<std::string> args) {
    if (args.size() < 6)
        return 1;

    auto line = new Line(ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_DEC(args[4]), ATOI_HEX(args[5]));
    canvas->draw(line);
    delete line;
    return 0;
}

int parse_cmd(std::string cmd, std::string file_name) { 
    std::vector<std::string> args { split(cmd, " ") }; 
    std::string function { args[0] };
    int failed {0};

    if (function == "clear") {
        clear_screen();
        return 0;
    
    } else if (function == "help") {
        show_help();
        return 0;
    
    } else if (function == "save") {
        std::ofstream file(file_name, std::ios::out);
        canvas->save_to_ppm(file);
        unsaved_changes = 0;
        return 0;

    } else if (cmds.find(function) != cmds.end()) {
        failed = cmds[function](args);
    
    } else {
        failed = 1;
    }

    if (!failed)
        unsaved_changes++;
    
    return failed; 
}

int read_and_show_file_dimensions(std::string file_name) {
    std::ifstream input_file {file_name, std::ios::in};
    if (!input_file.is_open())
        return 1;

    size_t height {}, width {};
    get_ppm_dimensions(input_file, &height, &width);

    uint32_t *pixels = new uint32_t[height * width];
    
    canvas = new RaccoonCanvas(pixels, height, width);
    canvas->read_ppm(input_file);

    std::cout << "File dimensions are " << height << "x" << width << "\n";

    input_file.close();
    return 0;
}

int rectangle(std::vector<std::string> args) {
    if (args.size() < 6)
        return 1;
    
    auto rec = new Rectangle(ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_DEC(args[4]), ATOI_HEX(args[5])); 
    canvas->draw(rec);
    delete rec;
    return 0;
}

int rotate(std::vector<std::string>) {
    return canvas->rotate() != nullptr;
}

void run(std::string file_name) {
    std::string cmd {};
    std::ofstream output_file {file_name, std::ios::out};

    do {
        std::cout << GREEN << "raccoon@ppm:" << BLUE << file_name << RESET << "$ ";
        std::getline(std::cin, cmd);
        
        if (parse_cmd(cmd, file_name) && cmd != "exit")
            std::cout << RED << "Unable to parse this command\n" << RESET;

    } while (cmd != "exit");

    if (unsaved_changes)
        ask_to_save(output_file, unsaved_changes);

    output_file.close();
    delete canvas;
}

int saturation(std::vector<std::string> args) {
    if (args.size() < 2)
        return 1;

    canvas->saturation(ATOI_DEC(args[1]));
    return 0;
}

static inline void show_banner() {
    std::cout   << "\n\n" << YELLOW
                << "\t██████╗  █████╗  ██████╗ ██████╗ ██████╗  ██████╗ ███╗   ██╗\n"
                << "\t██╔══██╗██╔══██╗██╔════╝██╔════╝██╔═══██╗██╔═══██╗████╗  ██║\n"
                << "\t██████╔╝███████║██║     ██║     ██║   ██║██║   ██║██╔██╗ ██║\n"
                << "\t██╔══██╗██╔══██║██║     ██║     ██║   ██║██║   ██║██║╚██╗██║\n"
                << "\t██║  ██║██║  ██║╚██████╗╚██████╗╚██████╔╝╚██████╔╝██║ ╚████║\n"
                << "\t╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═══╝\n" << RESET;
}

static inline void show_help() {
    std::cout   << "Raccoon CLI: simple utility tool using raccoon library\n\n"
                << "------------ [ BASE UTILS ] ------------\n"
                << "clear                           clear screen\n"
                << "exit                            exit the program\n"
                << "help                            show this menu\n"
                << "save                            save the file\n"
                << "------------ [ EFFECTS ] ------------\n"
                << "ascii scale                     print the ppm using ascii chars in scale 1:scale\n"
                << "fill color                      fill the ppm with a color\n"
                << "flip                            flip the ppm\n"
                << "rotate                          rotate the ppm of 90 degrees\n"
                << "stretch s                       scale the image by a factor of size s\n"
                << "------------ [ COLORS ] ------------\n"
                << "hue i                           increase hue by i\n"
                << "saturation i                    increase saturation by i\n"
                << "value i                         increase value by i\n"
                << "------------ [ SHAPES ] ------------\n"
                << "circle r x y c                  draw a circle of color c and radius r in with center in P(x;y)\n"
                << "line x1 y1 x2 y2 c              draw a line passing through P(x1;y1) and Q(x2;y2) of color c\n"
                << "rectangle x y h b c             draw a rectangle of color c and base b, height h in position P(x;y)\n"
                << "text \"word\" x y fs c            write \"word\" of color c in position P(x;y) with font size fs\n"
                << "triangle x1 y1 x2 y2 x3 y3 c    triangle with veritices in P(x1;y1), Q(x2;y2), R(x3;y3) of color c\n";               
} 

std::vector<std::string> split(std::string s, std::string delimiter) {
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

int stretch(std::vector<std::string> args) {
    canvas->stretch(ATOI_DEC(args[1]));
    return 0;
}

int text(std::vector<std::string> args) {
    if (args.size() < 6)
        return 1;

    auto text = new Text(&args[1], ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_DEC(args[4]), ATOI_HEX(args[5]), &default_font); 
    canvas->draw(text);
    delete text;
    return 0;
}

int triangle(std::vector<std::string> args) {
    if (args.size() < 8)
        return 1;

    auto triangle = new Triangle(ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_DEC(args[4]), ATOI_DEC(args[5]), ATOI_DEC(args[6]), ATOI_HEX(args[7])); 
    canvas->draw(triangle);
    delete triangle;
    return 0;
}

int value(std::vector<std::string> args) {
    if (args.size() < 2)
        return 1;

    canvas->value(ATOI_DEC(args[1]));
    return 0;
}

int main(int argc, char *argv[]) {
    std::string file_name {};

    show_banner();

    if (argc == 1) {
        std::cout << "Type the file you want to edit: ";
        std::getline(std::cin, file_name);
    } else {
        file_name = argv[1];
    }

    if (read_and_show_file_dimensions(file_name)) {
        std::cout << "Unable to open the file\n";
        return 1;
    }
    
    std::cout << "Type help to show the menu\n";
    run(file_name);

    std::cout << "Bye bye :)\n";
    return 0;
}
