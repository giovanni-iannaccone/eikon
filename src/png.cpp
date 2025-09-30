#include "../include/png.hpp"

bool Chunk::is_valid() {
    return true;
}

PLTE::~PLTE() {
    delete[] this->entries;
}

uint32_t PLTE::operator[](const uint idx) const {
    return this->entries[idx];
}

uint32_t &PLTE::operator[](const uint idx) {
    return this->entries[idx];
}

PNGData::PNGData() {};

PNGData::~PNGData() {}

void PNGData::add_ancilliary_chunk(const AncilliaryChunk &ch) {
    this->ancilliary_chunks.push_back(ch);
}

void PNGData::add_unknown_chunk(const UnknownChunk &ch) {
    this->unknown_chunks.push_back(ch);
}

AncilliaryChunk *PNGData::get_ancilliary_chunk(const std::string &name) {
    for (auto &ch: this->ancilliary_chunks)
        if (ch.name == name)
            return &ch;

    return nullptr;
}

UnknownChunk *PNGData::get_unknown_chunk(const std::string &name) {
    for (auto &ch: this->unknown_chunks)
        if (ch.name == name)
            return &ch;
        
    return nullptr;
}

const uint png::crc_size          = 4;
const uint png::dimensions_pos    = 16;
const uint png::signature_size    = 8;

ChunkType png::chunk_type(const std::string &chunk_name) {
    if (is_critical_chunk(chunk_name))
        return ChunkType::CRITICAL;

    else if (is_ancilliary_chunk(chunk_name))
        return ChunkType::ANCILLIARY;

    else if (is_chunk_name(chunk_name))
        return ChunkType::UNKNOWN;

    else 
        return ChunkType::NOT_CHUNK;
}

void png::encode() {

}

void png::extract_signature(std::istream &file, uint8_t signature[]) {
    file.seekg(0);
    for (int i = 0; i < png::signature_size; i++)
        signature[i] = get_byte(file);
}

uint png::get_chunk_size(std::istream &file) {
   uint size;

    file.seekg(-1, std::ios::cur);
    size = be::get_bytes<uint>(file);

    return be::get_bytes<uint>(file);
}

void png::get_dimensions(std::istream &file, uint *height, uint *width) {
    file.seekg(png::dimensions_pos);

    *width = be::get_bytes<uint>(file);
    *height = be::get_bytes<uint>(file);
}

bool png::is_ancilliary_chunk(const std::string &chunk_name) {
    return in<std::string>(chunk_name, {
        "cHRM",
        "gAMA",
        "sBIT",
        "bKGD",
        "hIST",
        "tRNS",
        "pHYs",
        "tIME",
        "tEXt",
        "zTXt"
    });
}

bool png::is_chunk_name(const std::string &buffer) {
    return std::all_of(buffer.begin(), buffer.end(), ::isalpha);
}

bool png::is_critical_chunk(const std::string &chunk_name) {
    return in<std::string>(chunk_name, {
        "IHDR",
        "PLTE",
        "IDAT",
        "IEND"
    });
}

bool png::is_valid_colortype_bitdepth_combination(char ct, char bd) {
    
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

bool png::is_valid_signature(std::istream &file) {
    const uint8_t expected_signature[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

    uint8_t signature[png::signature_size]; 
    png::extract_signature(file, signature);

    return memcmp(expected_signature, signature, png::signature_size) == 0;
}
 
png::Error png::parse_ancilliary_chunk(std::istream &file, PNGData &png, std::string chunk_name) {
    return png::Error::NO_ERROR;
}

png::Error png::parse_critical_chunk(std::istream &file, PNGData &png, std::string chunk) {

	if (chunk == "IHDR")
        return parse_header(file, png);

	else if (chunk == "PLTE")
        return parse_plte(file, png);

	else
        return parse_idat(file, png);
}

png::Error png::parse_header(std::istream &file, PNGData &png) {
    png::get_dimensions(file, &png.ihdr.height, &png.ihdr.width);

    if (png.ihdr.height <= 0 || png.ihdr.width <= 0)
        return png::Error::INVALID_SIZE;

    png.ihdr.bitdepth = get_byte(file);
    if (!in<char>(png.ihdr.bitdepth, {1, 2, 4, 8, 16}))
        return png::Error::INVALID_BITDEPTH;

    png.ihdr.color_type = get_byte(file);
    if (!in<char>(png.ihdr.color_type, {0, 2, 3, 4, 6}))
        return png::Error::INVALID_COLORTYPE;

    if (!png::is_valid_colortype_bitdepth_combination(png.ihdr.color_type, png.ihdr.bitdepth))
        return png::Error::INVALID_COLORTYPE_BITDEPTH_COMBINATION;;
    
    png.ihdr.compression = get_byte(file);
    png.ihdr.filter = get_byte(file);
    png.ihdr.interlace = get_byte(file);

    return png.plte.is_valid()
        ? png::Error::NO_ERROR
        : png::Error::WRONG_CRC;
}

png::Error png::parse_idat(std::istream &file, PNGData &png) {
    std::string line;
    std::string previous;
    
    uint idat_size = png::get_chunk_size(file);
    
	for (int i = 0; i < idat_size; i += line.length()) {
        previous.assign(line);

	    getline(file, line);
        if (!png::unfilter_line(png, line, previous))
            return png::Error::INVALID_FILTER;
	}

    return png::Error::NO_ERROR;
}

png::Error png::parse_plte(std::istream &file, PNGData &png) {
    char r {}, g {}, b {};
    uint entries_rgb = png::get_chunk_size(file);

    if (entries_rgb % 3 != 0)
        return png::Error::INVALID_LENGTH;

    uint entries = entries_rgb / 3;
    png.plte.entries = new uint32_t[entries];

    for (uint i = 0; i < entries; i++) {
        r = get_byte(file);
        g = get_byte(file);
        b = get_byte(file);

        png.plte.entries[i] = get_hex(r, g, b);
    }

    return png.plte.is_valid()
        ? png::Error::NO_ERROR
        : png::Error::WRONG_CRC;
}

png::Error png::parse(std::istream &file, PNGData &png) {
    if (!png::is_valid_signature(file))
        return png::Error::INVALID_SIGNATURE;

    char buffer[5] {" "};
    png::Error success {png::Error::NO_ERROR};

    while (file.read(buffer, sizeof(buffer) - 1)) {

        switch (png::chunk_type(buffer)) {
            
        case ChunkType::CRITICAL:
            success = png::parse_critical_chunk(file, png, buffer);
            break;
        
        case ChunkType::ANCILLIARY:
            success = png::parse_ancilliary_chunk(file, png, buffer);
            break;

        case ChunkType::UNKNOWN:
            success = png::parse_unknown_chunk(file, png, buffer);
            break;

        }

        if (success != png::Error::NO_ERROR)
            return success;
    }

    return png::Error::NO_ERROR;
}

png::Error png::parse_unknown_chunk(std::istream &file, PNGData &png, std::string chunk_name) {
    uint start = file.tellg();

    uint chunk_size = png::get_chunk_size(file);
    UnknownChunk ch = UnknownChunk(start, chunk_size, chunk_name);
    png.add_unknown_chunk(ch);
    
    if (!ch.is_valid())
        return png::Error::WRONG_CRC;

    file.seekg(start + chunk_size + png::crc_size);
    return png::Error::NO_ERROR;
}

png::Error png::read(std::istream &file, uint32_t **pixels, uint *height_ptr, uint *width_ptr) {
    PNGData png;
    png.idat.pixels = pixels;

    png::Error err = png::parse(file, png);
    if (err != png::Error::NO_ERROR)
        return err;
    
    *height_ptr = png.ihdr.height;
    *width_ptr  = png.ihdr.width;
    
    return *height_ptr != 0 && *width_ptr != 0
        ? png::Error::NO_ERROR
        : png::Error::INVALID_SIZE;
}

png::Error png::save(std::ostream &file, uint32_t **pixels, uint height, uint width, void *args) {
    PNGData *example_png = static_cast<PNGData *>(args);
    return png::Error::NO_ERROR;
}

bool png::unfilter_line(PNGData &png, std::string &line, std::string &previous) {
    switch (png.ihdr.filter) {
    case FilterType::NONE:
        return true;
        
    default:
        return false;
    }
}
