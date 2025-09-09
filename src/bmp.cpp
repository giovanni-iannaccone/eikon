#include "../include/bmp.hpp"

const uint bmp::signature_size = 2;

void bmp::extract_signature(std::istream &file, char *signature) {
    file.seekg(0);
    for (uint i = 0; i < bmp::signature_size; i++)
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
    LE_get_bytes<uint32_t>(file);
    
    LE_get_bytes<uint32_t>(file);
    LE_get_bytes<uint32_t>(file);
}

BMPData bmp::read_info_header(std::istream &file, uint *height_ptr, uint *width_ptr) {
    BMPData bmpdata;

    LE_get_bytes<uint32_t>(file);
    
    *width_ptr = LE_get_bytes<uint32_t>(file);
    *height_ptr = LE_get_bytes<uint32_t>(file);
    
    bmpdata.planes = LE_get_bytes<uint16_t>(file);
    bmpdata.bit_count = LE_get_bytes<uint16_t>(file);

    bmpdata.compression = LE_get_bytes<uint32_t>(file);
    bmpdata.image_size = LE_get_bytes<uint32_t>(file);

    bmpdata.x_pixels_per_meter = LE_get_bytes<uint32_t>(file);
    bmpdata.y_pixels_per_meter = LE_get_bytes<uint32_t>(file);

    bmpdata.clr_used = LE_get_bytes<uint32_t>(file);
    bmpdata.clr_important = LE_get_bytes<uint32_t>(file);

    return bmpdata;
}

void bmp::read_raw_data(std::istream &file, uint32_t **pixels, const uint height, const uint width) {
    uint8_t r {}, g {}, b {};

    for (uint y = height; y > 0; y--)
        for (uint x = 0; x < width; x++) {
            b = get_byte(file);
            g = get_byte(file);
            r = get_byte(file);

            pixels[y - 1][x] = get_hex(r, g, b);
        }
}

void bmp::read_rle_data(std::istream &file, uint32_t **pixels, const uint height, const uint width) {
    uint8_t r {}, g {}, b {};
    uint8_t times;

    for (uint y = height; y > 0; y--)
        for (uint x = 0; x < width; x++) {
            times = get_byte(file);

            b = get_byte(file);
            g = get_byte(file);
            r = get_byte(file);

            for (uint i = 0; i < times; i++)
                pixels[y - 1][x + i] = get_hex(r, g, b);
        }
}

bool bmp::save(std::ostream &file, uint32_t **pixels, uint height, uint width, void *args) {
    BMPData *bmpdata = (BMPData *)args;

    bmp::write_header(file, height, width);
    bmp::write_info_header(file, height, width, bmpdata);
    
    if (bmpdata != nullptr && bmpdata->compression == bmp::Compression::RLE)
        bmp::write_rle_data(file, pixels, height, width);
    else
        bmp::write_raw_data(file, pixels, height, width);
    
    return true;
}

void bmp::write_header(std::ostream &file, uint height, uint width) {
    bmp::write_signature(file);

    const uint header_size = 0x36;

    const int file_size = header_size + 3 * height * width;
    LE_write_as_bytes(file, file_size);
    LE_write_as_bytes<uint32_t>(file, 0);

    LE_write_as_bytes(file, header_size);
}

void bmp::write_info_header(std::ostream &file, uint height, uint width, BMPData *header) {
    LE_write_as_bytes(file, 0x28);

    LE_write_as_bytes(file, width);
    LE_write_as_bytes(file, height);

    LE_write_as_bytes(file, 0x180001);
    
    if (header != nullptr) {
        LE_write_as_bytes(file, header->compression);
        LE_write_as_bytes(file, 3 * height * width);

        LE_write_as_bytes(file, header->x_pixels_per_meter);
        LE_write_as_bytes(file, header->y_pixels_per_meter);

        LE_write_as_bytes(file, header->clr_used);
        LE_write_as_bytes(file, header->clr_important);

    } else {
        LE_write_as_bytes(file, 0);
        LE_write_as_bytes(file, 3 * height * width);

        LE_write_as_bytes(file, 0);
        LE_write_as_bytes(file, 0);

        LE_write_as_bytes(file, 0);
        LE_write_as_bytes(file, 0);
    }
}

void bmp::write_raw_data(std::ostream &file, uint32_t **pixels, uint height, uint width) {
    uint8_t r {}, g {}, b {};

    for (uint y = height; y > 0; y--)
        for (uint x = 0; x < width; x++) {
            get_rgb(pixels[y - 1][x], &r, &g, &b);

            write_byte(file, b);
            write_byte(file, g);
            write_byte(file, r);
        }
}

void bmp::write_rle_data(std::ostream &file, uint32_t **pixels, uint height, uint width) {
    uint32_t color {};
    uint8_t times  {};

    for (uint y = height; y > 0; y--) {
        color = pixels[y - 1][0];
        times = 1;

        for (uint x = 1; x < width; x++)
            if (pixels[y - 1][x] == color) {
                times++;
            } else {
                write_repeated(file, color, times);

                color = pixels[y - 1][x];
                times = 1;
            }
    }
}

void bmp::write_signature(std::ostream &file) {
    const char signature[] = {0x42, 0x4D};

    for (const auto &byte: signature)
        write_byte(file, byte);
}