#include <cstdint>
#include <zlib.h>

#include "../include/png.hpp"
#include "../include/utils.hpp"

namespace eikon {
    
int paeth_predict(int a, int b, int c) { 
    int p = a + b - c;
    int pa = std::abs(p - a);
    int pb = std::abs(p - b);
    int pc = std::abs(p - c);

    return (pa <= pb && pa <= pc)
        ? a
        : (pb <= pc)
        ? b
        : c;
}

void filter::add::avg(std::string &line, const std::string &previous) {
    uint8_t a = 0, b = 0;
    
    for (size_t i = 0; i < line.size(); i++) {
        b = previous[i];
        line[i] -= (a + b) / 2;
        a = line[i];
    }
}

void filter::add::paeth(std::string &line, const std::string &previous) {
    uint8_t a = 0, b = 0, c = 0;
    
    for (size_t i = 0; i < line.size(); i++) {
        b = previous[i];
        line[i] -= paeth_predict(a, b, c);

        a = line[i];
        c = previous[i];
    }
}

void filter::add::sub(std::string &line) {
    uint8_t a = 0;

    for (size_t i = 0; i < line.size(); i++) {
        line[i] -= a;
        a = line[i];
    }
}

void filter::add::up(std::string &line, const std::string &previous) {
    uint8_t b {};

    for (size_t i = 0; i < line.size(); i++) {
        b = previous[i];
        line[i] -= b;
    }
}


void filter::remove::avg(std::string &line, const std::string &previous) {
    uint8_t a = 0, b = 0;
    
    for (size_t i = 0; i < line.size(); i++) {
        b = previous[i];
        line[i] += (a + b) / 2;
        a = line[i];
    }
}

void filter::remove::paeth(std::string &line, const std::string &previous) {
    uint8_t a = 0, b = 0, c = 0;
    
    for (size_t i = 0; i < line.size(); i++) {
        b = previous[i];
        line[i] += paeth_predict(a, b, c);

        a = line[i];
        c = previous[i];
    }
}

void filter::remove::sub(std::string &line) {
    uint8_t a = 0;

    for (size_t i = 0; i < line.size(); i++) {
        line[i] += a;
        a = line[i];
    }
}

void filter::remove::up(std::string &line, const std::string &previous) {
    uint8_t b {};

    for (size_t i = 0; i < line.size(); i++) {
        b = previous[i];
        line[i] += b;
    }
}

PLTE::~PLTE() {}

uint32_t PLTE::operator[](const uint idx) const {
    return this->entries[idx];
}

uint32_t &PLTE::operator[](const uint idx) {
    return this->entries[idx];
}

PNGData::PNGData() {}
PNGData::~PNGData() {}

PNG::PNG() {}

PNG::ChunkType PNG::chunk_type(const std::string &chunk_name) {
    if (is_critical_chunk(chunk_name))
        return ChunkType::CRITICAL;

    else if (is_chunk_name(chunk_name))
        return ChunkType::UNKNOWN;

    else 
        return ChunkType::NOT_CHUNK;
}

void PNG::encode() {}

void PNG::extract_signature(std::istream &file, uint8_t signature[]) {
    file.seekg(0);
    for (int i = 0; i < PNG::signature_size; i++)
        signature[i] = get_byte(file);
}

uint PNG::get_chunk_size(std::istream &file) {
    file.seekg(-1, std::ios::cur);
    return be::get_bytes<uint>(file);
}

int PNG::get_dimensions(std::istream &file, uint *height, uint *width) {
    file.seekg(this->dimensions_pos);

    *width = be::get_bytes<uint>(file);
    *height = be::get_bytes<uint>(file);

    return (*width == 0 || *height == 0)
        ? Error::INVALID_SIZE
        : Error::NO_ERROR;
}

int PNG::ignore_chunk(std::istream &file, const std::string &chunk_name) {
    uint start = file.tellg();
    
    uint chunk_size = this->get_chunk_size(file);
    file.seekg(start + chunk_size + this->crc_size);
    
    return Error::NO_ERROR;
}

bool PNG::is_chunk_name(const std::string &buffer) {
    return std::all_of(buffer.begin(), buffer.end(), ::isalpha);
}

bool PNG::is_critical_chunk(const std::string &chunk_name) {
    return in<std::string>(chunk_name, {
        "IHDR",
        "PLTE",
        "IDAT",
        "IEND"
    });
}

bool PNG::is_valid_colortype_bitdepth_combination(char ct, char bd) {
    
    return (
        in<char>(ct, {2, 4, 6}) && 
        !in<char>(bd, {8, 16})
    ) || 
    (
        ct == 3 && 
        !in<char>(bd, {1, 2, 4, 8})
    ) ||
        ct == 0;
}

bool PNG::is_valid_signature(std::istream &file) {
    const uint8_t expected_signature[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

    uint8_t *signature = new uint8_t[this->signature_size]; 
    this->extract_signature(file, signature);

    bool valid = memcmp(expected_signature, signature, sizeof(uint8_t) * this->signature_size) == 0;
    delete[] signature;
    return true;
}

int PNG::parse_critical_chunk(std::istream &file, PNGData &png, const std::string &chunk_name) {

	if (chunk_name == "IHDR")
        return parse_header(file, png);

	else if (chunk_name == "PLTE")
        return parse_plte(file, png);

	else
        return parse_idat(file, png);
}

int PNG::parse_header(std::istream &file, PNGData &pngdata) {
    if (this->get_dimensions(file, &pngdata.ihdr.height, &pngdata.ihdr.width) != Error::NO_ERROR)
        return Error::INVALID_SIZE;

    pngdata.ihdr.bitdepth = get_byte(file);
    if (!in<char>(pngdata.ihdr.bitdepth, {1, 2, 4, 8, 16}))
        return Error::INVALID_BITDEPTH;

    pngdata.ihdr.color_type = get_byte(file);
    if (!in<char>(pngdata.ihdr.color_type, {0, 2, 3, 4, 6}))
        return Error::INVALID_COLORTYPE;

    if (!this->is_valid_colortype_bitdepth_combination(pngdata.ihdr.color_type, pngdata.ihdr.bitdepth))
        return Error::INVALID_COLORTYPE_BITDEPTH_COMBINATION;;
    
    pngdata.ihdr.compression = get_byte(file);
    pngdata.ihdr.filter = get_byte(file);
    pngdata.ihdr.interlace = get_byte(file);

    return Error::NO_ERROR;
}

int PNG::parse_idat(std::istream &file, PNGData &pngdata) {    
    uint idat_size = this->get_chunk_size(file);

    uint8_t *bytes = new uint8_t[idat_size];
    
	for (uint i = 0; i < idat_size; i++)
        bytes[i] = get_byte(file);

    //z_stream zs {bytes, idat_size};
    //inflateInit(&zs);

    char *outbuffer = new char[pngdata.ihdr.height * pngdata.ihdr.width]; 
    std::string outstr;

    /*int ret;
    
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = inflate(&zs, 0);

        if (outstr.size() < zs.total_out)
        outstr.append(outbuffer, zs.total_out - outstr.size());
         
    } while (ret == Z_OK);
    
    inflateEnd(&zs);*/
    
    delete[] bytes;

    for (uint y = 0; y < pngdata.ihdr.height; y++)
        for (uint x = 0; x < pngdata.ihdr.width; x++)
            pngdata.idat.pixels->at(x, y) = outstr[y * pngdata.ihdr.width + x];
    
    return Error::NO_ERROR;
}

int PNG::parse_plte(std::istream &file, PNGData &pngdata) {
    char r {}, g {}, b {};
    uint entries_rgb = this->get_chunk_size(file);

    if (entries_rgb % 3 != 0)
        return Error::INVALID_LENGTH;

    uint entries = entries_rgb / 3;
    pngdata.plte.entries = new uint32_t[entries];

    for (uint i = 0; i < entries; i++) {
        r = get_byte(file);
        g = get_byte(file);
        b = get_byte(file);

        pngdata.plte.entries[i] = get_hex(r, g, b);
    }

    return Error::NO_ERROR;
}

int PNG::parse(std::istream &file, PNGData &pngdata) {
    if (!is_valid_signature(file))
        return Error::INVALID_SIGNATURE;
    
    char buffer[4] = "";
    int success = Error::NO_ERROR;

    while (file.read(buffer, sizeof(buffer))) {
        
        if (this->chunk_type(buffer) == ChunkType::CRITICAL) {
            success = this->parse_critical_chunk(file, pngdata, buffer);
            
            if (success != Error::NO_ERROR)
                return success;

        } else {
            this->ignore_chunk(file, buffer);
        }
    }

    return Error::NO_ERROR;
}

int PNG::read(std::istream &file, PixelBuffer &pixels, FormatData *data) {
    PNGData png;
    png.idat.pixels = &pixels;

    int err = this->parse(file, png);
    if (err != Error::NO_ERROR)
        return err;
    
    return pixels.height != 0 && pixels.width != 0
        ? Error::NO_ERROR
        : Error::INVALID_SIZE;
}

int PNG::save(std::ostream &file, const PixelBuffer &pixels, FormatData *data) {
    PNGData *example_png = static_cast<PNGData *>(data);
    return Error::NO_ERROR;
}

bool PNG::unfilter_line(PNGData &png, std::string &line, const std::string &previous) {
    switch (png.ihdr.filter) {
    case FilterType::NONE:
        break;

    case FilterType::SUB:
        filter::remove::sub(line);
        break;
        
    case FilterType::AVG:
        filter::remove::avg(line, previous);
        break;
        
    case FilterType::UP:
        filter::remove::up(line, previous);
        break;

    case FilterType::PAETH:
        filter::remove::paeth(line, previous);
        break;
        
    default:
        return false;
    }

    return true;
}

} // namespace eikon
