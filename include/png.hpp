#pragma once

#if defined(__WIN32__) || defined(__WIN64__)
    #include <winsock2.h> 
#else
    #include <arpa/inet.h>
#endif

#include <cstdint>
#include <cstring>
#include <iostream>
#include <utility>

#include <zlib.h>

class Chunk {
public:
    size_t start;
    size_t size;
    std::string name;

    Chunk *next;

    Chunk(size_t start, size_t size, std::string name, Chunk *next)
        : start(start), size(size),
        name(name), next(next) {}
};

class PNGData {
private:
    Chunk *chunks;

public:
    size_t height;
    size_t width;

    ~PNGData() {
        Chunk* current = chunks;
        while (current) {
            Chunk* next = current->next;
            delete current;
            current = next;
        }
    }

    void add_chunk(Chunk *new_node) {
        if (!chunks) {
            chunks = new_node;
            return;
        } 

        Chunk *node = chunks;

        while (node->next != nullptr)
            node = node->next;
        
        node->next = new_node;
    }
};

const int dimensions_pos    = 16;
const int signature_size    = 8;

PNGData png;

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

bool is_valid_chunk(std::istream &file, const Chunk &ch) {
    return true;
}

bool is_valid_png(std::istream &file) {
    const int expected_signature[signature_size] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

    int signature[signature_size];
    extract_signature(file, signature);

    return memcmp(expected_signature, signature, signature_size) == 0;
}

bool parse_png(std::istream &file) {
    if (!is_valid_png(file))
        return false;

    Chunk *IHDR = new Chunk{8, 13, "IHDR", nullptr};
    png.add_chunk(IHDR);

    get_png_dimensions(file, &png.height, &png.width);

    int pos = file.tellg();
    char buffer[4];
    while (file.read(buffer, sizeof(buffer))) {

        if (
            strcasecmp(buffer, "gAMA") ||
            strcasecmp(buffer, "cHRM") ||
            strcasecmp(buffer, "pHYs") ||
            strcasecmp(buffer, "tIME") ||
            strcasecmp(buffer, "bKGD") ||
            strcasecmp(buffer, "IDAT") ||
            strcasecmp(buffer, "tEXt")
        ) {
            size_t chunk_size = get_size_by_prev(file);
            Chunk *ch = new Chunk(pos, chunk_size, buffer, nullptr);
            png.add_chunk(ch);
            if (!is_valid_chunk(file, *ch))
                return false;
        }

        pos += sizeof(buffer);
    }

    return true;
}

bool read_png(std::istream &file, uint32_t pixels[], size_t *height_ptr, size_t *width_ptr) {
    if (!parse_png(file))
        return false;

    decode_png(file, pixels);
    return true;
}

void save_png(std::ostream &file, uint32_t pixels[], size_t height, size_t width) {

}