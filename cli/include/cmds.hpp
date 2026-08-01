#pragma once

#include <string>
#include <vector>

#include <eikon/eikon.hpp>

enum Error {
    FEW_ARGUMENTS,
    GENERIC_ERROR,
    INVALID_DIMENSIONS,
    NO_ERROR,
    UNKNOWN_FLAG
};

namespace cmd {
    Error ascii(eikon::Canvas &canvas, std::vector<std::string> &args);

    Error chop(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error crop(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error fill(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error flip(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error flop(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error padding(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error roll(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error rotate(eikon::Canvas &canvas, std::vector<std::string> &agrs);
    Error stretch(eikon::Canvas &canvas, std::vector<std::string> &args);

    Error circle(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error ellipse(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error line(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error rectangle(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error text(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error triangle(eikon::Canvas &canvas, std::vector<std::string> &args);

    Error brightness(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error contrast(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error equalize(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error gray_scale(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error negate(eikon::Canvas &canvas, std::vector<std::string> &args);

    Error hue(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error saturation(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error value(eikon::Canvas &canvas, std::vector<std::string> &args);

    Error add_noise(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error blur(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error raise(eikon::Canvas &canvas, std::vector<std::string> &args);

    Error isolate(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error sepia(eikon::Canvas &canvas, std::vector<std::string> &args);
    Error solarize(eikon::Canvas &canvas, std::vector<std::string> &args);
}
