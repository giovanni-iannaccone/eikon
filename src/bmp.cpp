#include "../include/bmp.hpp"

const uint bmp::signature_size = 2;

void bmp::extract_signature(std::istream &file, char *signature) {
    file.seekg(0);
    for (int i = 0; i < bmp::signature_size; i++)
        signature[i] = get_byte(file);
}

bool bmp::is_valid_signature(std::istream &file) {
    const char expected_signature[bmp::signature_size] = {0x42, 0x4D};

    char signature[bmp::signature_size];
    bmp::extract_signature(file, signature);

    return memcmp(expected_signature, signature, bmp::signature_size) == 0;
}

bool bmp::read(std::istream &file, uint32_t **pixels, uint *height_ptr, uint *width_ptr) {
    if (!bmp::is_valid_signature(file))
        return false;

    bmp::read_header(file);
    BMPData bmpdata = bmp::read_info_header(
        file, height_ptr, width_ptr
    );

    if (bmpdata.compression == bmp::Compression::NO_COMPRESSION)
        bmp::read_raw_data(file, pixels, *height_ptr, *width_ptr);
    else 
        bmp::read_rle_data(file, pixels, *height_ptr, *width_ptr);

    return true;
}

void bmp::read_header(std::istream &file) {

}

BMPData bmp::read_info_header(std::istream &file, uint *height_ptr, uint *width_ptr) {
    BMPData bmpdata;

    bmpdata.size = get_dword(file);

    return bmpdata;
}

void bmp::read_raw_data(std::istream &file, uint32_t **pixels, const uint height, const uint width) {
    uint8_t r {}, g {}, b {};

    for (uint y = 0; y < height; y++)
        for (uint x = 0; x < width; x++) {
            b = get_byte(file);
            g = get_byte(file);
            r = get_byte(file);

            pixels[y][x] = get_hex(r, g, b);
        }
}

void bmp::read_rle_data(std::istream &file, uint32_t **pixels, const uint height, const uint width) {
    uint8_t r {}, g {}, b {};
    uint8_t times;

    for (uint y = 0; y < height; y++)
        for (uint x = 0; x < width; x++) {
            times = get_byte(file);

            b = get_byte(file);
            g = get_byte(file);
            r = get_byte(file);

            for (uint i = 0; i < times; i++)
                pixels[y][x + i] = get_hex(r, g, b);
        }
}

bool bmp::save(std::ostream &file, uint32_t **pixels, uint height, uint width, void *args) {
    BMPData *bmpdata = (BMPData *)args;

    bmp::write_header(file, height, width);
    bmp::write_info_header(file, height, width, bmpdata);

    if (bmpdata != nullptr && bmpdata->compression == bmp::Compression::NO_COMPRESSION)
        bmp::write_raw_data(file, pixels, height, width);
    else 
        bmp::write_rle_data(file, pixels, height, width);
    
    return true;
}

void bmp::write_header(std::ostream &file, uint height, uint width) {
    bmp::write_signature(file);

    const int file_size = 0;
    write_as_bytes(file, file_size);
} 

void bmp::write_info_header(std::ostream &file, uint height, uint width, BMPData *header) {
    if (header != nullptr) {
        // TODO: save header data to file
    } else {
        // TODO: save default data to file
    }
}

void bmp::write_raw_data(std::ostream &file, uint32_t **pixels, uint height, uint width) {
    uint8_t r {}, g {}, b {};

    for (uint y = 0; y < height; y++)
        for (uint x = 0; x < width; x++) {
            get_rgb(pixels[y][x], &r, &g, &b);

            write_as_bytes(file, b);
            write_as_bytes(file, g);
            write_as_bytes(file, r);
        }
}

void bmp::write_rle_data(std::ostream &file, uint32_t **pixels, uint height, uint width) {
    uint8_t r {}, g {}, b {};

    uint32_t color {};
    uint8_t times  {};

    for (uint y = 0; y < height; y++) {
        color = pixels[y][0];
        times = 0;

        for (uint x = 1; x < width; x++) {
            if (pixels[y][x] == color)
                times ++;
            else {
                get_rgb(color, &r, &g, &b);

                for (uint i = 0; i < times; i++) {
                    write_as_bytes(file, times);

                    write_as_bytes(file, b);
                    write_as_bytes(file, g);
                    write_as_bytes(file, r);
                }

                color = pixels[y][x];
                times = 0;
            }
        }
    }
}

void bmp::write_signature(std::ostream &file) {
    const char signature[bmp::signature_size] = {0x42, 0x4D};

    for (auto byte: signature)
        write_as_bytes(file, byte);
}