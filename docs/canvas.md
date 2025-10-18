# EikonCanvas

The `EikonCanvas` class includes a series of useful methods that we’ll explore in this section.

## The constructor
To accommodate a wide range of use cases, eikon provides multiple constructors:

### Constructor 1
The first constructor accepts two `uint` parameters, `height` and `width` which represents image's dimensions.
These dimensions are useful to initialize `PixelBuffer`, the class the canvas internally uses to manage pixels. 

### Constructor 2
This constructor accepts an `std::istream` and a `FileType` ( an enum, type `FileType::` to see allowed file types). 
It uses the `FileType` to read the `std::istream` and instantiate `PixelBuffer` with the data in the stream.

### Constructor 3
This constructor is very similar to the previous one but instead of `std::istream` and `FileType` it accepts a string 
that represents the filename. These two constructors are really useful to load an existing image in your project.

### Constructor 4
This constructor accepts a `PixelBuffer`, if you already have one in your project, you can easly use it to instantiate `EikonCanvas`.

## `area`
This method is particularly useful for executing code on a specific subsection of the canvas. It takes four parameters:
- `x1` and `y1` are the coordinates of the top-left corner of the area
- `h` and `b` are the height and width of the region, respectively

It returns a `std::shared_ptr` to a `EikonCanvas` object, which will be automatically deleted when no longer in use.

This snippet achieves high performance by directly manipulating `canvas` pixels in-place, eliminating the overhead of copying and ensuring memory safety.

```cpp
std::shared_ptr<EikonCanvas> area(uint x1, uint y1, uint h, uint b) {
    PixelBuffer pixels_area {h, 0, false};
    
    for (uint i = 0; i < h; i++)
        pixels_area[i] = this->pixels[y1 + i] + x1;

    pixels_area.width = b;
    
    return std::make_shared<EikonCanvas>(
        pixels_area
    );
}
```

You can chain this method with others to apply any operation to a specific area:
```cpp
canvas->area(100, 100, 100, 100)
    ->flip();
```

## `ascii`
Prints an ASCII representation of the pixels array to a chosen output stream, based on each pixel's brightness. The default value of the `out` parameter is `std::cout`, but it can be changed by passing a different `ostream`.

```cpp
EikonCanvas *ascii(uint scale = 1, std::ostream &out = std::cout) {
    const std::string gradient = " `,^\":;~+_-iIl!?][*}{1)(|\\/tfjrvuncoazxmwqpdbkhXYUJCLQ0OZ#MW&8%B$@";
    
    for (uint y = 0; y < this->height(); y += scale) {
        for (uint x = 0; x < this->width(); x += scale) {
            uint8_t brightness = get_pixel_brightness(this->pixels[y][x]);
            out << gradient[brightness * gradient.length() / 256];
        }

        out << std::endl;
    }

    return this;
}
```

## `at`
This method returns the value of a pixel at the specified coordinates
```cpp
uint32_t EikonCanvas::at(uint x, uint y) const {
    return this->pixels[y][x];
}
```

## `draw`
This method is used to draw shapes. Create an instance of a shape class and pass it to this method:
```cpp
Rectangle rec {150, 200, 100, 200, 0xFFA1FF15};

canvas->fill(0xFF000000)
    ->draw(rec);
```

Internally, the method just calls the draw method of a reference to a `Drawable` object:
```cpp
EikonCanvas *draw(Drawable &obj) {
    obj.draw(this->pixels);
    return this;
}
```
Check the <a href="shapes/">shapes documentation</a> for more details on default and custom shapes.

## `fill`
This method fills the entire canvas with a single color. Internally, it sets every element in `pixels` to the specified value:
```cpp
EikonCanvas *fill(uint32_t color) {
    for (uint y = 0; y < this->height(); y++)
        std::memset(this->pixels[y], color, sizeof(uint32_t) * this->width());
    
    return this;
}
```
Provide an ARGB hex color code to uniformly paint the canvas. Alternatively to using a hex code, you can include `eikon/colors.hpp` and use any of the standard CSS color names.

```cpp
#include <eikon/colors.hpp>

canvas->fill(colors::ALICE_BLUE);
```

## `map`
This method performa a function on every pixel in the canvas. As pixels operation can be heavy, this method can also cache the result to speedup the process:

```cpp
EikonCanvas *map(std::function <void (uint32_t &)> f, bool cache_values = true) {
    if (!cache_values) {

        for (uint y = 0; y < this->height(); y++)
            for (uint x = 0; x < this->width(); x++)
                f(this->pixels[y][x]);

        return this;
    }

    cache value = initialize_cache(this->pixels[0][0], f);
    
    for (uint y = 0; y < this->height(); y++)
        for (uint x = 0; x < this->width(); x++)
            
            if (this->pixels[y][x] == value.input) {
                this->pixels[y][x] = value.output;
            } else {
                value.input = this->pixels[y][x];
                f(this->pixels[y][x]);
                value.output = this->pixels[y][x];
            }
    
    return this;
}

```

## `get_pixels` and `get_pixels_copy`
These two methods return a reference of the internal canvas' pixels and a copy of it. If you need to directly edit canvas, use the first one and do your modifications on it. Use the second one if you want to keep it after canvas deletion. 

## `height` and `width`
These two methods return respectively canvas' height and width.

## `size`
This method returns a pair with both height and width

```cpp
const std::pair<uint, uint> EikonCanvas::size() const {
    return std::make_pair(this->height(), this->width());
}
```

To use it:
```cpp
auto [height, width] = canvas->size();
```

## `read`
This method saves the image and provides two overloads:
1. The first accepts a reference to `std::ostream` along with a file type. The file is a reference to `std::istream`, and the type is a value from the `FileType` enum.

2. The second takes a file name and automatically detects the file type based on its extension.

When called, this function loads the canvas's pixel values and image dimensions.

## `save`
This method saves the image and provides two overloads:
1. The first accepts a reference to `std::ostream` along with a file type.
2. The second takes a file name and automatically detects the file type based on its extension.

Both overloads also accept a `FormatData *` pointer to a header model specific to the file format (see <a href="formats/">formats</a> for more details). The image is written according to the standard of the selected format.

## `hue`, `saturation`, `value`
Each of these methods takes a single parameter: increment, a positive float. The process involves converting each element in the pixels array from ARGB to HSV, adjusting the relevant HSV component by multiplying it with the increment, and then converting it back to ARGB.

To reduce a component, use an increment between 0 and 1. To increase it, use a value greater than 1.

## `brightness`, `contrast`, `equalize`, `grayscale`, `negate`
See the <a href="enhancements/">enhancements documentation</a> for more information.

## `chop`, `crop`, `flip`, `flop`, `padding`, `roll`, `rotate`, `stretch`
See the <a href="trasformations/">trasformations documentation</a> for more information.

## `add_noise`, `blur`, `raise`
See the <a href="effects/">effects documentation</a> for more information.

## `isolate`, `sepia`, `solarize`
See the <a href="FX/">FX documentation</a> for more information.
