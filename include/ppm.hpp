#pragma once

#include <cstdint>
#include <cstring>

#include "formats.hpp"
#include "pixels.hpp"

class PPMData: public FormatData {};

class PPM: public FormatHandler {
private:
    void write_header(std::ostream &file, uint height, uint width);
    void write_signature(std::ostream &file);

public:
    const uint signature_size = 2;

    enum Error: int {
        INVALID_SIGNATURE,
        INVALID_SIZE,
        NO_ERROR
    };

    PPM();
    
    void extract_signature(std::istream &file, uint8_t signature[]) override;
    bool is_valid_signature(std::istream &file) override;

    int get_dimensions(std::istream &file, uint *height, uint *width) override;

    int read(std::istream &file, PixelBuffer &pixels, FormatData *data = nullptr) override;
    int save(std::ostream &file, const PixelBuffer &pixels, FormatData *data = nullptr) override;
};
