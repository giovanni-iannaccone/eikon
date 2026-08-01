#include "../include/ppm.hpp"
#include "../include/utils.hpp"

namespace eikon::ppm {
        
Handler::Handler() {};

void Handler::extract_signature(std::istream &file, uint8_t signature[])
{
    file.seekg(0);
    for (uint i = 0; i < Handler::signature_size; i++)
        signature[i] = utils::get_byte(file);

    file.seekg(1, file.cur);
}

int Handler::get_dimensions(std::istream &file, Size &size)
{
    file.seekg(Handler::signature_size);
    uint buffer;
    
    file >> size.width >> size.height >> buffer;
    file.seekg(1, file.cur);
    
    return Error::NO_ERROR;
}

bool Handler::is_valid_signature(std::istream &file)
{
    uint8_t *signature = new uint8_t[this->signature_size];
    this->extract_signature(file, signature);
    
    bool valid = memcmp(signature, "P6", Handler::signature_size) == 0;
    delete[] signature;
    return valid;
}

int Handler::read(std::istream &file, PixelBuffer &pixels, FormatData *data)
{
    if (!this->is_valid_signature(file))
        return Error::INVALID_SIGNATURE;
    
    this->get_dimensions(file, pixels.size);
    char *buff = new char[pixels.width() * 3];
    
    for (uint y = 0; y < pixels.height(); y++) {
        file.read(buff, pixels.width() * 3);

        for (uint x = 0; x < pixels.width(); x++) {
            uint8_t r = buff[x*3 + 0];
            uint8_t g = buff[x*3 + 1];
            uint8_t b = buff[x*3 + 2];
            
            pixels[y][x] = utils::get_hex(r, g, b);
        }
    }

    delete[] buff;
    return Error::NO_ERROR;
}

int Handler::save(std::ostream &file, const PixelBuffer &pixels, FormatData *data)
{
    this->write_signature(file);

    if (pixels.size.has_any_zero())
        return Error::INVALID_SIZE;

    this->write_header(file, pixels.size);
    
    for (uint y = 0; y < pixels.height(); y++) {
        for (uint x = 0; x < pixels.width(); x++) {
            auto [r, g, b] = utils::get_rgb(pixels[y][x]);
            
            utils::write_byte(file, r);
            utils::write_byte(file, g);
            utils::write_byte(file, b);
        }
    }

    return Error::NO_ERROR;
}

void Handler::write_header(std::ostream &file, const Size &size)
{
    file << size.width << " " << size.height << "\n255\n";
}

void Handler::write_signature(std::ostream &file)
{
    file << "P6\n";
}

} // namespace ppm
