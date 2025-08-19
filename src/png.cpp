#include "../include/png.hpp"

bool Chunk::is_valid() {
    return true;
}

PLTE::~PLTE() {
    delete[] this->entries;
}

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

ChunkType chunk_type(const std::string &chunk_name) {
    if (is_critical_chunk(chunk_name))
        return ChunkType::CRITICAL;

    else if (is_ancilliary_chunk(chunk_name))
        return ChunkType::ANCILLIARY;

    else if (is_chunk_name(chunk_name))
        return ChunkType::UNKNOWN;

    else 
        return ChunkType::NOT_CHUNK;
}

void encode_png() {

}

void extract_signature(std::istream &file, int signature[]) {
    file.seekg(0);
    for (int i = 0; i < signature_size; i++)
        file.read(reinterpret_cast<char*>(signature + i), sizeof(unsigned char));
}

void get_byte(std::istream &file, char *dst) {
    file.read(reinterpret_cast<char*>(dst), sizeof(unsigned char));
}

u_int get_chunk_size(std::istream &file) {
    int size;

    file.seekg(-1, std::ios::cur);
    file.read((char *)&size, 4);

    file.read((char *)nullptr, 4);
    return ntohl(size);
}

void get_png_dimensions(std::istream &file, size_t *height, size_t *width) {
    file.seekg(dimensions_pos);

    file.read((char *)width, 4);
    file.read((char *)height, 4);

    *width = ntohl(*width);
    *height = ntohl(*height);
}

bool is_ancilliary_chunk(const std::string &chunk_name) {
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

bool is_chunk_name(const std::string &buffer) {
    for (int i = 0; i < buffer.length(); i++)
        if (!isalpha(buffer[i]))
            return false;

    return true;
}

bool is_critical_chunk(const std::string &chunk_name) {
    return in<std::string>(chunk_name, {
        "IHDR",
        "PLTE",
        "IDAT",
        "IEND"
    });
}

bool is_valid_colortype_bitdepth_combination(char ct, char bd) {
    
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

bool is_valid_signature(std::istream &file) {
    const int expected_signature[signature_size] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

    int signature[signature_size];
    extract_signature(file, signature);

    return memcmp(expected_signature, signature, signature_size) == 0;
}

bool parse_ancilliary_chunk(std::istream &file, std::string chunk_name) {
    return true;
}

bool parse_critical_chunk(std::istream &file, std::string chunk) {

	if (chunk == "IHDR")
        return parse_header(file);

	else if (chunk == "PLTE")
        return parse_plte(file);

	else
        return parse_idat(file);
}

bool parse_header(std::istream &file) {
    PNGData *png = PNGData::get_instance();
    get_png_dimensions(file, &png->ihdr.height, &png->ihdr.width);

    if (png->ihdr.height <= 0 || png->ihdr.width <= 0)
        return false;

    get_byte(file, &png->ihdr.bitdepth);
    if (!in<char>(png->ihdr.bitdepth, {1, 2, 4, 8, 16}))
        return false;

    get_byte(file, &png->ihdr.color_type);
    if (!in<char>(png->ihdr.color_type, {0, 2, 3, 4, 6}))
        return false;

    if (!is_valid_colortype_bitdepth_combination(png->ihdr.color_type, png->ihdr.bitdepth))
        return false;
    
    get_byte(file, &png->ihdr.compression);
    get_byte(file, &png->ihdr.filter);
    get_byte(file, &png->ihdr.interlace);

    return png->ihdr.is_valid();
}

bool parse_idat(std::istream &file) {
    std::string line;
    std::string previous;
    
    u_int idat_size = get_chunk_size(file);
    
	for (int i = 0; i < idat_size; i += line.length()) {
        previous.assign(line);

	    getline(file, line);
        if (!unfilter_line(line))
            return false;
	}

    return true;
}

bool parse_plte(std::istream &file) {
    char r {}, g {}, b {};
    u_int entries_rgb = get_chunk_size(file);

    if (entries_rgb % 3 != 0)
        return false;

    int entries = entries_rgb / 3;
    PNGData *png = PNGData::get_instance();
    png->plte.entries = new uint32_t[entries];

    for (int i = 0; i < entries; i++) {
        get_byte(file, &r);
        get_byte(file, &g);
        get_byte(file, &b);

        png->plte.entries[i] = get_hex(r, g, b);
    }

    return png->plte.is_valid();
}

bool parse_png(std::istream &file) {
    if (!is_valid_signature(file))
        return false;

    char buffer[5] {" "};
    bool success {false};

    while (file.read(buffer, sizeof(buffer) - 1)) {

        switch (chunk_type(buffer)) {
            case ChunkType::CRITICAL:
                success = parse_critical_chunk(file, buffer);
                break;
            
            case ChunkType::ANCILLIARY:
                success = parse_ancilliary_chunk(file, buffer);
                break;

            case ChunkType::UNKNOWN:
                success = parse_unknown_chunk(file, buffer);
                break;

            default:
                success = false;
        }

        if (!success)
            return success;
    }

    return true;
}

bool parse_unknown_chunk(std::istream &file, std::string chunk_name) {
    PNGData *png = PNGData::get_instance();
    size_t start = file.tellg();

    u_int chunk_size = get_chunk_size(file);
    UnknownChunk ch = UnknownChunk(start, chunk_size, chunk_name);
    png->add_unknown_chunk(ch);
    
    if (!ch.is_valid())
        return false;

    file.seekg(start + chunk_size + CRC_SIZE);
    return true;
}

bool read_png(std::istream &file, uint32_t pixels[], size_t *height_ptr, size_t *width_ptr) {
    PNGData *png = PNGData::get_instance();
    png->idat.pixels = pixels;

    if (!parse_png(file))
        return false;
    
    *height_ptr = png->ihdr.height;
    *width_ptr  = png->ihdr.width;
    return true;
}

void save_png(std::ostream &file, uint32_t pixels[], size_t height, size_t width, void *args) {
    PNGData *example_png = static_cast<PNGData *>(args);
}

bool unfilter_line(std::string &line, std::string &previous) {
    PNGData *png = PNGData::get_instance();

    switch (png->ihdr.filter) {
        case FilterType::NONE:
            return true;
            
        default:
            return false;
    }
}
