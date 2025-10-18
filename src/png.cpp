#include "../include/png.hpp"
#include "../include/utils.hpp"

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

PNG::PNG() {};

PNG::ChunkType PNG::chunk_type(const std::string &chunk_name) {
    if (is_critical_chunk(chunk_name))
        return ChunkType::CRITICAL;

    else if (is_ancilliary_chunk(chunk_name))
        return ChunkType::ANCILLIARY;

    else if (is_chunk_name(chunk_name))
        return ChunkType::UNKNOWN;

    else 
        return ChunkType::NOT_CHUNK;
}

void PNG::encode() {

}

void PNG::extract_signature(std::istream &file, uint8_t signature[]) {
    file.seekg(0);
    for (int i = 0; i < PNG::signature_size; i++)
        signature[i] = get_byte(file);
}

uint PNG::get_chunk_size(std::istream &file) {
   uint size;

    file.seekg(-1, std::ios::cur);
    size = be::get_bytes<uint>(file);

    return be::get_bytes<uint>(file);
}

int PNG::get_dimensions(std::istream &file, uint *height, uint *width) {
    file.seekg(PNG::dimensions_pos);

    *width = be::get_bytes<uint>(file);
    *height = be::get_bytes<uint>(file);

    return Error::NO_ERROR;
}

bool PNG::is_ancilliary_chunk(const std::string &chunk_name) {
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

    bool valid = memcmp(expected_signature, signature, this->signature_size) == 0;
    delete[] signature;
    return valid;
}

int PNG::parse_ancilliary_chunk(std::istream &file, PNGData &png, std::string chunk_name) {
    return Error::NO_ERROR;
}

int PNG::parse_critical_chunk(std::istream &file, PNGData &png, std::string chunk) {

	if (chunk == "IHDR")
        return parse_header(file, png);

	else if (chunk == "PLTE")
        return parse_plte(file, png);

	else
        return parse_idat(file, png);
}

int PNG::parse_header(std::istream &file, PNGData &pngdata) {
    this->get_dimensions(file, &pngdata.ihdr.height, &pngdata.ihdr.width);

    if (pngdata.ihdr.height <= 0 || pngdata.ihdr.width <= 0)
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

    return pngdata.plte.is_valid()
        ? Error::NO_ERROR
        : Error::WRONG_CRC;
}

int PNG::parse_idat(std::istream &file, PNGData &pngdata) {
    std::string line;
    std::string previous;
    
    uint idat_size = this->get_chunk_size(file);
    
	for (int i = 0; i < idat_size; i += line.length()) {
        previous.assign(line);

	    getline(file, line);
        if (!unfilter_line(pngdata, line, previous))
            return Error::INVALID_FILTER;
	}

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

    return pngdata.plte.is_valid()
        ? Error::NO_ERROR
        : Error::WRONG_CRC;
}

int PNG::parse(std::istream &file, PNGData &pngdata) {
    if (!is_valid_signature(file))
        return Error::INVALID_SIGNATURE;

    char buffer[5] {" "};
    int success {Error::NO_ERROR};

    while (file.read(buffer, sizeof(buffer) - 1)) {

        switch (this->chunk_type(buffer)) {
            
        case ChunkType::CRITICAL:
            success = this->parse_critical_chunk(file, pngdata, buffer);
            break;
        
        case ChunkType::ANCILLIARY:
            success = this->parse_ancilliary_chunk(file, pngdata, buffer);
            break;

        case ChunkType::UNKNOWN:
            success = this->parse_unknown_chunk(file, pngdata, buffer);
            break;

        default:
            break;
        }

        if (success != Error::NO_ERROR)
            return success;
    }

    return Error::NO_ERROR;
}

int PNG::parse_unknown_chunk(std::istream &file, PNGData &pngdata, std::string chunk_name) {
    uint start = file.tellg();

    uint chunk_size = this->get_chunk_size(file);
    UnknownChunk ch = UnknownChunk(start, chunk_size, chunk_name);
    pngdata.add_unknown_chunk(ch);
    
    if (!ch.is_valid())
        return Error::WRONG_CRC;

    file.seekg(start + chunk_size + this->crc_size);
    return Error::NO_ERROR;
}

int PNG::read(std::istream &file, PixelBuffer &pixels, FormatData *data) {
    PNGData png;
    png.idat.pixels = pixels;

    int err = parse(file, png);
    if (err != Error::NO_ERROR)
        return err;
    
    pixels.height = png.ihdr.height;
    pixels.width  = png.ihdr.width;
    
    return pixels.height != 0 && pixels.width != 0
        ? Error::NO_ERROR
        : Error::INVALID_SIZE;
}

int PNG::save(std::ostream &file, const PixelBuffer &pixels, FormatData *data) {
    PNGData *example_png = static_cast<PNGData *>(data);
    return Error::NO_ERROR;
}

bool PNG::unfilter_line(PNGData &png, std::string &line, std::string &previous) {
    switch (png.ihdr.filter) {
    case FilterType::NONE:
        return true;
        
    default:
        return false;
    }
}
