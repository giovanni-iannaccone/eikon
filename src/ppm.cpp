#include "../include/ppm.hpp"
#include "../include/utils.hpp"

PPM::PPM() {};

void PPM::extract_signature(std::istream &file, uint8_t signature[]) {
    file.seekg(0);
    for (uint i = 0; i < PPM::signature_size; i++)
        signature[i] = get_byte(file);

    file.seekg(1, file.cur);
}

int PPM::get_dimensions(std::istream &file, uint *height, uint *width) {
    file.seekg(PPM::signature_size);
    uint buffer;
    
    file >> *width >> *height >> buffer;
    file.seekg(1, file.cur);
    
    return Error::NO_ERROR;
}

bool PPM::is_valid_signature(std::istream &file) {
    uint8_t *signature = new uint8_t[this->signature_size];
    this->extract_signature(file, signature);
    
    bool valid = memcmp(signature, "P6", PPM::signature_size) == 0;
    delete[] signature;
    return valid;
}

int PPM::read(std::istream &file, PixelBuffer &pixels, FormatData *data) {
    if (!this->is_valid_signature(file))
        return Error::INVALID_SIGNATURE;
    
    this->get_dimensions(file, &pixels.height, &pixels.width);

    uint8_t r {}, g {}, b {};
    
    for (uint y = 0; y < pixels.height; y++) {
        for (uint x = 0; x < pixels.width; x++) {
            r = get_byte(file);
            g = get_byte(file);
            b = get_byte(file);
            
            pixels[y][x] = get_hex(r, g, b);
        }
    }

    return Error::NO_ERROR;
}

int PPM::save(std::ostream &file, const PixelBuffer &pixels, FormatData *data) {
    this->write_signature(file);

    if (pixels.height == 0 || pixels.width == 0)
        return Error::INVALID_SIZE;

    this->write_header(file, pixels.height, pixels.width);

    uint8_t r {}, g {}, b {};
    
    for (uint y = 0; y < pixels.height; y++) {
        for (uint x = 0; x < pixels.width; x++) {
            get_rgb(pixels[y][x], r, g, b);
            write_byte(file, r);
            write_byte(file, g);
            write_byte(file, b);
        }
    }

    return Error::NO_ERROR;
}

void PPM::write_header(std::ostream &file, uint height, uint width) {
    file << width << " " << height << "\n255\n";
}

void PPM::write_signature(std::ostream &file) {
    file << "P6\n";
}
