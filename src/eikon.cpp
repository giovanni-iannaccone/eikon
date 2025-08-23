#include "../include/eikon.hpp"

EikonCanvas::EikonCanvas(uint32_t *pixels, uint height, uint width)
: height(height),
width(width) {

    this->pixels = new uint32_t*[height];

    for (uint i = 0; i < height; i++)
        this->pixels[i] = pixels + i * width;
}

EikonCanvas::EikonCanvas(uint32_t **pixels, uint height, uint width)
: pixels(pixels),
height(height),
width(width) {};

EikonCanvas::~EikonCanvas() {
    delete[] this->pixels;
}

std::shared_ptr<EikonCanvas> EikonCanvas::area(uint x1, uint y1, uint h, uint b) {
    uint32_t **pixels_portion = new uint32_t*[h];
    for (uint i = 0; i < h; i++)
        pixels_portion[i] = &this->pixels[y1 + i][x1];

    return std::make_shared<EikonCanvas>(
        pixels_portion, h, b
    );
}

void EikonCanvas::ascii(uint scale) const {
    const std::string gradient = " `^\",:;Il!i~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
    
    for (uint y = 0; y < this->height; y += scale) {
        for (uint x = 0; x < this->width; x += scale) {
            uint8_t brightness = get_pixel_brightness(this->pixels[y][x]);
            std::cout << gradient[brightness * gradient.length() / 256];
        }

        std::cout << std::endl;
    }
}

EikonCanvas *EikonCanvas::blur() {
    return this;
}

EikonCanvas *EikonCanvas::brightness(int perc) {
    float inc = 1 + (static_cast<float>(perc) / 100);
    uint8_t r {}, g {}, b {};

    for (uint y = 0; y < this->height; y++)
        for (uint x = 0; x < this->width; x++) {
            get_rgb(this->pixels[y][x], &r, &g, &b);

            r = static_cast<uint8_t>(std::min(255.0f, r * inc));
            g = static_cast<uint8_t>(std::min(255.0f, g * inc));
            b = static_cast<uint8_t>(std::min(255.0f, b * inc));

            this->pixels[y][x] = get_hex(r, g, b);
        }

    return this;
}

EikonCanvas *EikonCanvas::chop(int cols) {
    if (cols > 0)
        for (int i = 0; i < this->height; i++)
            this->pixels[i] += cols;
    
    this->width -= abs(cols);        
    return this;
}

EikonCanvas *EikonCanvas::chop_and_delete(int cols) {
    if (cols < 0)
        for (int i = this->height - 1; i > this->height - cols; i++)
            for (int j = this->width + cols; j < this->width; j++)
                delete &this->pixels[i][j];
        
    else
        for (int i = 0; i < this->height; i++) {
            for (int j = 0; j < cols; j++)
                delete &this->pixels[i][j];
            
            this->pixels[i] += cols;
        }
    
    this->width -= abs(cols);
    return this;
}

EikonCanvas *EikonCanvas::contrast(int perc) {
    return this;
}

EikonCanvas *EikonCanvas::crop(int row) {
    auto new_pixels = new uint32_t*[this->height - abs(row)];

    if (row < 0)
        for (int i = 0; i < this->height + row; i++)
            new_pixels[i] = this->pixels[i];

    else
        for (int i = row; i < this->height; i++)
            new_pixels[i - row] = this->pixels[i];
    
    delete[] this->pixels;
    this->pixels = new_pixels;
    this->height -= abs(row);

    return this;
}

EikonCanvas *EikonCanvas::crop_and_delete(int row) {
    auto new_pixels = new uint32_t*[this->height - abs(row)];

    if (row < 0) {
        for (int i = 0; i < this->height + row; i++)
            new_pixels[i] = this->pixels[i];

        for (int i = this->height + row; i < this->height; i++)
            delete[] this->pixels[i];
    
    } else {
        for (int i = 0; i < row; i++)
            delete[] this->pixels[i];

        for (int i = row; i < this->height; i++)
            new_pixels[i - row] = this->pixels[i];
    }

    delete[] this->pixels;
    this->pixels = new_pixels;
    this->height -= abs(row);

    return this;
}

void EikonCanvas::delete_all() {
    for (uint i = 0; i < this->height; i++)
        delete[] this->pixels[i];
}

EikonCanvas *EikonCanvas::draw(Drawable &obj) {
    obj.draw(this->pixels, this->height, this->width);
    return this;
}

EikonCanvas *EikonCanvas::equalize() {

    return this;
}

EikonCanvas *EikonCanvas::fill(uint32_t color) {
    for (uint i = 0; i < this->height; i++)
        memset(this->pixels[i], color, sizeof(uint32_t) * this->width);
    
    return this;
}

EikonCanvas *EikonCanvas::flip() {
    for (uint y = 0; y < this->height / 2; y++)
        for (uint x = 0; x < this->width; x++)
            std::swap(
                this->pixels[y][x],
                this->pixels[this->height - y - 1][x]
            );
    
    return this;
}

EikonCanvas *EikonCanvas::flop() {
    for (uint y = 0; y < this->height; y++)
        for (uint x = 0; x < this->width / 2; x++)
            std::swap(
                this->pixels[y][x],
                this->pixels[y][this->width - x - 1]
            );
    
    return this;
}

EikonCanvas *EikonCanvas::gray_scale() {
    uint8_t r {}, g {}, b {};
    uint32_t pixel;

    for (uint y = 0; y < this->height; y++)
        for (uint x = 0; x < this->width; x++) {
            get_rgb(this->pixels[y][x], &r, &g, &b);

            pixel = 0.30 * r + 0.59 * g + 0.11 * b;

            this->pixels[y][x] = get_hex(
                pixel,
                pixel,
                pixel
            );
        }

    return this;
}

EikonCanvas *EikonCanvas::hue(int inc) {
    float h {}, s {}, v {};
    uint8_t r {}, g {}, b {};
    
    for (uint y = 0; y < this->height; y++) {
        for (uint x = 0; x < this->width; x++) {
            get_rgb(this->pixels[y][x], &r, &g, &b);
            rgb_2_hsv(r, g, b, &h, &s, &v);
            
            h += inc;
            hsv_2_rgb(h, s, v, &r, &g, &b);
            this->pixels[y][x] = get_hex(r, g, b);
        }
    }
    
    return this;
}

EikonCanvas *EikonCanvas::negate() {
    uint8_t r {}, g {}, b {};

    for (uint y = 0; y < this->height; y++) {
        for (uint x = 0; x < this->width; x++) {
            get_rgb(this->pixels[y][x], &r, &g, &b);

            r = 255 - r;
            g = 255 - g;
            b = 255 - b;

            this->pixels[y][x] = get_hex(r, g, b);
        }
    }

    return this;
}

EikonCanvas *EikonCanvas::read(std::istream &file, FileType ft) {
    const std::map<FileType, reader> readers = {
        {FileType::JPEG, read_jpeg},
        {FileType::PNG,  read_png},
        {FileType::PPM,  read_ppm}
    };

    if (!readers.count(ft))
        return nullptr;
    
    readers.at(ft)(file, this->pixels, &this->height, &this->width);
    return this;
}

EikonCanvas *EikonCanvas::roll(uint col) {

    for (uint i = 0; i < this->height; i++)
        std::rotate(
            this->pixels[i], 
            &this->pixels[i][this->width - col], 
            &this->pixels[i][this->width]
        );

    return this;
}

EikonCanvas *EikonCanvas::rotate() {
    if (this->width != this->height)
        return nullptr;
    
    transpose_matrix(this->pixels, this->height, this->width);
    reverse_matrix(this->pixels, this->height, this->width);
    
    return this;
}

EikonCanvas *EikonCanvas::saturation(int inc) {
    float h {}, s {}, v {};
    uint8_t r {}, g {}, b {};
    
    for (uint y = 0; y < this->height; y++) {
        for (uint x = 0; x < this->width; x++) {
            get_rgb(this->pixels[y][x], &r, &g, &b);
            rgb_2_hsv(r, g, b, &h, &s, &v);
            
            s += inc;
            hsv_2_rgb(h, s, v, &r, &g, &b);
            this->pixels[y][x] = get_hex(r, g, b);
        }
    }
    
    return this;
}

bool EikonCanvas::save(std::ostream &file, FileType ft, void *args) {
    const std::map<FileType, saver> savers = {
        {FileType::JPEG, save_jpeg},
        {FileType::PNG,  save_png},
        {FileType::PPM,  save_ppm}
    };

    if (!savers.count(ft))
        return false;
    
    return savers.at(ft)(file, this->pixels, this->height, this->width, args);
}

EikonCanvas *EikonCanvas::sepia() {
    uint8_t r {}, g {}, b {};

    for (uint y = 0; y < this->height; y++)
        for (uint x = 0; x < this->width; x++) {
            get_rgb(this->pixels[y][x], &r, &g, &b);

            this->pixels[y][x] = get_hex(
                0.393 * r + 0.769 * g + 0.189 * b,
                0.349 * r + 0.686 * g + 0.168 * b,
                0.272 * r + 0.534 * g + 0.131 * b
            );
        }

    return this;
}

EikonCanvas *EikonCanvas::stretch(uint size) {
    uint32_t *new_pixels {};

    for (uint y = 0; y < this->height; y++) {
        new_pixels = new uint32_t[this->width * size];
        
        for (uint x = 0; x < this->width; x++)
            for (uint i = 0; i < size; i++)
                new_pixels[x * size + i] = this->pixels[y][x];

        this->pixels[y] = new_pixels;
    }

    this->width *= size;
    new_pixels = nullptr;
    return this;
}

EikonCanvas *EikonCanvas::value(int inc) {
    float h {}, s {}, v {};
    uint8_t r {}, g {}, b {};

    for (uint y = 0; y < this->height; y++) {
        for (uint x = 0; x < this->width; x++) {
            get_rgb(this->pixels[y][x], &r, &g, &b);
            rgb_2_hsv(r, g, b, &h, &s, &v);
            
            v += inc;
            hsv_2_rgb(h, s, v, &r, &g, &b);
            this->pixels[y][x] = get_hex(r, g, b);
        }
    }

    return this;
}
