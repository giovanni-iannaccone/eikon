#include <print>

#include <eikon/eikon.hpp>
#include <eikon/shapes.hpp>

#include "tfunctions.hpp"
#include "./include/tframework.hpp"

using Suite = TestSuite<
    Test<"area", area>,
    Test<"blur", blur>,
    Test<"brightness", brightness>,
    Test<"chop", chop>,
    Test<"circle", circle>,
    Test<"concat", concat>,
    Test<"contrast", contrast>,
    Test<"crop", crop>,
    Test<"ellipse", ellipse>,
    Test<"equalize", equalize>,
    Test<"flip_rectangle", flip_rectangle>,
    Test<"flop_rectangle", flop_rectangle>,
    Test<"gray_scale", gray_scale>,
    Test<"isolate", isolate>,
    Test<"line", line>,
    Test<"negate", negate>,
    Test<"overlap", overlap>,
    Test<"padding", padding>,
    Test<"raise", raise_>,
    Test<"roll", roll>,
    Test<"rotate_rectangle", rotate_rectangle>,
    Test<"saturation", saturation>,
    Test<"sepia", sepia>,
    Test<"solarize", solarize>,
    Test<"stretch", stretch>,
    Test<"text", text>,
    Test<"triangle", triangle>
>;

void run_tests(const std::string &ext)
{
    std::println("\n\n========== [TEST {}] ==========", ext);
    Suite::run(ext);
}

