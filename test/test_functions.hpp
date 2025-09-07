#pragma once

#include <string>
#include <eikon/eikon.hpp>

int area(EikonCanvas *canvas, const std::string &out) {
    Circle circle {100.0, 400, 400, 0xFF0000FF};

    canvas->fill(0xFF000000)
        ->draw(circle)
        ->area(300, 300, 100, 100)
        ->flop();

    return canvas->save(out);
}

int blur(EikonCanvas *canvas, const std::string &out) {
    Circle circle {100.0, 400, 400, 0xFF00FF00};
    Triangle triangle {100, 100, 600, 200, 400, 500, 0xFFFFFF00};
    Text text {"eikon", 150, 200, 10, 0xFFFF5C00};

    return canvas->draw(triangle)
        ->draw(circle)
        ->draw(text)
        ->blur(5)
        ->save(out);
}

int brightness(EikonCanvas *canvas, const std::string &out) {
    Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};

    return canvas->fill(0xFF000000)
        ->draw(rec)
        ->brightness(1.5)
        ->save(out);
}

int circle(EikonCanvas *canvas, const std::string &out) {
    Circle circle {100.0, 400, 400, 0xFF0000FF};

    return canvas->fill(0xFF000000)
        ->draw(circle)
        ->save(out);
}

int contrast(EikonCanvas *canvas, const std::string &out) {
    Circle circle {100.0, 400, 400, 0xFF00007F};

    return canvas->fill(0xFF0F0F0F)
        ->draw(circle)
        ->contrast(1.5)
        ->save(out);
}

int ellipse(EikonCanvas *canvas, const std::string &out) {
    Ellipse ellipse {400, 400, 200, 100, 0xFF00FF00};

    return canvas->fill(0xFF000000)
        ->draw(ellipse)
        ->save(out);
}

int flip_rectangle(EikonCanvas *canvas, const std::string &out) {
    Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};

    return canvas->fill(0xFF000000)
        ->draw(rec)
        ->flip()
        ->save(out);
}

int flop_rectangle(EikonCanvas *canvas, const std::string &out) {
    Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};

    return canvas->fill(0xFF000000)
        ->draw(rec)
        ->flop()
        ->save(out);
}

int gray_scale(EikonCanvas *canvas, const std::string &out) {
    Rectangle rec {150, 200, 100, 200, 0xFFFF00AB};

    return canvas->fill(0xFF15FFA1)
        ->draw(rec)
        ->gray_scale()
        ->save(out);
}

int line(EikonCanvas *canvas, const std::string &out) {
    Line l {0, 0, 800, 800, 0xFFFF00FF};

    return canvas->fill(0xFF000000)
        ->draw(l)
        ->save(out);
}

int negate(EikonCanvas *canvas, const std::string &out) {
    canvas->fill(0xFF000000);

    Rectangle r1 = {100, 200, 200, 300, 0xFFFF00FF};
    Circle c1 = {75, 300, 300, 0x7FFF0000};
    Circle c2 = {150, 400, 400, 0xFF00FF00};

    return canvas->draw(r1)
        ->draw(c1)
        ->draw(c2)
        ->negate()
        ->save(out);
}

int overlap(EikonCanvas *canvas, const std::string &out) {
    canvas->fill(0xFF000000);

    Circle circle {100, 400, 400, 0xFF0000FF};
    canvas->draw(circle);

    circle = {75, 300, 300, 0x7FFF0000};
    canvas->draw(circle);

    return canvas->save(out);
}

int roll(EikonCanvas *canvas, const std::string &out) {
    Rectangle rec {50, 200, 100, 200, 0xFF15FFA1};
    
    return canvas->fill(0xFF000000)
        ->draw(rec)
        ->roll(100)
        ->save(out);
}

int rotate_rectangle(EikonCanvas *canvas, const std::string &out) {
    Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};
    
    return canvas->fill(0xFF000000)
        ->draw(rec)
        ->rotate()
        ->save(out);
}

int saturation(EikonCanvas *canvas, const std::string &out) {
    Circle circle {100.0, 400, 400, 0xFF00FF00};

    return canvas->fill(0xFF000000)
        ->draw(circle)
        ->saturation(1.5)
        ->save(out);
}

int sepia(EikonCanvas *canvas, const std::string &out) {
    Rectangle rec {150, 200, 100, 200, 0xFFFF00AB};
    Circle circle {100, 400, 400, 0xFFFF0000};

    return canvas->fill(0xFF15FFA1)
        ->draw(rec)
        ->draw(circle)
        ->sepia()
        ->save(out);
}

uint32_t **stretch(EikonCanvas *canvas, const std::string &out) {
    Circle circle {100.0, 400, 400, 0xFFFFFFFF};
    uint32_t **new_pixels;

    canvas->fill(0xFF000000)
        ->draw(circle)
        ->stretch(2, &new_pixels)
        ->save(out);

    return new_pixels;
}

int text(EikonCanvas *canvas, const std::string &out) {
    Text txt {"hello, world!", 150, 200, 10, 0xFF00FF00};

    return canvas->fill(0xFF000000)
        ->draw(txt)
        ->save(out);
}

int triangle(EikonCanvas *canvas, const std::string &out) {
    Triangle triangle {100, 100, 600, 200, 400, 500, 0xFFFF00EE};

    return canvas->fill(0xFF000000)
        ->draw(triangle)
        ->save(out);
}