# PPM

The PPM format is very basic. It starts with a simple signature (P + version), followed by the image dimensions (width and height), the maximum color value and then a sequence of RGB triplets representing the pixels.

Here’s an example of what a basic PPM file might look like:
```
P6 3 3 255 255 255 255 255 255 255 255 255 255 255 255 255 0 0 0 255 255 255 255 255 255 255 255 255 255 255 255
```

Parsing a PPM file is straightforward. A function to read a PPM might look like this:
```cpp
bool read_ppm(std::istream &file, uint32_t pixels[], size_t *height_ptr, size_t *width_ptr) {
    uint8_t r {}, g {}, b {};
    
    get_ppm_dimensions(file, height_ptr, width_ptr);

    for (size_t i = 0; i < (*height_ptr) * (*width_ptr); i++) {
        file >> r >> g >> b;
        pixels[i] = get_hex(r, g, b);
    }

    return true;
}
```

Parameters:
- `file`: the input stream representing the file to be read
- `pixels`: an array where the hexadecimal pixel values will be stored
- `height_ptr` and `width_ptr`: pointers that will be set to the image dimensions

Saving a PPM file is even simpler:
```cpp
bool save_ppm(std::ostream &file, uint32_t pixels[], size_t height, size_t width, void *args) {
    file << "P6\n" << width << " " << height << "\n255\n";

    for (size_t i = 0; i < height * width; i++) {
        uint32_t pixel = pixels[i];
        
        uint8_t r = (pixel >> 0) & 0xFF;
        uint8_t g = (pixel >> 8) & 0xFF;
        uint8_t b = (pixel >> 16) & 0xFF;

        file << r << g << b;
    }

    return true;
}
```

The three lines in the middle simply extract each RGB component from the hexadecimal pixel value.