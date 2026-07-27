#pragma once

#include <filesystem>
#include <memory>

#include "bmp.hpp"
#include "png.hpp"
#include "ppm.hpp"

namespace eikon {

namespace files {

    enum class Type {
        BMP,
        PNG,
        PPM
    };

    constexpr std::array names {
        "bmp",
        "png",
        "ppm"
    };
    
    constexpr files::Type detect_type(const std::filesystem::path& file)
    {
        const auto ext = file.extension();

        if (ext == ".bmp") return files::Type::BMP;
        if (ext == ".png") return files::Type::PNG;
        
        return files::Type::PPM;
    }
    
} // namespace files

using format_handler = std::function<std::unique_ptr<FormatHandler> (files::Type)>;

inline std::unique_ptr<eikon::FormatHandler> get_format_handler(eikon::files::Type ft)
{
    switch (ft) {
    case eikon::files::Type::BMP:
        return std::make_unique<eikon::bmp::Handler>();
        
    case eikon::files::Type::PNG:
        return std::make_unique<eikon::png::Handler>();
        
    default:
        return std::make_unique<eikon::ppm::Handler>();
    }
}

} // namespace eikon
