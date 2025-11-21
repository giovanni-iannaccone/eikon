#pragma once

#include <istream>

#include "pixels.hpp"

namespace eikon {
    
    class FormatData {};
    
    class FormatHandler {
    public:
        virtual ~FormatHandler() = default;
        
        virtual void extract_signature(std::istream &file, uint8_t signature[]) = 0;
        virtual bool is_valid_signature(std::istream &file) = 0;
        
        virtual int get_dimensions(std::istream &file, uint *height, uint *width) = 0;
        
        virtual int read(std::istream &file, PixelBuffer &pixels, FormatData *data = nullptr) = 0;
        virtual int save(std::ostream &file, const PixelBuffer &pixels, FormatData *data = nullptr) = 0;
    };
}
