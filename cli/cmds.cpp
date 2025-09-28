#include <eikon/eikon.hpp>

#include "cmds.hpp"
#include "data.hpp"
#include "utils.hpp"

Error cmd::add_noise(std::vector<std::string> args) {
    data::canvas->add_noise(ATOI_DEC(args[0]));
    return Error::NO_ERROR;
}

Error cmd::ascii(std::vector<std::string> args) {   
    data::canvas->ascii(ATOI_DEC(args[0]));
    return Error::NO_ERROR;
}

Error cmd::blur(std::vector<std::string> args) {
    data::canvas->blur(ATOI_DEC(args[0]));
    return Error::NO_ERROR;
}

Error cmd::brightness(std::vector<std::string> args) {
    data::canvas->brightness(ATOI_DEC(args[0]));
    return Error::NO_ERROR;
}

Error cmd::chop(std::vector<std::string> args) {
    if (data::width < ATOI_DEC(args[0]))
        return Error::INVALID_DIMENSIONS;

    data::canvas->chop_and_delete(ATOI_DEC(args[0]));

    data::width -= ATOI_DEC(args[0]);
    return Error::NO_ERROR;
}

Error cmd::circle(std::vector<std::string> args) {
    Circle circle {ATOI_DEC(args[0]), ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_HEX(args[3])}; 
    data::canvas->draw(circle);
    return Error::NO_ERROR;
}

Error cmd::contrast(std::vector<std::string> args) {
    data::canvas->contrast(ATOI_DEC(args[0]));
    return Error::NO_ERROR;
}

Error cmd::crop(std::vector<std::string> args) {
    if (data::height < ATOI_DEC(args[0]))
        return Error::INVALID_DIMENSIONS;

    data::canvas->crop_and_delete(ATOI_DEC(args[0]));

    data::height -= ATOI_DEC(args[0]);
    return Error::NO_ERROR;
}

Error cmd::ellipse(std::vector<std::string> args) {
    Ellipse ellipse {ATOI_DEC(args[0]), ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_HEX(args[4])};
    data::canvas->draw(ellipse);
    return Error::NO_ERROR;
}

Error cmd::fill(std::vector<std::string> args) {
    data::canvas->fill(ATOI_HEX(args[0]));
    return Error::NO_ERROR;
}

Error cmd::flip(std::vector<std::string> args) {
    data::canvas->flip();
    return Error::NO_ERROR;
}

Error cmd::flop(std::vector<std::string> args) {
    data::canvas->flop();
    return Error::NO_ERROR;
}

Error cmd::gray_scale(std::vector<std::string> args) {
    data::canvas->gray_scale();
    return Error::NO_ERROR;
}

Error cmd::hue(std::vector<std::string> args) {
    data::canvas->hue(ATOI_DEC(args[1]));
    return Error::NO_ERROR;
}

Error cmd::isolate(std::vector<std::string> args) {
    Channel ch;

    if (args[0] == "b")
        ch = Channel::BLUE;
    else if (args[0] == "g")
        ch = Channel::GREEN;
    else
        ch = Channel::RED;

    data::canvas->isolate(ch);
    return Error::NO_ERROR;
}

Error cmd::line(std::vector<std::string> args) {
    Line line {ATOI_DEC(args[0]), ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_HEX(args[4])};
    data::canvas->draw(line);
    return Error::NO_ERROR;
}

Error cmd::negate(std::vector<std::string> args) {
    data::canvas->negate();
    return Error::NO_ERROR;
}

Error cmd::padding(std::vector<std::string> args) {
    data::canvas->padding(ATOI_DEC(args[0]), ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_HEX(args[4]));
    return Error::NO_ERROR;
}

Error cmd::raise(std::vector<std::string> args) {
    data::canvas->raise(ATOI_DEC(args[0]));
    return Error::NO_ERROR;
}

Error cmd::rectangle(std::vector<std::string> args) {
    Rectangle rec {ATOI_DEC(args[0]), ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_HEX(args[4])};
    data::canvas->draw(rec);
    return Error::NO_ERROR;
}

Error cmd::rotate(std::vector<std::string> args) {
    return data::canvas->rotate() != nullptr
        ? Error::NO_ERROR
        : Error::GENERIC_ERROR;
}

Error cmd::saturation(std::vector<std::string> args) {
    data::canvas->saturation(ATOI_DEC(args[0]));
    return Error::NO_ERROR;
}

Error cmd::sepia(std::vector<std::string> args) {
    data::canvas->sepia();
    return Error::NO_ERROR;
}

Error cmd::solarize(std::vector<std::string> args) {
    data::canvas->solarize(ATOI_DEC(args[0]));
    return Error::NO_ERROR;
}

Error cmd::stretch(std::vector<std::string> args) {
    uint32_t **new_pixels;

    uint mul = ATOI_DEC(args[0]);
    data::canvas->stretch(mul, &new_pixels);

    free_pixels(new_pixels, data::height);

    data::width *= mul;
    return Error::NO_ERROR;
}

Error cmd::text(std::vector<std::string> args) {
    Text text {args[0], ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_HEX(args[4]), default_font}; 
    data::canvas->draw(text);
    return Error::NO_ERROR;
}

Error cmd::triangle(std::vector<std::string> args) {
    Triangle triangle {
        ATOI_DEC(args[0]), ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), 
        ATOI_DEC(args[4]), ATOI_DEC(args[5]), ATOI_HEX(args[6])
    };
     
    data::canvas->draw(triangle);
    return Error::NO_ERROR;
}

Error cmd::value(std::vector<std::string> args) {
    data::canvas->value(ATOI_DEC(args[0]));
    return Error::NO_ERROR;
}
