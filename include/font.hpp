#pragma once

#include <bits/stdc++.h>
#include <map>

namespace eikon {
    
    using Glyph = std::vector<std::vector<bool>>;
    
    typedef struct {
        const std::map<char, Glyph> &glyphs;
        uint height;
        uint width;
    } Font;

    extern std::map<char, Glyph> glyphs;
    extern Font default_font;
}
