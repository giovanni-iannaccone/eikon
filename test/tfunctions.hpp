#pragma once

#include <string>

#include <eikon/eikon.hpp>
#include <eikon/shapes.hpp>

constexpr uint HEIGHT = 800;
constexpr uint WIDTH  = 800;

inline eikon::Canvas area()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFF0000FF};

    canvas.fill(0xFF000000)
        .draw(circle)
        .area(300, 300, 100, 100)
        .flop();

    return canvas;
}

inline eikon::Canvas blur()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFF00FF00};
    eikon::shapes::Triangle triangle {100, 100, 600, 200, 400, 500, 0xFFFFFF00};
    eikon::shapes::Text text {"eikon", 150, 200, 10, 0xFFFF5C00};

    canvas.fill(0xFF000000)
        .draw(triangle)
        .draw(circle)
        .draw(text)
        .blur(5);

    return canvas;
}

inline eikon::Canvas brightness()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    eikon::shapes::Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};

    canvas.fill(0xFF000000)
        .draw(rec)
        .brightness(1.5);

    return canvas;
}

inline eikon::Canvas chop()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFFFF5C00};

    canvas.fill(0xFF000000)
        .draw(circle)
        .chop(-250);

    return canvas;
}

inline eikon::Canvas circle()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFF0000FF};
    
    canvas.fill(0xFF000000)
        .draw(circle);

    return canvas;
}

inline eikon::Canvas concat()
{
    eikon::Canvas first {HEIGHT, WIDTH};
    
    eikon::shapes::SafeCircle c1 {75.0, 325, 500, 0xFFFF0000};
    eikon::shapes::SafeCircle c2 {75.0, 475, 500, 0xFFFF0000};
    
    eikon::shapes::Triangle triangle {250, 0, 550, 0, 400, 200, 0xFFFF0000};

    eikon::Canvas second {first.height(), first.width()};
    
    second.fill(0xFF000000)
        .draw(triangle);

    return first.fill(0xFF000000)
        .crop(-300)
        .draw(c1)
        .draw(c2)
        .concat(second, eikon::utils::Axis::Y);
}

inline eikon::Canvas contrast()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFF00007F};

    canvas.fill(0xFF0F0F0F)
        .draw(circle)
        .contrast(1.5);

    return canvas;
}

inline eikon::Canvas crop()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFFFFFF00};

    canvas.fill(0xFF000000)
        .draw(circle)
        .crop(-250);

    return canvas;
}

inline eikon::Canvas ellipse()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    eikon::shapes::Ellipse ellipse {400, 400, 200, 100, 0xFF00FF00};

    canvas.fill(0xFF000000)
        .draw(ellipse);

    return canvas;
}

inline eikon::Canvas equalize()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFF0000FF};
    eikon::shapes::Ellipse ellipse {300, 110, 200, 100, 0xFF00FF00};
    eikon::shapes::Triangle triangle {600, 200, 750, 200, 400, 500, 0xFFFFFF00};

    canvas.fill(0xFF000000)
        .draw(circle)
        .draw(ellipse)
        .draw(triangle)
        .equalize();

    return canvas;
}

inline eikon::Canvas flip_rectangle()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    eikon::shapes::Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};

    canvas.fill(0xFF000000)
        .draw(rec)
        .flip();

    return canvas;
}

inline eikon::Canvas flop_rectangle()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    eikon::shapes::Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};

    canvas.fill(0xFF000000)
        .draw(rec)
        .flop();

    return canvas;
}

inline eikon::Canvas gray_scale()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    eikon::shapes::Rectangle rec {150, 200, 100, 200, 0xFFFF00AB};

    canvas.fill(0xFF15FFA1)
        .draw(rec)
        .gray_scale();

    return canvas;
}

inline eikon::Canvas isolate()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    eikon::shapes::Triangle triangle {100, 100, 600, 200, 400, 500, 0xFFFFFFFF};

    canvas.fill(0xFF000000)
        .draw(triangle)
        .isolate(eikon::Channel::BLUE);

    return canvas;
}

inline eikon::Canvas line()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    eikon::shapes::Line l {0, 0, 800, 800, 0xFFFF00FF};

    canvas.fill(0xFF000000)
        .draw(l);

    return canvas;
}

inline eikon::Canvas negate()
{    
    eikon::Canvas canvas {HEIGHT, WIDTH};
    
    eikon::shapes::Rectangle r1 = {100, 200, 200, 300, 0xFFFF00FF};
    eikon::shapes::Circle c1 = {75, 300, 300, 0x7FFF0000};
    eikon::shapes::Circle c2 = {150, 400, 400, 0xFF00FF00};

    canvas.fill(0xFF000000)
        .draw(r1)
        .draw(c1)
        .draw(c2)
        .negate();

    return canvas;
}

inline eikon::Canvas overlap()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    
    eikon::shapes::Circle c1 {100, 400, 400, 0xFF0000FF};
    eikon::shapes::Circle c2 {75, 300, 300, 0x7FFF0000};

    canvas.fill(0xFF000000)
        .draw(c1)
        .draw(c2);

    return canvas;
}

inline eikon::Canvas padding()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFFFFFFFF};

    canvas.fill(0xFF000000)
        .draw(circle)
        .padding(50, 10, 20, 100, 0xFFFF0000);

    return canvas;
}

inline eikon::Canvas raise_()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFFFF0000};

    canvas.fill(0xFF2F2F2F)
        .draw(circle)
        .raise(30);

    return canvas;
}

inline eikon::Canvas roll()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    eikon::shapes::Rectangle rec {50, 200, 100, 200, 0xFF15FFA1};
    
    canvas.fill(0xFF000000)
        .draw(rec)
        .roll(700);

    return canvas;
}

inline eikon::Canvas rotate_rectangle()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    eikon::shapes::Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};
    
    canvas.fill(0xFF000000)
        .draw(rec)
        .rotate();

    return canvas;
}

inline eikon::Canvas saturation()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFF00FF00};

    canvas.fill(0xFF000000)
        .draw(circle)
        .saturation(1.5);

    return canvas;
}

inline eikon::Canvas sepia()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    
    eikon::shapes::Rectangle rec {150, 200, 100, 200, 0xFFFF00AB};
    eikon::shapes::Circle circle {100, 400, 400, 0xFFFF0000};

    canvas.fill(0xFF15FFA1)
        .draw(rec)
        .draw(circle)
        .sepia();

    return canvas;
}

inline eikon::Canvas solarize()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    
    eikon::shapes::Triangle triangle {100, 100, 600, 200, 400, 500, 0xFF00008F};
    eikon::shapes::Circle circle {100, 400, 400, 0xFF00C2D2};

    canvas.fill(0xFFFFFFFF)
        .draw(triangle)
        .draw(circle)
        .solarize();

    return canvas;
}

inline eikon::Canvas stretch()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};    
    eikon::shapes::Circle circle {100.0, 400, 400, 0xFFFFFFFF};

    canvas.fill(0xFF000000)
        .draw(circle)
        .stretch(2);

    return canvas;
}

inline eikon::Canvas text()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    eikon::shapes::Text txt {"hello, world!", 150, 200, 10, 0xFF00FF00};

    canvas.fill(0xFF000000)
        .draw(txt);

    return canvas;
}

inline eikon::Canvas triangle()
{
    eikon::Canvas canvas {HEIGHT, WIDTH};
    eikon::shapes::Triangle triangle {100, 100, 600, 200, 400, 500, 0xFFFF00EE};

    canvas.fill(0xFF000000)
        .draw(triangle);

    return canvas;
}
