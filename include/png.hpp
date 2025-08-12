#pragma once

#if defined(__WIN32__) || defined(__WIN64__)
    #include <winsock2.h>
#else
    #include <arpa/inet.h>
#endif

#include <cstdint>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <mutex>
#include <vector>
#include <utility>
#include <zlib.h>

#include "utils.hpp"

#define CRC_SIZE 4
    
typedef enum ChunkType {
    CRITICAL,
    ANCILLIARY,
    UNKNOWN,
    NOT_CHUNK
};

typedef enum ColorType: char {
    GRAY_SCALE       = 0,
    RGB_TRIPLE       = 2,
    PLTE_INDEX       = 3,
    GRAY_SCALE_ALPGA = 4,
    RGB_TRIPLE_ALPHA = 6
};

typedef enum FilterType: char {
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

    u_int start;
    u_int size;
    std::string name;

    UnknownChunk(u_int start, u_int size, const std::string &name)
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

    uint32_t *pixels;
};

class IHDR: public Chunk {

public:
    size_t height;
    size_t width;

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
};

class PNGData {
    
public:
    IHDR ihdr;
    PLTE plte;
    IDAT idat;

    std::vector<AncilliaryChunk>    ancilliary_chunks;
    std::vector<UnknownChunk>       unknown_chunks;

    static std::mutex mtx;
    static PNGData *instance;

    PNGData();
    ~PNGData();
        
    static PNGData *get_instance();
    static PNGData get_data();
    
    void add_ancilliary_chunk(const AncilliaryChunk &ch);
    void add_unknown_chunk(const UnknownChunk &ch);

    AncilliaryChunk *get_ancilliary_chunk(const std::string& name);
    UnknownChunk *get_unknown_chunk(const std::string& name);
};

const int dimensions_pos    = 16;
const int signature_size    = 8;

ChunkType chunk_type(const std::string &chunk_name);

void encode_png();

void extract_signature(std::istream &file, int signature[]);

void get_byte(std::istream &file, char *dst);
void get_png_dimensions(std::istream &file, size_t *height, size_t *width);
u_int get_chunk_size(std::istream &file);

bool is_ancilliary_chunk(const std::string &chunk_name);
bool is_chunk_name(const std::string &buffer);
bool is_critical_chunk(const std::string &chunk_name);

bool is_valid_colortype_bitdepth_combination(char ct, char bd);
bool is_valid_signature(std::istream &file);

bool parse_ancilliary_chunk(std::istream &file, std::string chunk_name);
bool parse_critical_chunk(std::istream &file, std::string chunk);
bool parse_unknown_chunk(std::istream &file, std::string chunk_name);

bool parse_header(std::istream &file);
bool parse_idat(std::istream &file);
bool parse_plte(std::istream &file);

bool parse_png(std::istream &file);

bool read_png(std::istream &file, uint32_t pixels[], size_t *height_ptr, size_t *width_ptr);
void save_png(std::ostream &file, uint32_t pixels[], size_t height, size_t width, void *args = nullptr);

bool unfilter_line(std::string &line);
