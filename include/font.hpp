#pragma once

#include <array>
#include <bits/stdc++.h>
#include <map>

const int LETTER_HEIGHT = 6;
const int LETTER_WIDTH  = 5;

using Glyph = std::array<std::array<bool, LETTER_WIDTH>, LETTER_HEIGHT>;

typedef struct {
    const std::map<char, Glyph> * const glyphs;
    uint height;
    uint width;
} Font;

extern std::map<char, Glyph> glyphs;
extern Font default_font;
