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
#include <vector>
#include <utility>
#include <zlib.h>

#include "utils.hpp"

#define GRAY_SCALE          0
#define RGB_TRIPLE          2
#define PLTE_INDEX          3
#define GRAY_SCALE_ALPHA    4
#define RGB_TRIPLE_ALPHA    6

typedef enum ChunkType {
    CRITICAL,
    ANCILLIARY,
    UNKNOWN,
    NOT_CHUNK
};

class Chunk {

public:
    int crc;

    bool is_valid() {
        // TODO: controll validity with crc
        return true;
    }
};

class UnknownChunk: public Chunk {

public:

    size_t start;
    size_t size;
    std::string name;

    UnknownChunk(size_t start, size_t size, const std::string &name)
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
    uint32_t pixels[];
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

    ~PLTE() {
        delete[] this->entries;
    }
};

class PNGData {

public:
    IHDR ihdr;
    PLTE plte;
    IDAT idat;

    std::vector<AncilliaryChunk>    ancilliary_chunks;
    std::vector<UnknownChunk>       unknown_chunks;

    ~PNGData() {
        delete &this->ihdr;
        delete &this->plte;
        delete &this->idat;

        delete[] &this->ancilliary_chunks;
        delete[] &this->unknown_chunks;
    }

    void add_ancilliary_chunk(const AncilliaryChunk &ch) {
        this->ancilliary_chunks.push_back(ch);
    }

    void add_unknown_chunk(const UnknownChunk &ch) {
        this->unknown_chunks.push_back(ch);
    }

    AncilliaryChunk *get_ancilliary_chunk(const std::string& name) {
        for (auto &ch: this->ancilliary_chunks)
            if (ch.name == name)
                return &ch;
        
        return nullptr;
    }

    UnknownChunk *get_unknown_chunk(const std::string& name) {
        for (auto &ch: this->unknown_chunks)
            if (ch.name == name)
                return &ch;
        
        return nullptr;
    }
};

const int dimensions_pos    = 16;
const int signature_size    = 8;

PNGData *png = nullptr;

ChunkType chunk_type(const std::string &chunk_name) {
    if (is_critical_chunk(chunk_name))
        return ChunkType::CRITICAL;

    else if (is_ancilliary_chunk(chunk_name))
        return ChunkType::ANCILLIARY;

    else if (is_chunk_name(chunk_name))
        return ChunkType::UNKNOWN;

    else 
        return ChunkType::NOT_CHUNK;
}

void concatenate_all_idat_chunks() {

}

void create_mock_png() {

}

void decode_idat(std::istream &file, uint32_t pixels[]) {
    z_streamp idat;
    inflate(idat, Z_NO_FLUSH);
}

void encode_png() {
    
}

void extract_signature(std::istream &file, int signature[]) {
    file.seekg(0);
    for (int i = 0; i < signature_size; i++)
        file.read(reinterpret_cast<char*>(signature + i), sizeof(unsigned char));
}

void get_byte(std::istream &file, char *dst) {
    file.read(reinterpret_cast<char*>(dst), sizeof(unsigned char));
}

PNGData get_png_data() {
    return *png;
}

void get_png_dimensions(std::istream &file, size_t *height, size_t *width) {
    file.seekg(dimensions_pos);

    file.read((char *)width, 4);
    file.read((char *)height, 4);

    *width = ntohl(*width);
    *height = ntohl(*height);
}

int get_chunk_size(std::istream &file) {
    int size;

    file.seekg(-1, std::ios::cur);
    file.read((char *)size, 4);

    file.read((char *)nullptr, 4);
    return ntohl(size);
}

bool is_ancilliary_chunk(const std::string &chunk_name) {
    return in<std::string>(chunk_name, {
        "cHRM",
        "gAMA",
        "sBIT",
        "bKGD",
        "hIST",
        "tRNS",
        "pHYs",
        "tIME",
        "tEXt",
        "zTXt"
    });
}

bool is_chunk_name(const std::string &buffer) {
    for (int i = 0; i < buffer.length(); i++)
        if (!isalpha(buffer[i]))
            return false;

    return true;
}

bool is_critical_chunk(const std::string &chunk_name) {
    return in<std::string>(chunk_name, {
        "IHDR",
        "PLTE",
        "IDAT",
        "IEND"
    });
}

bool is_palette_image() {
    return png->ihdr.color_type == PLTE_INDEX;
}

bool is_valid_colortype_bitdepth_combination() {
    return (
        in<char>(png->ihdr.color_type, {2, 4, 6}) && 
        !in<char>(png->ihdr.bitdepth, {8, 16})
    ) || 
    (
        png->ihdr.color_type == 3 && 
        !in<char>(png->ihdr.bitdepth, {1, 2, 4, 8})
    ) ||
        png->ihdr.color_type == 0;
}

bool is_valid_signature(std::istream &file) {
    const int expected_signature[signature_size] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

    int signature[signature_size];
    extract_signature(file, signature);

    return memcmp(expected_signature, signature, signature_size) == 0;
}

bool parse_ancilliary_chunk(std::istream &file, std::string chunk_name) {

}

bool parse_header(std::istream &file) {
    get_png_dimensions(file, &png->ihdr.height, &png->ihdr.width);

    if (png->ihdr.height <= 0 || png->ihdr.width <= 0)
        return false;

    get_byte(file, &png->ihdr.bitdepth);
    if (!in<char>(png->ihdr.bitdepth, {1, 2, 4, 8, 16}))
        return false;

    get_byte(file, &png->ihdr.color_type);
    if (!in<char>(png->ihdr.color_type, {0, 2, 3, 4, 6}))
        return false;

    if (!is_valid_colortype_bitdepth_combination())
        return false;

    get_byte(file, &png->ihdr.compression);
    get_byte(file, &png->ihdr.filter);
    get_byte(file, &png->ihdr.interlace);

    return png->ihdr.is_valid();
}

bool parse_idat(std::istream &file) {
    concatenate_all_idat_chunks();
}

bool parse_plte(std::istream &file) {
    char r {}, g {}, b {};
    int entries_rgb = get_chunk_size(file);

    if (entries_rgb % 3 != 0)
        return false;

    int entries = entries_rgb / 3;
    png->plte.entries = new uint32_t[entries];

    for (int i = 0; i < entries; i++) {
        get_byte(file, &r);
        get_byte(file, &g);
        get_byte(file, &b);

        png->plte.entries[i] = get_hex(r, g, b);
    }

    return png->plte.is_valid();
}

bool parse_critical_chunk(std::istream &file, std::string chunk) {

    const std::map<std::string, std::function<bool (std::istream&)>> parsers = {
        {"IHDR", parse_header},
        {"PLTE", parse_plte},
        {"IDAT", parse_idat},
    };

    return parsers.at(chunk)(file);
}

bool parse_unknown_chunk(std::istream &file, std::string chunk_name) {
    size_t start = file.tellg();

    size_t chunk_size = get_chunk_size(file);
    UnknownChunk ch = UnknownChunk(start, chunk_size, chunk_name);
    png->add_unknown_chunk(ch);
    
    if (!ch.is_valid())
        return false;

    file.seekg(start + chunk_size);
}

bool parse_png(std::istream &file) {
    if (!is_valid_signature(file))
        return false;

    size_t pos = file.tellg();
    char buffer[5] = " ";
    bool success {false};

    while (file.read(buffer, sizeof(buffer) - 1)) {

        switch (chunk_type(buffer)) {
            case ChunkType::CRITICAL:
                success = parse_critical_chunk(file, buffer);
                break;
            
            case ChunkType::ANCILLIARY: 
                success = parse_ancilliary_chunk(file, buffer);
                break;

            case ChunkType::UNKNOWN:
                success = parse_unknown_chunk(file, buffer);

            default:
                success = false;
        }            

        if (!success)
            return success;
    }
    
    return true;
}

bool read_png(std::istream &file, uint32_t pixels[], size_t *height_ptr, size_t *width_ptr) {
    if (!parse_png(file))
        return false;

    pixels = png->idat.pixels;
    
    *height_ptr = png->ihdr.height;
    *width_ptr  = png->ihdr.width;
    return true;
}

void save_png(std::ostream &file, uint32_t pixels[], size_t height, size_t width) {
    if (png == nullptr)
        create_mock_png();
    
}