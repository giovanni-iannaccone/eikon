#include "../include/bmp.hpp"
#include "../include/utils.hpp"

namespace eikon::bmp {
        
Handler::Handler() {}

void Handler::extract_signature(std::istream &file, uint8_t signature[])
{
    file.seekg(0);
    for (uint i = 0; i < this->signature_size; i++)
        signature[i] = utils::get_byte(file);
}

int Handler::get_dimensions(std::istream &file, Size &size)
{
    file.clear();
    file.seekg(18, std::ios::beg);
    
    size.width  = std::abs(utils::le::get_bytes<int>(file));
    size.height = std::abs(utils::le::get_bytes<int>(file));

    return Error::NO_ERROR;
}

bool Handler::is_valid_signature(std::istream &file)
{
    const uint8_t expected_signature[] = {0x42, 0x4D};

    uint8_t *signature = new uint8_t[this->signature_size];
    this->extract_signature(file, signature);

    bool valid = (memcmp(expected_signature, signature, this->signature_size) == 0);
    delete[] signature;
    return valid;
}

int Handler::read(std::istream &file, PixelBuffer &pixels, FormatData *data)
{
    if (!this->is_valid_signature(file))
        return Error::INVALID_SIGNATURE;

    this->read_header(file);

    Data bmpdata = this->read_info_header(
        file, pixels.size
    );

    if (pixels.size.has_any_zero())
        return Error::INVALID_SIZE;

    if (bmpdata.compression == Compression::NO_COMPRESSION)
        this->read_raw_data(file, pixels);
    else if (bmpdata.compression == Compression::RLE)
        this->read_rle_data(file, pixels);
    else 
        return Error::INVALID_COMPRESSION;

    if (data != nullptr)
        *data = bmpdata;
    
    return Error::NO_ERROR;
}

void Handler::read_header(std::istream &file)
{
    utils::le::get_bytes<uint32_t>(file);    
    utils::le::get_bytes<uint32_t>(file);
    utils::le::get_bytes<uint32_t>(file);
}

Data Handler::read_info_header(std::istream &file, Size &size)
{
    Data bmpdata;

    utils::le::get_bytes<uint32_t>(file);
    
    size.width  = std::abs(utils::le::get_bytes<int>(file));
    size.height = std::abs(utils::le::get_bytes<int>(file));

    bmpdata.planes    = utils::le::get_bytes<uint16_t>(file);
    bmpdata.bit_count = utils::le::get_bytes<uint16_t>(file);

    bmpdata.compression = utils::le::get_bytes<uint32_t>(file);
    bmpdata.image_size  = utils::le::get_bytes<uint32_t>(file);

    bmpdata.x_pixels_per_meter = utils::le::get_bytes<uint32_t>(file);
    bmpdata.y_pixels_per_meter = utils::le::get_bytes<uint32_t>(file);

    bmpdata.clr_used = utils::le::get_bytes<uint32_t>(file);
    bmpdata.clr_important = utils::le::get_bytes<uint32_t>(file);

    return bmpdata;
}

void Handler::read_raw_data(std::istream &file, PixelBuffer &pixels) {
    uint8_t r, g, b;
    uint padding = (4 - ((pixels.width() * 3) % 4)) % 4;

    char *buff = new char[pixels.width() * 3];
    
    for (uint y = pixels.height(); y > 0; y--) {
        file.read(buff, pixels.width() * 3);

        for (uint x = 0; x < pixels.width(); x++) {
            b = buff[x*3];
            g = buff[x*3 + 1];
            r = buff[x*3 + 2];
            
            pixels[y - 1][x] = utils::get_hex(r, g, b);
        }

        for (uint i = 0; i < padding; i++)
            utils::get_byte(file);
    }

    delete[] buff;
}

void Handler::read_rle_data(std::istream &file, PixelBuffer &pixels) {
    uint8_t r, g, b;
    uint8_t times;

    for (uint y = pixels.height(); y > 0; y--)
        for (uint x = 0; x < pixels.width(); x++) {
            times = utils::get_byte(file);

            b = utils::get_byte(file);
            g = utils::get_byte(file);
            r = utils::get_byte(file);

            for (uint i = 0; i < times; i++)
                pixels[y - 1][x + i] = utils::get_hex(r, g, b);
        }
}

int Handler::save(std::ostream &file, const PixelBuffer &pixels, FormatData *data) {
    Data *bmpdata = static_cast<Data *>(data);
    
    if (pixels.size.has_any_zero())
        return Error::INVALID_SIZE;

    this->write_header(file, pixels.size);
    this->write_info_header(file, pixels.size, bmpdata);
    
    if (bmpdata != nullptr && bmpdata->compression == Compression::RLE)
        this->write_rle_data(file, pixels);
    else
        this->write_raw_data(file, pixels);
    
    return Error::NO_ERROR;
}

void Handler::write_header(std::ostream &file, const Size &size) {
    this->write_signature(file);

    const uint header_size = 0x36;

    const int file_size = header_size + 3 * size.height * size.width;
    utils::le::write_as_bytes(file, file_size);
    utils::le::write_as_bytes<uint32_t>(file, 0);

    utils::le::write_as_bytes(file, header_size);
}

void Handler::write_info_header(std::ostream &file, const Size &size, Data *bmp) {
    utils::le::write_as_bytes(file, 0x28);

    utils::le::write_as_bytes(file, size.width);
    utils::le::write_as_bytes(file, size.height);

    utils::le::write_as_bytes(file, 0x180001);
    
    if (bmp != nullptr) {
        utils::le::write_as_bytes(file, bmp->compression);
        utils::le::write_as_bytes(file, 3 * size.height * size.width);

        utils::le::write_as_bytes(file, bmp->x_pixels_per_meter);
        utils::le::write_as_bytes(file, bmp->y_pixels_per_meter);

        utils::le::write_as_bytes(file, bmp->clr_used);
        utils::le::write_as_bytes(file, bmp->clr_important);

    } else {
        utils::le::write_as_bytes(file, 0);
        utils::le::write_as_bytes(file, 3 * size.height * size.width);

        utils::le::write_as_bytes(file, 0);
        utils::le::write_as_bytes(file, 0);

        utils::le::write_as_bytes(file, 0);
        utils::le::write_as_bytes(file, 0);
    }
}

void Handler::write_raw_data(std::ostream &file, const PixelBuffer &pixels) {
    uint padding = (pixels.width() * 3) % 4;

    for (uint y = pixels.height(); y > 0; y--) {
        for (uint x = 0; x < pixels.width(); x++) {
            auto [r, g, b] = utils::get_rgb(pixels[y - 1][x]);

            utils::write_byte(file, b);
            utils::write_byte(file, g);
            utils::write_byte(file, r);
        }

        for (uint i = 0; i < padding; i++)
            utils::write_byte(file, 0);
    }
}

void Handler::write_rle_data(std::ostream &file, const PixelBuffer &pixels) {

    for (uint y = pixels.height(); y > 0; y--) {
        uint32_t color = pixels[y - 1][0];
        uint8_t times = 1;

        for (uint x = 1; x < pixels.width(); x++)
            if (pixels[y - 1][x] == color) {
                times++;
            } else {
                utils::write_repeated(file, color, times);

                color = pixels[y - 1][x];
                times = 1;
            }
    }
}

void Handler::write_signature(std::ostream &file) {
    const char signature[] = {0x42, 0x4D};

    for (const auto &byte: signature)
        utils::write_byte(file, byte);
}

} // namespace bmp
