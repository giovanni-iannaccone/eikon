#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "bmp.hpp"
#include "formats.hpp"
#include "png.hpp"
#include "ppm.hpp"

namespace eikon::files {
    enum class Type {
        BMP,
        PNG,
        PPM
    };
    
    static const std::string get_ext(const std::string &file) {
        int i = file.length() - 1;
        
        while (i >= 0 && file.at(i) != '.')
            i--;
        
        return (i >= 0)
            ? std::string{file.begin() + i + 1, file.end()}
            : "";
    }
    
    inline files::Type detect_type(const std::string &file_name) {
        const std::string ext = get_ext(file_name);
        
        const std::unordered_map<std::string, files::Type> exts = {
            {"bmp", files::Type::BMP},
            {"png", files::Type::PNG},
            {"ppm", files::Type::PPM}
        };
        
        return exts.at(ext);
    }

} // namespace files

inline std::unique_ptr<eikon::FormatHandler> get_format_handler(eikon::files::Type ft) {
    switch (ft) {
    case eikon::files::Type::BMP:
        return std::make_unique<eikon::bmp::Handler>();
        
    case eikon::files::Type::PNG:
        return std::make_unique<eikon::png::Handler>();
        
    default:
        return std::make_unique<eikon::ppm::Handler>();
    }
}
