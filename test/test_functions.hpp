#pragma once

#include <string>
#include <eikon/eikon.hpp>

void area(eikon::Canvas &canvas) {
    eikon::Circle circle {100.0, 400, 400, 0xFF0000FF};

    canvas.fill(0xFF000000)
        ->draw(circle)
        ->area(300, 300, 100, 100)
        ->flop();
}

void blur(eikon::Canvas &canvas) {
    eikon::Circle circle {100.0, 400, 400, 0xFF00FF00};
    eikon::Triangle triangle {100, 100, 600, 200, 400, 500, 0xFFFFFF00};
    eikon::Text text {"eikon", 150, 200, 10, 0xFFFF5C00};

    canvas.fill(0xFF000000)
        ->draw(triangle)
        ->draw(circle)
        ->draw(text)
        ->blur(5);
}

void brightness(eikon::Canvas &canvas) {
    eikon::Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};

    canvas.fill(0xFF000000)
        ->draw(rec)
        ->brightness(1.5);
}

void chop(eikon::Canvas &canvas) {
    eikon::Circle circle {100.0, 400, 400, 0xFFFF5C00};

    canvas.fill(0xFF000000)
        ->draw(circle)
        ->chop(-250);
}

void circle(eikon::Canvas &canvas) {
    eikon::Circle circle {100.0, 400, 400, 0xFF0000FF};

    canvas.fill(0xFF000000)
        ->draw(circle);
}

void contrast(eikon::Canvas &canvas) {
    eikon::Circle circle {100.0, 400, 400, 0xFF00007F};

    canvas.fill(0xFF0F0F0F)
        ->draw(circle)
        ->contrast(1.5);
}

void crop(eikon::Canvas &canvas) {
    eikon::Circle circle {100.0, 400, 400, 0xFFFFFF00};

    canvas.fill(0xFF000000)
        ->draw(circle)
        ->crop(-250);
}

void ellipse(eikon::Canvas &canvas) {
    eikon::Ellipse ellipse {400, 400, 200, 100, 0xFF00FF00};

    canvas.fill(0xFF000000)
        ->draw(ellipse);
}

void equalize(eikon::Canvas &canvas) {
    eikon::Circle circle {100.0, 400, 400, 0xFF0000FF};
    eikon::Ellipse ellipse {300, 110, 200, 100, 0xFF00FF00};
    eikon::Triangle triangle {600, 200, 750, 200, 400, 500, 0xFFFFFF00};

    canvas.fill(0xFF000000)
        ->draw(circle)
        ->draw(ellipse)
        ->draw(triangle)
        ->equalize();
}

void flip_rectangle(eikon::Canvas &canvas) {
    eikon::Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};

    canvas.fill(0xFF000000)
        ->draw(rec)
        ->flip();
}

void flop_rectangle(eikon::Canvas &canvas) {
    eikon::Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};

    canvas.fill(0xFF000000)
        ->draw(rec)
        ->flop();
}

void gray_scale(eikon::Canvas &canvas) {
    eikon::Rectangle rec {150, 200, 100, 200, 0xFFFF00AB};

    canvas.fill(0xFF15FFA1)
        ->draw(rec)
        ->gray_scale();
}

void isolate(eikon::Canvas &canvas) {
    eikon::Triangle triangle {100, 100, 600, 200, 400, 500, 0xFFFFFFFF};

    canvas.fill(0xFF000000)
        ->draw(triangle)
        ->isolate(eikon::Channel::BLUE);
}

void line(eikon::Canvas &canvas) {
    eikon::Line l {0, 0, 800, 800, 0xFFFF00FF};

    canvas.fill(0xFF000000)
        ->draw(l);
}

void negate(eikon::Canvas &canvas) {
    
    eikon::Rectangle r1 = {100, 200, 200, 300, 0xFFFF00FF};
    eikon::Circle c1 = {75, 300, 300, 0x7FFF0000};
    eikon::Circle c2 = {150, 400, 400, 0xFF00FF00};

    canvas.fill(0xFF000000)
        ->draw(r1)
        ->draw(c1)
        ->draw(c2)
        ->negate();
}

void overlap(eikon::Canvas &canvas) {
    eikon::Circle c1 {100, 400, 400, 0xFF0000FF};
    eikon::Circle c2 {75, 300, 300, 0x7FFF0000};

    canvas.fill(0xFF000000)
        ->draw(c1)
        ->draw(c2);
}

void padding(eikon::Canvas &canvas) {
    eikon::Circle circle {100.0, 400, 400, 0xFFFFFFFF};

    canvas.fill(0xFF000000)
        ->draw(circle)
        ->padding(50, 10, 20, 100, 0xFFFF0000);
}

void raise_(eikon::Canvas &canvas) {
    eikon::Circle circle {100.0, 400, 400, 0xFFFF0000};

    canvas.fill(0xFF2F2F2F)
        ->draw(circle)
        ->raise(30);
}

void roll(eikon::Canvas &canvas) {
    eikon::Rectangle rec {50, 200, 100, 200, 0xFF15FFA1};
    
    canvas.fill(0xFF000000)
        ->draw(rec)
        ->roll(700);
}

void rotate_rectangle(eikon::Canvas &canvas) {
    eikon::Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};
    
    canvas.fill(0xFF000000)
        ->draw(rec)
        ->rotate();
}

void saturation(eikon::Canvas &canvas) {
    eikon::Circle circle {100.0, 400, 400, 0xFF00FF00};

    canvas.fill(0xFF000000)
        ->draw(circle)
        ->saturation(1.5);
}

void sepia(eikon::Canvas &canvas) {
    eikon::Rectangle rec {150, 200, 100, 200, 0xFFFF00AB};
    eikon::Circle circle {100, 400, 400, 0xFFFF0000};

    canvas.fill(0xFF15FFA1)
        ->draw(rec)
        ->draw(circle)
        ->sepia();
}

void solarize(eikon::Canvas &canvas) {
    eikon::Triangle triangle {100, 100, 600, 200, 400, 500, 0xFF00008F};
    eikon::Circle circle {100, 400, 400, 0xFF00C2D2};

    canvas.fill(0xFFFFFFFF)
        ->draw(triangle)
        ->draw(circle)
        ->solarize();
}

void stretch(eikon::Canvas &canvas) {
    eikon::Circle circle {100.0, 400, 400, 0xFFFFFFFF};

    canvas.fill(0xFF000000)
        ->draw(circle)
        ->stretch(2);
}

void text(eikon::Canvas &canvas) {
    eikon::Text txt {"hello, world!", 150, 200, 10, 0xFF00FF00};

    canvas.fill(0xFF000000)
        ->draw(txt);
}

void triangle(eikon::Canvas &canvas) {
    eikon::Triangle triangle {100, 100, 600, 200, 400, 500, 0xFFFF00EE};

    canvas.fill(0xFF000000)
        ->draw(triangle);
}
