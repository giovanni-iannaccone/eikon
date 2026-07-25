#pragma once

#include <cstdint>
#include <cstring>

#include "formats.hpp"
#include "pixels.hpp"

namespace eikon::ppm {
        
class Data final: public FormatData {};

class Handler final: public FormatHandler {
private:
    void write_header(std::ostream &file, const Size &size);
    void write_signature(std::ostream &file);

public:
    static constexpr uint signature_size = 2;

    enum Error: int {
        INVALID_SIGNATURE,
        INVALID_SIZE,
        NO_ERROR
    };

    Handler();
    
    void extract_signature(std::istream &file, uint8_t signature[]) override;
    bool is_valid_signature(std::istream &file) override;

    int get_dimensions(std::istream &file, Size &size) override;

    int read(std::istream &file, PixelBuffer &pixels, FormatData *data = nullptr) override;
    int save(std::ostream &file, const PixelBuffer &pixels, FormatData *data = nullptr) override;
};

} // namespace ppm
