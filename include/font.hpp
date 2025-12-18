#pragma once

#include <map>
#include <vector>

typedef unsigned int uint;

namespace eikon {
    
    using Glyph = std::vector<std::vector<bool>>;
    
    typedef struct {
        const std::map<char, Glyph> &glyphs;
        uint height;
        uint width;
    } Font;

    extern Font default_font;
}
