#include "../include/matrix_utils.hpp"

auto reverse_matrix(Canvas &canvas) -> void {
    for (size_t y = 0; y < canvas.height; y++)
        for(size_t x = 0; x < canvas.height/2; x++)
            std::swap(canvas.pixels[y*canvas.height + x], 
                canvas.pixels[y*canvas.height + canvas.height - x - 1]
            );
}

auto transpose_matrix(Canvas &canvas) -> void {
    for (size_t y = 0; y < canvas.height; y++) 
        for (size_t x = y + 1; x < canvas.width; x++)
            std::swap(canvas.pixels[y*canvas.width + x], 
                canvas.pixels[x*canvas.width + y]
            );
}