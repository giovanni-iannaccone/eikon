#pragma once

#include <cstdint>
#include <cstring>

#include "formats.hpp"
#include "pixels.hpp"

namespace eikon::bmp {
        
class Data final: public FormatData {

public:
    uint16_t planes;
    uint16_t bit_count;

    uint compression;
    uint32_t image_size;
    
    uint32_t x_pixels_per_meter;
    uint32_t y_pixels_per_meter;

    uint32_t clr_used;
    uint32_t clr_important;
};

class Handler final: public FormatHandler {

private:
    void read_header(std::istream &file);
    Data read_info_header(std::istream &file, Size &size);

    void read_raw_data(std::istream &file, PixelBuffer &pixels);
    void read_rle_data(std::istream &file, PixelBuffer &pixels);

    void write_raw_data(std::ostream &file, const PixelBuffer &pixels);
    void write_rle_data(std::ostream &file, const PixelBuffer &pixels);

    void write_header(std::ostream &file, const Size &size);
    void write_info_header(std::ostream &file, const Size &size, Data *bmp);
    void write_signature(std::ostream &file);

public:
    enum Compression {
        NO_COMPRESSION,
        RLE
    };

    enum Error: int {
        NO_ERROR,
        INVALID_COMPRESSION,
        INVALID_SIGNATURE,
        INVALID_SIZE
    };

    static constexpr uint signature_size = 2;

    Handler();
    
    void extract_signature(std::istream &file, uint8_t signature[]) override;
    bool is_valid_signature(std::istream &file) override;

    int get_dimensions(std::istream &file, Size &size) override;
    
    int read(std::istream &file, PixelBuffer &pixels, FormatData *data = nullptr) override;
    int save(std::ostream &file, const PixelBuffer &pixels, FormatData *data = nullptr) override;
};

} // namespace bmp
