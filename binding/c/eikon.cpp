#include <functional>

#include <eikon/eikon.hpp>

#include "eikon.h"

Canvas *executor(Canvas *c, std::function<void (eikon::Canvas *)> &&func) {
    eikon::Canvas *canvas = reinterpret_cast<eikon::Canvas *>(c);
    func(canvas);
    return c;
}

EIKON_FUNC(add_noise, uint8_t intensity) {
    return executor(c, [intensity] (eikon::Canvas *canvas) {
        canvas->add_noise(intensity);
    });
}

EIKON_FUNC(ascii, uint scale) {
    return executor(c, [scale] (eikon::Canvas *canvas) {
        canvas->ascii(scale);
    });
}

EIKON_FUNC(blur, uint8_t radius) {
    return executor(c, [radius] (eikon::Canvas *canvas) {
        canvas->blur(radius);
    });
}

EIKON_FUNC(brightness, float inc) {
    return executor(c, [inc] (eikon::Canvas *canvas) {
        canvas->brightness(inc);
    });
}
    
EIKON_FUNC(chop, int cols) {
    return executor(c, [cols] (eikon::Canvas *canvas) {
        canvas->chop(cols);
    });
}

EIKON_FUNC(contrast, float inc) {
    return executor(c, [inc] (eikon::Canvas *canvas) {
        canvas->contrast(inc);
    });
}
    
EIKON_FUNC(crop, int rows) {
    return executor(c, [rows] (eikon::Canvas *canvas) {
        canvas->crop(rows);
    });
}

EIKON_FUNC(equalize) {
    return executor(c, [] (eikon::Canvas *canvas) {
        canvas->equalize();
    });
}
    
EIKON_FUNC(fill, uint32_t color) {
    return executor(c, [color] (eikon::Canvas *canvas) {
        canvas->fill(color);
    });
}

EIKON_FUNC(flip) {
    return executor(c, [] (eikon::Canvas *canvas) {
        canvas->flip();
    });
}

EIKON_FUNC(flop) {
    return executor(c, [] (eikon::Canvas *canvas) {
        canvas->flop();
    });
}

EIKON_FUNC(grayscale) {
    return executor(c, [] (eikon::Canvas *canvas) {
        canvas->gray_scale();
    });
}

EIKON_FUNC(hue, float inc) {
    return executor(c, [inc] (eikon::Canvas *canvas) {
        canvas->hue(inc);
    });
}

EIKON_FUNC(isolate, eikon::Channel ch) {
    return executor(c, [ch] (eikon::Canvas *canvas) {
        canvas->isolate(ch);
    });
}

EIKON_FUNC(map, void (*func)(uint32_t *)) {
    return executor(c, [func] (eikon::Canvas *canvas) {
        canvas->map([func] (uint32_t &pixel) {
            (*func)(&pixel);
        });
    });
}

EIKON_FUNC(negate) {
    return executor(c, [] (eikon::Canvas *canvas) {
        canvas->negate();
    });
}

EIKON_FUNC(padding, uint t, uint r, uint b, uint l, uint color) {
    return executor(c, [t, r, b, l, color] (eikon::Canvas *canvas) {
        canvas->padding(t, r, b, l, color);
    });
}

EIKON_FUNC(raise, uint border_width) {
    return executor(c, [border_width] (eikon::Canvas *canvas) {
        canvas->raise(border_width);
    });
}

EIKON_FUNC(read, char file_name[]) {
    return executor(c, [file_name] (eikon::Canvas *canvas) {
        canvas->read(file_name);
    });
}

EIKON_FUNC(roll, int col) {
    return executor(c, [col] (eikon::Canvas *canvas) {
        canvas->roll(col);
    });
}

EIKON_FUNC(rotate) {
    return executor(c, [] (eikon::Canvas *canvas) {
        canvas->rotate();
    });
}

EIKON_FUNC(saturation, float inc) {
    return executor(c, [inc] (eikon::Canvas *canvas) {
        canvas->saturation(inc);
    });
}

EIKON_FUNC(save, char file_name[]) {
    return executor(c, [file_name] (eikon::Canvas *canvas) {
        canvas->save(file_name);
    });
}

EIKON_FUNC(sepia) {
    return executor(c, [] (eikon::Canvas *canvas) {
        canvas->sepia();
    });
}

EIKON_FUNC(solarize, float perc) {
    return executor(c, [perc] (eikon::Canvas *canvas) {
        canvas->solarize(perc);
    });
}

EIKON_FUNC(stretch, uint size) {
    return executor(c, [size] (eikon::Canvas *canvas) {
        canvas->stretch(size);
    });
}

EIKON_FUNC(value, float inc) {
    return executor(c, [inc] (eikon::Canvas *canvas) {
        canvas->value(inc);
    });
}

Canvas *eikon_new_canvas_dim(uint h, uint b) {
    eikon::Canvas *c = new eikon::Canvas{h, b};
    return reinterpret_cast<Canvas *>(c);
}

Canvas *eikon_new_canvas_file(char file_name[]) {
    eikon::Canvas *c = new eikon::Canvas{file_name};
    return reinterpret_cast<Canvas *>(c);
}

uint32_t eikon_canvas_get_pixel(void *c, uint x, uint y) {
    eikon::Canvas *canvas = reinterpret_cast<eikon::Canvas *>(c);
    return canvas->at(x, y);
}

void eikon_canvas_set_pixel(Canvas *c, uint x, uint y, uint32_t value) {
    eikon::Canvas *canvas = reinterpret_cast<eikon::Canvas *>(c);
    canvas->at(x, y) = value;
}

void eikon_destroy_canvas(Canvas *c) {
    eikon::Canvas *canvas = reinterpret_cast<eikon::Canvas *>(c);
    delete canvas;
}

uint eikon_height(void *c) {
    eikon::Canvas *canvas = reinterpret_cast<eikon::Canvas *>(c);
    return canvas->height();
}

uint eikon_width(void *c) {
    eikon::Canvas *canvas = reinterpret_cast<eikon::Canvas *>(c);
    return canvas->width();
}

EIKON_FUNC(circle, float radius, uint cx, uint cy, uint32_t color) {
    return executor(c, [radius, cx, cy, color] (eikon::Canvas *canvas) {
        canvas->draw(eikon::Circle{radius, cx, cy, color});
    });
}

EIKON_FUNC(ellipse, uint cx, uint cy, uint a, uint b, uint32_t color) {
    return executor(c, [cx, cy, a, b, color] (eikon::Canvas *canvas) {
        canvas->draw(eikon::Ellipse{cx, cy, a, b, color});
    });
}

EIKON_FUNC(line, uint x1, uint y1, uint x2, uint y2, uint32_t color) {
    return executor(c, [x1, y1, x2, y2, color] (eikon::Canvas *canvas) {
        canvas->draw(eikon::Line{x1, y1, x2, y2, color});
    });
}

EIKON_FUNC(rectangle, uint x1, uint y1, uint h, uint b, uint32_t color) {
    return executor(c, [x1, y1, h, b, color] (eikon::Canvas *canvas) {
        canvas->draw(eikon::Rectangle{x1, y1, h, b, color});
    });
}

EIKON_FUNC(text, char *word, uint x1, uint y1, uint font_size, uint32_t color) {
    return executor(c, [word, x1, y1, font_size, color] (eikon::Canvas *canvas) {
        canvas->draw(eikon::Text{word, x1, y1, font_size, color});
    });
}

EIKON_FUNC(triangle, uint x1, uint y1, uint x2, uint y2, uint x3, uint y3, uint32_t color) {
    return executor(c, [x1, y1, x2, y2, x3, y3, color] (eikon::Canvas *canvas) {
        canvas->draw(eikon::Triangle{x1, y1, x2, y2, x3, y3, color});
    });
}
