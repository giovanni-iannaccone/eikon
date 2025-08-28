#include "../include/png.hpp"

bool Chunk::is_valid() {
    return true;
}

PLTE::~PLTE() {
    delete[] this->entries;
}

PNGData    *PNGData::instance = nullptr;
std::mutex  PNGData::mtx;

PNGData::PNGData() {};

PNGData::~PNGData() {
    delete this->instance;
    
    delete &this->ihdr;
    delete &this->plte;
    delete &this->idat;

    delete[] &this->ancilliary_chunks;
    delete[] &this->unknown_chunks;
}

PNGData *PNGData::get_instance() {
    if (instance == nullptr) {
        std::lock_guard<std::mutex> lock(mtx);
        if (instance == nullptr)
            instance = new PNGData();
    }
        
    return instance;
}

PNGData PNGData::get_data() {
    return *PNGData::get_instance();
}

void PNGData::add_ancilliary_chunk(const AncilliaryChunk &ch) {
    this->ancilliary_chunks.push_back(ch);
}

void PNGData::add_unknown_chunk(const UnknownChunk &ch) {
    this->unknown_chunks.push_back(ch);
}

AncilliaryChunk *PNGData::get_ancilliary_chunk(const std::string& name) {
    for (auto &ch: this->ancilliary_chunks)
        if (ch.name == name)
            return &ch;

    return nullptr;
}

UnknownChunk *PNGData::get_unknown_chunk(const std::string& name) {
    for (auto &ch: this->unknown_chunks)
        if (ch.name == name)
            return &ch;
        
    return nullptr;
}

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

void png::extract_signature(std::istream &file, int signature[]) {
    file.seekg(0);
    for (int i = 0; i < png::signature_size; i++)
        file.read(reinterpret_cast<char*>(signature + i), sizeof(unsigned char));
}

uint png::get_chunk_size(std::istream &file) {
    int size;

    file.seekg(-1, std::ios::cur);
    file.read((char *)&size, 4);

    file.read((char *)nullptr, 4);
    return ntohl(size);
}

void png::get_dimensions(std::istream &file, uint *height, uint *width) {
    file.seekg(png::dimensions_pos);

    file.read((char *)width, 4);
    file.read((char *)height, 4);

    *width = ntohl(*width);
    *height = ntohl(*height);
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
    for (uint i = 0; i < buffer.length(); i++)
        if (!isalpha(buffer[i]))
            return false;

    return true;
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
    const int expected_signature[png::signature_size] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

    int signature[png::signature_size];
    png::extract_signature(file, signature);

    return memcmp(expected_signature, signature, png::signature_size) == 0;
}

bool png::parse_ancilliary_chunk(std::istream &file, std::string chunk_name) {
    return true;
}

bool png::parse_critical_chunk(std::istream &file, std::string chunk) {

	if (chunk == "IHDR")
        return parse_header(file);

	else if (chunk == "PLTE")
        return parse_plte(file);

	else
        return parse_idat(file);
}

bool png::parse_header(std::istream &file) {
    PNGData *png = PNGData::get_instance();
    png::get_dimensions(file, &png->ihdr.height, &png->ihdr.width);

    if (png->ihdr.height <= 0 || png->ihdr.width <= 0)
        return false;

    png->ihdr.bitdepth = get_byte(file);
    if (!in<char>(png->ihdr.bitdepth, {1, 2, 4, 8, 16}))
        return false;

    png->ihdr.color_type = get_byte(file);
    if (!in<char>(png->ihdr.color_type, {0, 2, 3, 4, 6}))
        return false;

    if (!png::is_valid_colortype_bitdepth_combination(png->ihdr.color_type, png->ihdr.bitdepth))
        return false;
    
    png->ihdr.compression = get_byte(file);
    png->ihdr.filter = get_byte(file);
    png->ihdr.interlace = get_byte(file);

    return png->ihdr.is_valid();
}

bool png::parse_idat(std::istream &file) {
    std::string line;
    std::string previous;
    
    u_int idat_size = png::get_chunk_size(file);
    
	for (int i = 0; i < idat_size; i += line.length()) {
        previous.assign(line);

	    getline(file, line);
        if (!png::unfilter_line(line, previous))
            return false;
	}

    return true;
}

bool png::parse_plte(std::istream &file) {
    char r {}, g {}, b {};
    uint entries_rgb = png::get_chunk_size(file);

    if (entries_rgb % 3 != 0)
        return false;

    int entries = entries_rgb / 3;
    PNGData *png = PNGData::get_instance();
    png->plte.entries = new uint32_t[entries];

    for (int i = 0; i < entries; i++) {
        r = get_byte(file);
        g = get_byte(file);
        b = get_byte(file);

        png->plte.entries[i] = get_hex(r, g, b);
    }

    return png->plte.is_valid();
}

bool png::parse(std::istream &file) {
    if (!png::is_valid_signature(file))
        return false;

    char buffer[5] {" "};
    bool success {false};

    while (file.read(buffer, sizeof(buffer) - 1)) {

        switch (png::chunk_type(buffer)) {
            case ChunkType::CRITICAL:
                success = png::parse_critical_chunk(file, buffer);
                break;
            
            case ChunkType::ANCILLIARY:
                success = png::parse_ancilliary_chunk(file, buffer);
                break;

            case ChunkType::UNKNOWN:
                success = png::parse_unknown_chunk(file, buffer);
                break;

            default:
                success = false;
        }

        if (!success)
            return success;
    }

    return true;
}

bool png::parse_unknown_chunk(std::istream &file, std::string chunk_name) {
    PNGData *png = PNGData::get_instance();
    uint start = file.tellg();

    u_int chunk_size = png::get_chunk_size(file);
    UnknownChunk ch = UnknownChunk(start, chunk_size, chunk_name);
    png->add_unknown_chunk(ch);
    
    if (!ch.is_valid())
        return false;

    file.seekg(start + chunk_size + CRC_SIZE);
    return true;
}

bool png::read(std::istream &file, uint32_t **pixels, uint *height_ptr, uint *width_ptr) {
    PNGData *png = PNGData::get_instance();
    png->idat.pixels = pixels;

    if (!png::parse(file))
        return false;
    
    *height_ptr = png->ihdr.height;
    *width_ptr  = png->ihdr.width;
    return true;
}

bool png::save(std::ostream &file, uint32_t **pixels, uint height, uint width, void *args) {
    PNGData *example_png = static_cast<PNGData *>(args);
    return true;
}

bool png::unfilter_line(std::string &line, std::string &previous) {
    PNGData *png = PNGData::get_instance();

    switch (png->ihdr.filter) {
        case FilterType::NONE:
            return true;
            
        default:
            return false;
    }
}
