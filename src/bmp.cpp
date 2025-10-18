#include "../include/bmp.hpp"
#include "../include/utils.hpp"

BMP::BMP() {}

void BMP::extract_signature(std::istream &file, uint8_t signature[]) {
    file.seekg(0);
    for (uint i = 0; i < this->signature_size; i++)
        signature[i] = get_byte(file);
}

int BMP::get_dimensions(std::istream &file, uint *height, uint *width) {
    file.seekg(18);

    *width  = le::get_bytes<uint>(file);
    *height = le::get_bytes<uint>(file);

    return Error::NO_ERROR;
}

bool BMP::is_valid_signature(std::istream &file) {
    const uint8_t expected_signature[] = {0x42, 0x4D};

    uint8_t *signature = new uint8_t[this->signature_size];
    this->extract_signature(file, signature);

    bool valid = (memcmp(expected_signature, signature, this->signature_size) == 0);
    delete[] signature;
    return valid;
}

int BMP::read(std::istream &file, PixelBuffer &pixels, FormatData *data) {
    if (!this->is_valid_signature(file))
        return Error::INVALID_SIGNATURE;

    this->read_header(file);

    BMPData bmpdata = this->read_info_header(
        file, &pixels.height, &pixels.width
    );

    if (pixels.height == 0 || pixels.width == 0)
        return Error::INVALID_SIZE;

    if (bmpdata.compression == Compression::NO_COMPRESSION)
        this->read_raw_data(file, pixels);
    else if (bmpdata.compression == Compression::RLE)
        this->read_rle_data(file, pixels);
    else 
        return Error::INVALID_COMPRESSION;

    if (data != nullptr)
        *data = bmpdata;
    
    return Error::NO_ERROR;
}

void BMP::read_header(std::istream &file) {
    le::get_bytes<uint32_t>(file);
    
    le::get_bytes<uint32_t>(file);
    le::get_bytes<uint32_t>(file);
}

BMPData BMP::read_info_header(std::istream &file, uint *height_ptr, uint *width_ptr) {
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

void BMP::read_raw_data(std::istream &file, PixelBuffer &pixels) {
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

void BMP::read_rle_data(std::istream &file, PixelBuffer &pixels) {
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

int BMP::save(std::ostream &file, const PixelBuffer &pixels, FormatData *data) {
    BMPData *bmpdata = static_cast<BMPData *>(data);
    
    if (pixels.height == 0 || pixels.width == 0)
        return Error::INVALID_SIZE;

    this->write_header(file, pixels.height, pixels.width);
    this->write_info_header(file, pixels.height, pixels.width, bmpdata);
    
    if (bmpdata != nullptr && bmpdata->compression == Compression::RLE)
        this->write_rle_data(file, pixels);
    else
        this->write_raw_data(file, pixels);
    
    return Error::NO_ERROR;
}

void BMP::write_header(std::ostream &file, uint height, uint width) {
    this->write_signature(file);

    const uint header_size = 0x36;

    const int file_size = header_size + 3 * height * width;
    le::write_as_bytes(file, file_size);
    le::write_as_bytes<uint32_t>(file, 0);

    le::write_as_bytes(file, header_size);
}

void BMP::write_info_header(std::ostream &file, uint height, uint width, BMPData *bmp) {
    le::write_as_bytes(file, 0x28);

    le::write_as_bytes(file, width);
    le::write_as_bytes(file, height);

    le::write_as_bytes(file, 0x180001);
    
    if (bmp != nullptr) {
        le::write_as_bytes(file, bmp->compression);
        le::write_as_bytes(file, 3 * height * width);

        le::write_as_bytes(file, bmp->x_pixels_per_meter);
        le::write_as_bytes(file, bmp->y_pixels_per_meter);

        le::write_as_bytes(file, bmp->clr_used);
        le::write_as_bytes(file, bmp->clr_important);

    } else {
        le::write_as_bytes(file, 0);
        le::write_as_bytes(file, 3 * height * width);

        le::write_as_bytes(file, 0);
        le::write_as_bytes(file, 0);

        le::write_as_bytes(file, 0);
        le::write_as_bytes(file, 0);
    }
}

void BMP::write_raw_data(std::ostream &file, const PixelBuffer &pixels) {
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

void BMP::write_rle_data(std::ostream &file, const PixelBuffer &pixels) {
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

void BMP::write_signature(std::ostream &file) {
    const char signature[] = {0x42, 0x4D};

    for (const auto &byte: signature)
        write_byte(file, byte);
}
