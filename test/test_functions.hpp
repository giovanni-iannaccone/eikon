#pragma once

#include <string>
#include <eikon/eikon.hpp>

void area(EikonCanvas &canvas) {
    Circle circle {100.0, 400, 400, 0xFF0000FF};

    canvas.fill(0xFF000000)
        ->draw(circle)
        ->area(300, 300, 100, 100)
        ->flop();
}

void blur(EikonCanvas &canvas) {
    Circle circle {100.0, 400, 400, 0xFF00FF00};
    Triangle triangle {100, 100, 600, 200, 400, 500, 0xFFFFFF00};
    Text text {"eikon", 150, 200, 10, 0xFFFF5C00};

    canvas.fill(0xFF000000)
        ->draw(triangle)
        ->draw(circle)
        ->draw(text)
        ->blur(5);
}

void brightness(EikonCanvas &canvas) {
    Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};

    canvas.fill(0xFF000000)
        ->draw(rec)
        ->brightness(1.5);
}

void chop(EikonCanvas &canvas) {
    Circle circle {100.0, 400, 400, 0xFFFF5C00};

    canvas.fill(0xFF000000)
        ->draw(circle)
        ->chop(-250);
}

void circle(EikonCanvas &canvas) {
    Circle circle {100.0, 400, 400, 0xFF0000FF};

    canvas.fill(0xFF000000)
        ->draw(circle);
}

void contrast(EikonCanvas &canvas) {
    Circle circle {100.0, 400, 400, 0xFF00007F};

    canvas.fill(0xFF0F0F0F)
        ->draw(circle)
        ->contrast(1.5);
}

void crop(EikonCanvas &canvas) {
    Circle circle {100.0, 400, 400, 0xFFFFFF00};

    canvas.fill(0xFF000000)
        ->draw(circle)
        ->crop(-250);
}

void ellipse(EikonCanvas &canvas) {
    Ellipse ellipse {400, 400, 200, 100, 0xFF00FF00};

    canvas.fill(0xFF000000)
        ->draw(ellipse);
}

void equalize(EikonCanvas &canvas) {
    Circle circle {100.0, 400, 400, 0xFF0000FF};
    Ellipse ellipse {300, 110, 200, 100, 0xFF00FF00};
    Triangle triangle {600, 200, 750, 200, 400, 500, 0xFFFFFF00};

    canvas.fill(0xFF000000)
        ->draw(circle)
        ->draw(ellipse)
        ->draw(triangle)
        ->equalize();
}

void flip_rectangle(EikonCanvas &canvas) {
    Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};

    canvas.fill(0xFF000000)
        ->draw(rec)
        ->flip();
}

void flop_rectangle(EikonCanvas &canvas) {
    Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};

    canvas.fill(0xFF000000)
        ->draw(rec)
        ->flop();
}

void gray_scale(EikonCanvas &canvas) {
    Rectangle rec {150, 200, 100, 200, 0xFFFF00AB};

    canvas.fill(0xFF15FFA1)
        ->draw(rec)
        ->gray_scale();
}

void isolate(EikonCanvas &canvas) {
    Triangle triangle {100, 100, 600, 200, 400, 500, 0xFFFFFFFF};

    canvas.fill(0xFF000000)
        ->draw(triangle)
        ->isolate(Channel::BLUE);
}

void line(EikonCanvas &canvas) {
    Line l {0, 0, 800, 800, 0xFFFF00FF};

    canvas.fill(0xFF000000)
        ->draw(l);
}

void negate(EikonCanvas &canvas) {
    
    Rectangle r1 = {100, 200, 200, 300, 0xFFFF00FF};
    Circle c1 = {75, 300, 300, 0x7FFF0000};
    Circle c2 = {150, 400, 400, 0xFF00FF00};

    canvas.fill(0xFF000000)
        ->draw(r1)
        ->draw(c1)
        ->draw(c2)
        ->negate();
}

void overlap(EikonCanvas &canvas) {
    Circle c1 {100, 400, 400, 0xFF0000FF};
    Circle c2 {75, 300, 300, 0x7FFF0000};

    canvas.fill(0xFF000000)
        ->draw(c1)
        ->draw(c2);
}

void padding(EikonCanvas &canvas) {
    Circle circle {100.0, 400, 400, 0xFFFFFFFF};

    canvas.fill(0xFF000000)
        ->draw(circle)
        ->padding(50, 10, 20, 100, 0xFFFF0000);
}

void raise_(EikonCanvas &canvas) {
    Circle circle {100.0, 400, 400, 0xFFFF0000};

    canvas.fill(0xFF2F2F2F)
        ->draw(circle)
        ->raise(30);
}

void roll(EikonCanvas &canvas) {
    Rectangle rec {50, 200, 100, 200, 0xFF15FFA1};
    
    canvas.fill(0xFF000000)
        ->draw(rec)
        ->roll(700);
}

void rotate_rectangle(EikonCanvas &canvas) {
    Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};
    
    canvas.fill(0xFF000000)
        ->draw(rec)
        ->rotate();
}

void saturation(EikonCanvas &canvas) {
    Circle circle {100.0, 400, 400, 0xFF00FF00};

    canvas.fill(0xFF000000)
        ->draw(circle)
        ->saturation(1.5);
}

void sepia(EikonCanvas &canvas) {
    Rectangle rec {150, 200, 100, 200, 0xFFFF00AB};
    Circle circle {100, 400, 400, 0xFFFF0000};

    canvas.fill(0xFF15FFA1)
        ->draw(rec)
        ->draw(circle)
        ->sepia();
}

void solarize(EikonCanvas &canvas) {
    Triangle triangle {100, 100, 600, 200, 400, 500, 0xFF00008F};
    Circle circle {100, 400, 400, 0xFF00C2D2};

    canvas.fill(0xFFFFFFFF)
        ->draw(triangle)
        ->draw(circle)
        ->solarize();
}

void stretch(EikonCanvas &canvas) {
    Circle circle {100.0, 400, 400, 0xFFFFFFFF};

    canvas.fill(0xFF000000)
        ->draw(circle)
        ->stretch(2);
}

void text(EikonCanvas &canvas) {
    Text txt {"hello, world!", 150, 200, 10, 0xFF00FF00};

    canvas.fill(0xFF000000)
        ->draw(txt);
}

void triangle(EikonCanvas &canvas) {
    Triangle triangle {100, 100, 600, 200, 400, 500, 0xFFFF00EE};

    canvas.fill(0xFF000000)
        ->draw(triangle);
}
