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

class Chunk {

public:
    int crc;

    bool is_valid() {

    }
};

class UnknownChunk: public Chunk {

public:

    size_t start;
    size_t size;
    std::string name;

    UnknownChunk(size_t start, size_t size, std::string name)
        : start(start), size(size), name(name) {}
};

class IDAT: public Chunk {

};

class IEND: public Chunk {

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
    std::vector<uint32_t> entries;

    ~PLTE() {
        delete[] &this->entries;
    }
};

class PNGData {

public:
    IHDR ihdr;
    PLTE plte;
    IDAT idat;
    IEND iend;

    std::vector<UnknownChunk> ancilliary_chunks;

    ~PNGData() {
        delete &this->ihdr;
        delete &this->plte;
        delete &this->idat;
        delete &this->iend;

        delete[] &this->ancilliary_chunks;
    }

    void add_chunk(const UnknownChunk &ch) {
        this->ancilliary_chunks.push_back(ch);
    }

    Chunk *get_chunk(const std::string& name) {
        for (auto &ch: this->ancilliary_chunks)
            if (ch.name == name)
                return &ch;
        
        return NULL;
    }
};

const int dimensions_pos    = 16;
const int signature_size    = 8;

PNGData *png;

void create_mock_png() {

}

void decode_png(std::istream &file, uint32_t pixels[]) {
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

void get_png_dimensions(std::istream &file, size_t *height, size_t *width) {
    file.seekg(dimensions_pos);

    file.read((char *)width, 4);
    file.read((char *)height, 4);

    *width = ntohl(*width);
    *height = ntohl(*height);
}

size_t get_size_by_prev(std::istream &file) {
    size_t size;

    file.seekg(-1, std::ios::cur);
    file.read((char *)size, 4);
    return ntohl(size);
}

bool is_chunk_name(char buffer[], int length) {
    for (int i = 0; i < length; i++)
        if (!isalpha(buffer[i]))
            return false;

    return true;
}

bool is_valid_signature(std::istream &file) {
    const int expected_signature[signature_size] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

    int signature[signature_size];
    extract_signature(file, signature);

    return memcmp(expected_signature, signature, signature_size) == 0;
}

bool parse_header(std::istream &file) {
    get_png_dimensions(file, &png->ihdr.height, &png->ihdr.width);

    if (png->ihdr.height <= 0 || png->ihdr.width <= 0)
        return false;

    get_byte(file, &png->ihdr.bitdepth);
    if (!in<char>(png->ihdr.bitdepth, {1, 2, 4, 6, 8, 16}))
        return false;

    get_byte(file, &png->ihdr.color_type);
    if (!in<char>(png->ihdr.color_type, {0, 2, 3, 4, 6}))

    get_byte(file, &png->ihdr.compression);
    get_byte(file, &png->ihdr.filter);
    get_byte(file, &png->ihdr.interlace);

    return png->ihdr.is_valid();
}

bool parse_idat(std::istream &file) {
    
}

bool parse_unknown_chunks(std::istream &file) {
    size_t pos = file.tellg();
    char buffer[5] = " ";

    while (file.read(buffer, sizeof(buffer) - 1)) {

        pos += sizeof(buffer) - 1;

        if (is_chunk_name(buffer, sizeof(buffer) - 1)) {
            size_t chunk_size = get_size_by_prev(file);
            UnknownChunk ch = UnknownChunk(pos, chunk_size, buffer);
            png->add_chunk(ch);
            
            if (!ch.is_valid())
                return false;

            pos += chunk_size;
            file.seekg(pos);
        }
    }
}

bool parse_png(std::istream &file) {
    if (!is_valid_signature(file))
        return false;

    if (!parse_header(file))
        return false;

    parse_idat(file);

    return parse_unknown_chunks(file);
}

bool read_png(std::istream &file, uint32_t pixels[], size_t *height_ptr, size_t *width_ptr) {
    if (!parse_png(file))
        return false;

    decode_png(file, pixels);
    
    *height_ptr = png->ihdr.height;
    *width_ptr  = png->ihdr.width;
    return true;
}

void save_png(std::ostream &file, uint32_t pixels[], size_t height, size_t width) {
    if (png == nullptr)
        create_mock_png();
    
    
}