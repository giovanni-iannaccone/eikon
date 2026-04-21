#pragma once

#include <string>

#include <eikon/eikon.hpp>
#include <eikon/shapes.hpp>

inline void area(eikon::Canvas &canvas) {
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFF0000FF};

    canvas.fill(0xFF000000)
        .draw(circle)
        .area(300, 300, 100, 100)
        .flop();
}

inline void blur(eikon::Canvas &canvas) {
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFF00FF00};
    eikon::shapes::Triangle triangle {100, 100, 600, 200, 400, 500, 0xFFFFFF00};
    eikon::shapes::Text text {"eikon", 150, 200, 10, 0xFFFF5C00};

    canvas.fill(0xFF000000)
        .draw(triangle)
        .draw(circle)
        .draw(text)
        .blur(5);
}

inline void brightness(eikon::Canvas &canvas) {
    eikon::shapes::Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};

    canvas.fill(0xFF000000)
        .draw(rec)
        .brightness(1.5);
}

inline void chop(eikon::Canvas &canvas) {
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFFFF5C00};

    canvas.fill(0xFF000000)
        .draw(circle)
        .chop(-250);
}

inline void circle(eikon::Canvas &canvas) {
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFF0000FF};
    
    canvas.fill(0xFF000000)
        .draw(circle);
}

inline void concat(eikon::Canvas &first) {
    eikon::shapes::SafeCircle c1 {75.0, 325, 500, 0xFFFF0000};
    eikon::shapes::SafeCircle c2 {75.0, 475, 500, 0xFFFF0000};
    
    eikon::shapes::Triangle triangle {250, 0, 550, 0, 400, 200, 0xFFFF0000};

    eikon::Canvas second {first.height(), first.width()};

    second.fill(0xFF000000)
        .draw(triangle);

    first = std::move(
        first.fill(0xFF000000)
        .crop(-300)
        .draw(c1)
        .draw(c2)
        .concat(second, eikon::utils::Axis::Y)
    );
}

inline void contrast(eikon::Canvas &canvas) {
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFF00007F};

    canvas.fill(0xFF0F0F0F)
        .draw(circle)
        .contrast(1.5);
}

inline void crop(eikon::Canvas &canvas) {
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFFFFFF00};

    canvas.fill(0xFF000000)
        .draw(circle)
        .crop(-250);
}

inline void ellipse(eikon::Canvas &canvas) {
    eikon::shapes::Ellipse ellipse {400, 400, 200, 100, 0xFF00FF00};

    canvas.fill(0xFF000000)
        .draw(ellipse);
}

inline void equalize(eikon::Canvas &canvas) {
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFF0000FF};
    eikon::shapes::Ellipse ellipse {300, 110, 200, 100, 0xFF00FF00};
    eikon::shapes::Triangle triangle {600, 200, 750, 200, 400, 500, 0xFFFFFF00};

    canvas.fill(0xFF000000)
        .draw(circle)
        .draw(ellipse)
        .draw(triangle)
        .equalize();
}

inline void flip_rectangle(eikon::Canvas &canvas) {
    eikon::shapes::Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};

    canvas.fill(0xFF000000)
        .draw(rec)
        .flip();
}

inline void flop_rectangle(eikon::Canvas &canvas) {
    eikon::shapes::Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};

    canvas.fill(0xFF000000)
        .draw(rec)
        .flop();
}

inline void gray_scale(eikon::Canvas &canvas) {
    eikon::shapes::Rectangle rec {150, 200, 100, 200, 0xFFFF00AB};

    canvas.fill(0xFF15FFA1)
        .draw(rec)
        .gray_scale();
}

inline void isolate(eikon::Canvas &canvas) {
    eikon::shapes::Triangle triangle {100, 100, 600, 200, 400, 500, 0xFFFFFFFF};

    canvas.fill(0xFF000000)
        .draw(triangle)
        .isolate(eikon::Channel::BLUE);
}

inline void line(eikon::Canvas &canvas) {
    eikon::shapes::Line l {0, 0, 800, 800, 0xFFFF00FF};

    canvas.fill(0xFF000000)
        .draw(l);
}

inline void negate(eikon::Canvas &canvas) {
    
    eikon::shapes::Rectangle r1 = {100, 200, 200, 300, 0xFFFF00FF};
    eikon::shapes::Circle c1 = {75, 300, 300, 0x7FFF0000};
    eikon::shapes::Circle c2 = {150, 400, 400, 0xFF00FF00};

    canvas.fill(0xFF000000)
        .draw(r1)
        .draw(c1)
        .draw(c2)
        .negate();
}

inline void overlap(eikon::Canvas &canvas) {
    eikon::shapes::Circle c1 {100, 400, 400, 0xFF0000FF};
    eikon::shapes::Circle c2 {75, 300, 300, 0x7FFF0000};

    canvas.fill(0xFF000000)
        .draw(c1)
        .draw(c2);
}

inline void padding(eikon::Canvas &canvas) {
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFFFFFFFF};

    canvas.fill(0xFF000000)
        .draw(circle)
        .padding(50, 10, 20, 100, 0xFFFF0000);
}

inline void raise_(eikon::Canvas &canvas) {
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFFFF0000};

    canvas.fill(0xFF2F2F2F)
        .draw(circle)
        .raise(30);
}

inline void roll(eikon::Canvas &canvas) {
    eikon::shapes::Rectangle rec {50, 200, 100, 200, 0xFF15FFA1};
    
    canvas.fill(0xFF000000)
        .draw(rec)
        .roll(700);
}

inline void rotate_rectangle(eikon::Canvas &canvas) {
    eikon::shapes::Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};
    
    canvas.fill(0xFF000000)
        .draw(rec)
        .rotate();
}

inline void saturation(eikon::Canvas &canvas) {
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFF00FF00};

    canvas.fill(0xFF000000)
        .draw(circle)
        .saturation(1.5);
}

inline void sepia(eikon::Canvas &canvas) {
    eikon::shapes::Rectangle rec {150, 200, 100, 200, 0xFFFF00AB};
    eikon::shapes::Circle circle {100, 400, 400, 0xFFFF0000};

    canvas.fill(0xFF15FFA1)
        .draw(rec)
        .draw(circle)
        .sepia();
}

inline void solarize(eikon::Canvas &canvas) {
    eikon::shapes::Triangle triangle {100, 100, 600, 200, 400, 500, 0xFF00008F};
    eikon::shapes::Circle circle {100, 400, 400, 0xFF00C2D2};

    canvas.fill(0xFFFFFFFF)
        .draw(triangle)
        .draw(circle)
        .solarize();
}

inline void stretch(eikon::Canvas &canvas) {
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFFFFFFFF};

    canvas.fill(0xFF000000)
        .draw(circle)
        .stretch(2);
}

inline void text(eikon::Canvas &canvas) {
    eikon::shapes::Text txt {"hello, world!", 150, 200, 10, 0xFF00FF00};

    canvas.fill(0xFF000000)
        .draw(txt);
}

inline void triangle(eikon::Canvas &canvas) {
    eikon::shapes::Triangle triangle {100, 100, 600, 200, 400, 500, 0xFFFF00EE};

    canvas.fill(0xFF000000)
        .draw(triangle);
}
