#pragma once

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>
#include <utility>
#include <zlib.h>

#include "pixels.hpp"
#include "utils.hpp"
    
enum ChunkType {
    CRITICAL,
    ANCILLIARY,
    UNKNOWN,
    NOT_CHUNK
};

enum ColorType: char {
    GRAY_SCALE       = 0,
    RGB_TRIPLE       = 2,
    PLTE_INDEX       = 3,
    GRAY_SCALE_ALPGA = 4,
    RGB_TRIPLE_ALPHA = 6
};

enum FilterType: char {
    NONE  = 0,
    SUB   = 1,
    UP    = 2,
    AVG   = 3,
    PAETH = 4,
};

class Chunk {

public:
    int crc;

    bool is_valid();
};

class UnknownChunk: public Chunk {

public:

    const uint start;
    const uint size;
    const std::string name;

    UnknownChunk(uint start, uint size, const std::string &name)
        : start(start), size(size), name(name) {}
};

class AncilliaryChunk: public Chunk {

public:
    std::string name;

    AncilliaryChunk(const std::string &name)
        : name(name) {}
};

class IDAT: public Chunk {

public:
    int length;

    PixelBuffer pixels;
};

class IEND: public Chunk {

public:
    
};
    
class IHDR: public Chunk {

public:
    uint height;
    uint width;

    char bitdepth;
    char color_type;
    
    char compression;
    char filter;
    char interlace;
};

class PLTE: public Chunk {

public:
    uint32_t *entries;

    ~PLTE();

    uint32_t operator[](const uint idx) const;
    uint32_t &operator[](const uint idx);
};

class PNGData {
    
public:
    IHDR ihdr;
    PLTE plte;
    IDAT idat;
    IEND iend;

    std::vector<AncilliaryChunk>    ancilliary_chunks;
    std::vector<UnknownChunk>       unknown_chunks;

    PNGData();
    ~PNGData();
    
    void add_ancilliary_chunk(const AncilliaryChunk &ch);
    void add_unknown_chunk(const UnknownChunk &ch);

    AncilliaryChunk *get_ancilliary_chunk(const std::string& name);
    UnknownChunk *get_unknown_chunk(const std::string& name);
};

namespace png {
    enum Error: int {
        NO_ERROR,
        INVALID_BITDEPTH,
        INVALID_COLORTYPE,
        INVALID_COLORTYPE_BITDEPTH_COMBINATION,
        INVALID_FILTER,
        INVALID_LENGTH,
        INVALID_SIGNATURE,
        INVALID_SIZE,
        WRONG_CRC
    };

    extern const uint crc_size;
    extern const uint dimensions_pos;
    extern const uint signature_size;

    ChunkType chunk_type(const std::string &chunk_name);

    void encode();

    void extract_signature(std::istream &file, uint8_t signature[]);

    void get_dimensions(std::istream &file, uint *height, uint *width);
    uint get_chunk_size(std::istream &file);

    bool is_ancilliary_chunk(const std::string &chunk_name);
    bool is_chunk_name(const std::string &buffer);
    bool is_critical_chunk(const std::string &chunk_name);

    bool is_valid_colortype_bitdepth_combination(char ct, char bd);
    bool is_valid_signature(std::istream &file);

    png::Error parse_ancilliary_chunk(std::istream &file, PNGData &png, std::string chunk_name);
    png::Error parse_critical_chunk(std::istream &file, PNGData &png, std::string chunk);
    png::Error parse_unknown_chunk(std::istream &file, PNGData &png, std::string chunk_name);

    png::Error parse_header(std::istream &file, PNGData &png);
    png::Error parse_idat(std::istream &file, PNGData &png);
    png::Error parse_plte(std::istream &file, PNGData &png);

    png::Error parse(std::istream &file, PNGData &png);

    png::Error read(std::istream &file, PixelBuffer &pixels);
    png::Error save(std::ostream &file, const PixelBuffer &pixels, void *args = nullptr);

    bool unfilter_line(PNGData &png, std::string &line, std::string &previous);
}
