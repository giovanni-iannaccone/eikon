#include "../include/bmp.hpp"
#include "../include/utils.hpp"

void bmp::extract_signature(std::istream &file, uint8_t signature[]) {
    file.seekg(0);
    for (uint i = 0; i < bmp::signature_size; i++)
        signature[i] = get_byte(file);
}

void bmp::get_dimensions(std::istream &file, uint *height, uint *width) {
    file.seekg(18);

    *width  = le::get_bytes<uint>(file);
    *height = le::get_bytes<uint>(file);
}

bool bmp::is_valid_signature(std::istream &file) {
    const uint8_t expected_signature[bmp::signature_size] = {0x42, 0x4D};

    uint8_t signature[bmp::signature_size];
    bmp::extract_signature(file, signature);

    return memcmp(expected_signature, signature, bmp::signature_size) == 0;
}

bmp::Error bmp::read(std::istream &file, PixelBuffer &pixels) {
    if (!bmp::is_valid_signature(file))
        return bmp::Error::INVALID_SIGNATURE;

    bmp::read_header(file);

    BMPData bmpdata = bmp::read_info_header(
        file, &pixels.height, &pixels.width
    );

    if (pixels.height == 0 || pixels.width == 0)
        return bmp::Error::INVALID_SIZE;

    if (bmpdata.compression == bmp::Compression::NO_COMPRESSION)
        bmp::read_raw_data(file, pixels);
    else if (bmpdata.compression == bmp::Compression::RLE)
        bmp::read_rle_data(file, pixels);
    else 
        return bmp::Error::INVALID_COMPRESSION;

    return bmp::Error::NO_ERROR;
}

void bmp::read_header(std::istream &file) {
    le::get_bytes<uint32_t>(file);
    
    le::get_bytes<uint32_t>(file);
    le::get_bytes<uint32_t>(file);
}

BMPData bmp::read_info_header(std::istream &file, uint *height_ptr, uint *width_ptr) {
    BMPData bmpdata;

    le::get_bytes<uint32_t>(file);
    
    *width_ptr  = le::get_bytes<uint32_t>(file);
    *height_ptr = le::get_bytes<uint32_t>(file);

    bmpdata.planes    = le::get_bytes<uint16_t>(file);
    bmpdata.bit_count = le::get_bytes<uint16_t>(file);

    bmpdata.compression = le::get_bytes<uint32_t>(file);
    bmpdata.image_size  = le::get_bytes<uint32_t>(file);

    bmpdata.x_pixels_per_meter = le::get_bytes<uint32_t>(file);
    bmpdata.y_pixels_per_meter = le::get_bytes<uint32_t>(file);

    bmpdata.clr_used = le::get_bytes<uint32_t>(file);
    bmpdata.clr_important = le::get_bytes<uint32_t>(file);

    return bmpdata;
}

void bmp::read_raw_data(std::istream &file, PixelBuffer &pixels) {
    uint8_t r {}, g {}, b {};
    uint padding = (pixels.width * 3) % 4;

    for (uint y = pixels.height; y > 0; y--) {
        for (uint x = 0; x < pixels.width; x++) {
            b = get_byte(file);
            g = get_byte(file);
            r = get_byte(file);

            pixels[y - 1][x] = get_hex(r, g, b);
        }

        for (uint i = 0; i < padding; i++)
            get_byte(file);
    }
}

void bmp::read_rle_data(std::istream &file, PixelBuffer &pixels) {
    uint8_t r {}, g {}, b {};
    uint8_t times;

    for (uint y = pixels.height; y > 0; y--)
        for (uint x = 0; x < pixels.width; x++) {
            times = get_byte(file);

            b = get_byte(file);
            g = get_byte(file);
            r = get_byte(file);

            for (uint i = 0; i < times; i++)
                pixels[y - 1][x + i] = get_hex(r, g, b);
        }
}

bmp::Error bmp::save(std::ostream &file, const PixelBuffer &pixels, void *args) {
    BMPData *bmpdata = (BMPData *)args;

    if (pixels.height == 0 || pixels.width == 0)
        return Error::INVALID_SIZE;

    bmp::write_header(file, pixels.height, pixels.width);
    bmp::write_info_header(file, pixels.height, pixels.width, bmpdata);
    
    if (bmpdata != nullptr && bmpdata->compression == bmp::Compression::RLE)
        bmp::write_rle_data(file, pixels);
    else
        bmp::write_raw_data(file, pixels);
    
    return Error::NO_ERROR;
}

void bmp::write_header(std::ostream &file, uint height, uint width) {
    bmp::write_signature(file);

    const uint header_size = 0x36;

    const int file_size = header_size + 3 * height * width;
    le::write_as_bytes(file, file_size);
    le::write_as_bytes<uint32_t>(file, 0);

    le::write_as_bytes(file, header_size);
}

void bmp::write_info_header(std::ostream &file, uint height, uint width, BMPData *header) {
    le::write_as_bytes(file, 0x28);

    le::write_as_bytes(file, width);
    le::write_as_bytes(file, height);

    le::write_as_bytes(file, 0x180001);
    
    if (header != nullptr) {
        le::write_as_bytes(file, header->compression);
        le::write_as_bytes(file, 3 * height * width);

        le::write_as_bytes(file, header->x_pixels_per_meter);
        le::write_as_bytes(file, header->y_pixels_per_meter);

        le::write_as_bytes(file, header->clr_used);
        le::write_as_bytes(file, header->clr_important);

    } else {
        le::write_as_bytes(file, 0);
        le::write_as_bytes(file, 3 * height * width);

        le::write_as_bytes(file, 0);
        le::write_as_bytes(file, 0);

        le::write_as_bytes(file, 0);
        le::write_as_bytes(file, 0);
    }
}

void bmp::write_raw_data(std::ostream &file, const PixelBuffer &pixels) {
    uint8_t r {}, g {}, b {};
    uint padding = (pixels.width * 3) % 4;

    for (uint y = pixels.height; y > 0; y--) {
        for (uint x = 0; x < pixels.width; x++) {
            get_rgb(pixels[y - 1][x], r, g, b);

            write_byte(file, b);
            write_byte(file, g);
            write_byte(file, r);
        }

        for (uint i = 0; i < padding; i++)
            write_byte(file, 0);
    }
}

void bmp::write_rle_data(std::ostream &file, const PixelBuffer &pixels) {
    uint32_t color {};
    uint8_t times  {};

    for (uint y = pixels.height; y > 0; y--) {
        color = pixels[y - 1][0];
        times = 1;

        for (uint x = 1; x < pixels.width; x++)
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
