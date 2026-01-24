#ifndef _EIKON_H_
#define _EIKON_H_

#include <stdint.h>

#define EIKON_FUNC(func_name, ...) Canvas *eikon_ ## func_name(Canvas *c, ## __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

    typedef void Canvas;
    typedef unsigned int uint;

    enum Channel: int {
        BLUE = 0,
        GREEN = 1,
        RED = 2
    };
    
    Canvas *eikon_new_canvas_dim(uint h, uint b);
    Canvas *eikon_new_canvas_file(char file_name[]);

    void eikon_destroy_canvas(Canvas *c);
    
    uint32_t eikon_canvas_get_pixel(Canvas *c, uint x, uint y);
    void eikon_canvas_set_pixel(Canvas *c, uint x, uint y, uint32_t value);
    
    EIKON_FUNC(ascii, uint scale);
    EIKON_FUNC(map, void (*func)(uint32_t *));
    
    uint eikon_height(Canvas *c);
    uint eikon_width(Canvas *c);

    EIKON_FUNC(fill, uint32_t color);
    EIKON_FUNC(flip);
    EIKON_FUNC(flop);
    EIKON_FUNC(padding, uint t, uint r, uint b, uint l, uint color);
    EIKON_FUNC(roll, int col);
    EIKON_FUNC(rotate);
    EIKON_FUNC(stretch, uint size);

    EIKON_FUNC(chop, int cols);
    EIKON_FUNC(crop, int rows);
    
    EIKON_FUNC(brightness, float inc);
    EIKON_FUNC(contrast, float inc);
    EIKON_FUNC(equalize);    
    EIKON_FUNC(grayscale);
    EIKON_FUNC(negate);
    
    EIKON_FUNC(hue, float inc);
    EIKON_FUNC(saturation, float inc);
    EIKON_FUNC(value, float inc);
        
    EIKON_FUNC(add_noise, uint8_t intensity);    
    EIKON_FUNC(blur, uint8_t radius);
    EIKON_FUNC(raise, uint border_width);
    
    EIKON_FUNC(isolate, enum Channel ch);
    EIKON_FUNC(sepia);
    EIKON_FUNC(solarize, float perc);

    EIKON_FUNC(read, char file_name[]);
    EIKON_FUNC(save, char file_name[]);
    
    EIKON_FUNC(circle, float radius, uint cx, uint cy, uint32_t color);
    EIKON_FUNC(ellipse, uint cx, uint cy, uint a, uint b, uint32_t color);
    EIKON_FUNC(line, uint x1, uint y1, uint x2, uint y2, uint32_t color);
    EIKON_FUNC(rectangle, uint x1, uint y1, uint h, uint b, uint32_t color);
    EIKON_FUNC(text, char *word, uint x1, uint y1, uint font_size, uint32_t color);
    EIKON_FUNC(triangle, uint x1, uint y1, uint x2, uint y2, uint x3, uint y3, uint32_t color);

    EIKON_FUNC(custom_shape, void (*func)(uint32_t **, void *), void *args);
    
#ifdef __cplusplus
}
#endif

#endif // _EIKON_H_
