#include <eikon/eikon.hpp>

#include "../include/cmds.hpp"
#include "../include/utils.hpp"

Error cmd::add_noise(eikon::Canvas &canvas, std::vector<std::string> &args) {
    canvas.add_noise(ATOI_DEC(args[0]));
    return Error::NO_ERROR;
}

Error cmd::ascii(eikon::Canvas &canvas, std::vector<std::string> &args) {   
    canvas.ascii(ATOI_DEC(args[0]));
    return Error::NO_ERROR;
}

Error cmd::blur(eikon::Canvas &canvas, std::vector<std::string> &args) {
    canvas.blur(ATOI_DEC(args[0]));
    return Error::NO_ERROR;
}

Error cmd::brightness(eikon::Canvas &canvas, std::vector<std::string> &args) {
    canvas.brightness(ATOF(args[0]));
    return Error::NO_ERROR;
}

Error cmd::chop(eikon::Canvas &canvas, std::vector<std::string> &args) {
    if (canvas.width() < ATOI_DEC(args[0]))
        return Error::INVALID_DIMENSIONS;

    canvas.chop(ATOI_DEC(args[0]));
    return Error::NO_ERROR;
}

Error cmd::circle(eikon::Canvas &canvas, std::vector<std::string> &args) {
    eikon::shapes::SafeCircle circle {
        ATOF(args[0]), ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_HEX(args[3])
    };
    
    canvas.draw(circle);
    return Error::NO_ERROR;
}

Error cmd::contrast(eikon::Canvas &canvas, std::vector<std::string> &args) {
    canvas.contrast(ATOF(args[0]));
    return Error::NO_ERROR;
}

Error cmd::crop(eikon::Canvas &canvas, std::vector<std::string> &args) {
    if (canvas.height() < ATOI_DEC(args[0]))
        return Error::INVALID_DIMENSIONS;

    canvas.crop(ATOI_DEC(args[0]));
    return Error::NO_ERROR;
}

Error cmd::ellipse(eikon::Canvas &canvas, std::vector<std::string> &args) {
    eikon::shapes::SafeEllipse ellipse {
        ATOI_DEC(args[0]), ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_HEX(args[4])
    };
    
    canvas.draw(ellipse);
    return Error::NO_ERROR;
}

Error cmd::fill(eikon::Canvas &canvas, std::vector<std::string> &args) {
    canvas.fill(ATOI_HEX(args[0]));
    return Error::NO_ERROR;
}

Error cmd::flip(eikon::Canvas &canvas, std::vector<std::string> &) {
    canvas.flip();
    return Error::NO_ERROR;
}

Error cmd::flop(eikon::Canvas &canvas, std::vector<std::string> &) {
    canvas.flop();
    return Error::NO_ERROR;
}

Error cmd::gray_scale(eikon::Canvas &canvas, std::vector<std::string> &) {
    canvas.gray_scale();
    return Error::NO_ERROR;
}

Error cmd::hue(eikon::Canvas &canvas, std::vector<std::string> &args) {
    canvas.hue(ATOI_DEC(args[1]));
    return Error::NO_ERROR;
}

Error cmd::isolate(eikon::Canvas &canvas, std::vector<std::string> &args) {
    eikon::Channel ch;

    if (args[0] == "b")
        ch = eikon::Channel::BLUE;
    else if (args[0] == "g")
        ch = eikon::Channel::GREEN;
    else
        ch = eikon::Channel::RED;

    canvas.isolate(ch);
    return Error::NO_ERROR;
}

Error cmd::line(eikon::Canvas &canvas, std::vector<std::string> &args) {
    eikon::shapes::Line line {
        ATOI_DEC(args[0]), ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_HEX(args[4])
    };
    
    canvas.draw(line);
    return Error::NO_ERROR;
}

Error cmd::negate(eikon::Canvas &canvas, std::vector<std::string> &) {
    canvas.negate();
    return Error::NO_ERROR;
}

Error cmd::padding(eikon::Canvas &canvas, std::vector<std::string> &args) {
    canvas.padding(ATOI_DEC(args[0]), ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_HEX(args[4]));
    return Error::NO_ERROR;
}

Error cmd::raise(eikon::Canvas &canvas, std::vector<std::string> &args) {
    canvas.raise(ATOI_DEC(args[0]));
    return Error::NO_ERROR;
}

Error cmd::rectangle(eikon::Canvas &canvas, std::vector<std::string> &args) {
    eikon::shapes::SafeRectangle rec {
        ATOI_DEC(args[0]), ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_HEX(args[4])
    };
    
    canvas.draw(rec);
    return Error::NO_ERROR;
}

Error cmd::rotate(eikon::Canvas &canvas, std::vector<std::string> &) {
    canvas.rotate();
    return Error::NO_ERROR;
}

Error cmd::saturation(eikon::Canvas &canvas, std::vector<std::string> &args) {
    canvas.saturation(ATOF(args[0]));
    return Error::NO_ERROR;
}

Error cmd::sepia(eikon::Canvas &canvas, std::vector<std::string> &) {
    canvas.sepia();
    return Error::NO_ERROR;
}

Error cmd::solarize(eikon::Canvas &canvas, std::vector<std::string> &args) {
    canvas.solarize(ATOI_DEC(args[0]));
    return Error::NO_ERROR;
}

Error cmd::stretch(eikon::Canvas &canvas, std::vector<std::string> &args) {
    canvas.stretch(ATOI_DEC(args[0]));
    return Error::NO_ERROR;
}

Error cmd::text(eikon::Canvas &canvas, std::vector<std::string> &args) {
    eikon::shapes::Text text {
        args[0], ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), ATOI_HEX(args[4]), eikon::default_font
    }; 
    canvas.draw(text);
    return Error::NO_ERROR;
}

Error cmd::triangle(eikon::Canvas &canvas, std::vector<std::string> &args) {
    eikon::shapes::SafeTriangle triangle {
        ATOI_DEC(args[0]), ATOI_DEC(args[1]), ATOI_DEC(args[2]), ATOI_DEC(args[3]), 
        ATOI_DEC(args[4]), ATOI_DEC(args[5]), ATOI_HEX(args[6])
    };
     
    canvas.draw(triangle);
    return Error::NO_ERROR;
}

Error cmd::value(eikon::Canvas &canvas, std::vector<std::string> &args) {
    canvas.value(ATOF(args[0]));
    return Error::NO_ERROR;
}
