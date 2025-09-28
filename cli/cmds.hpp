#pragma once

#include <string>
#include <vector>

enum Error {
    FEW_ARGUMENTS,
    GENERIC_ERROR,
    INVALID_DIMENSIONS,
    NO_ERROR,
    UNKNOWN_FLAG
};

namespace cmd {
    Error ascii(std::vector<std::string> args);

    Error chop(std::vector<std::string> args);
    Error crop(std::vector<std::string> args);
    Error fill(std::vector<std::string> args);
    Error flip(std::vector<std::string> args);
    Error flop(std::vector<std::string> args);
    Error padding(std::vector<std::string> args);
    Error roll(std::vector<std::string> args);
    Error rotate(std::vector<std::string>);
    Error stretch(std::vector<std::string> args);

    Error circle(std::vector<std::string> args);
    Error ellipse(std::vector<std::string> args);
    Error line(std::vector<std::string> args);
    Error rectangle(std::vector<std::string> args);
    Error text(std::vector<std::string> args);
    Error triangle(std::vector<std::string> args);

    Error brightness(std::vector<std::string> args);
    Error contrast(std::vector<std::string> args);
    Error equalize(std::vector<std::string> args);
    Error gray_scale(std::vector<std::string> args);
    Error negate(std::vector<std::string> args);

    Error hue(std::vector<std::string> args);
    Error saturation(std::vector<std::string> args);
    Error value(std::vector<std::string> args);

    Error add_noise(std::vector<std::string> args);
    Error blur(std::vector<std::string> args);
    Error raise(std::vector<std::string> args);

    Error isolate(std::vector<std::string> args);
    Error sepia(std::vector<std::string> args);
    Error solarize(std::vector<std::string> args);
}