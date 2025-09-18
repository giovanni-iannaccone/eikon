# EikonCanvas

The `EikonCanvas` class includes a series of useful methods that we’ll explore in this section.

## The constructor
To accommodate a wide range of use cases, eikon provides multiple constructors:

### Constructor 1
This is the most commonly used constructor when creating new images. It takes three parameters:
- `pixels`: an array of `uint32_t` values, each representing a pixel in ARGB hexadecimal format (e.g., `0xFF00FF00`). Internally, the object stores these values using an array of pointers, where each pointer marks the beginning of a row. This structure enables efficient implementation of various image-processing functions.
- `height`: an `uint` representing the image's height.
- `width`: an `uint` representing the image's width.

### Constructor 2
This version is functionally equivalent to the first but optimized for performance. Instead of a flat array, it accepts a matrix ( (`uint32_t **`)—the format that the first constructor internally converts the array into.

### Constructor 3
Ideal for working with pre-existing image files, this constructor accepts:
- `file_name`: the name of the image file to load.
- `pixels`: a pointer to a `uint32_t **`. Pass the address of a matrix pointer, and the constructor will populate it with the pixel data.
- `height`: a pointer to an unsigned int, which will be set to the image's height.
- `width`: same as height, but for the image's width.

This constructor automatically reads the image file, making it extremely convenient for integrating image loading into your project.

This last constructor will automatically read the image so it makes really easy to load images inside your project.

## The destructor
The destructor removes all variables created by Eikon for operation. This includes `pixels` and objects required by parsers (e.g., png). To preserve the PNG value and prevent its deletion, you can do the following:
```cpp
PNGData mypng = PNGData::get_data();
```

Refer to the <a href="formats/">formats documentation</a> to learn more.

>[!IMPORTANT]
> EikonCanvas does not free the pixel data—only the array of row pointers. You're responsible for manually releasing the pixel memory or using `free_pixels` or `canvas->free_all()` to do it safely.
> **Why?** Because customization comes first: we can't assume whether you'll still need the pixels after deleting the canvas.

## `free_all`
This method will release all variables contained within the canvas and deallocate the pixel data you provided, rendering that array unusable.

## `area`
This method is particularly useful for executing code on a specific subsection of the canvas. It takes four parameters:
- `x1` and `y1` are the coordinates of the top-left corner of the area
- `h` and `b` are the height and width of the region, respectively

It returns a `std::shared_ptr` to a `EikonCanvas` object, which will be automatically deleted when no longer in use.

This snippet achieves high performance by directly manipulating `canvas` pixels in-place, eliminating the overhead of copying and ensuring memory safety.

```cpp
std::shared_ptr<EikonCanvas> area(uint x1, uint y1, uint h, uint b) {
    uint32_t **pixels_portion = new uint32_t*[h];
    for (uint i = 0; i < h; i++)
        pixels_portion[i] = &this->pixels[y1 + i][x1];

    return std::make_shared<EikonCanvas>(
        pixels_portion, h, b
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
    const std::string gradient = " `^\",:;Il!i~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
    
    for (uint y = 0; y < this->height; y += scale) {
        for (uint x = 0; x < this->width; x += scale) {
            uint8_t brightness = get_pixel_brightness(this->pixels[y][x]);
            out << gradient[brightness * gradient.length() / 256];
        }

        out << std::endl;
    }

    return this;
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
    obj.draw(this->pixels, this->height, this->width);
    return this;
}
```
Check the <a href="shapes/">shapes documentation</a> for more details on default and custom shapes.

## `fill`
This method fills the entire canvas with a single color. Internally, it sets every element in `pixels` to the specified value:
```cpp
EikonCanvas *fill(uint32_t color) {
    for (size_t y = 0; y < this->height; y++)
        memset(this->pixels[y], color, sizeof(uint32_t) * this->width);
    
    return this;
}
```
Provide an ARGB hex color code to uniformly paint the canvas. Alternatively to using a hex code, you can include `eikon/colors.hpp` and use any of the standard CSS color names.

```cpp
#include <eikon/colors.hpp>

canvas->fill(ALICE_BLUE);
```

For performance reasons, the `fill` method uses `memset` instead of iterating over each element.

## `read`
This method saves the image and provides two overloads:
1. The first accepts a reference to `std::ostream` along with a file type. The file is a reference to `std::istream`, and the type is a value from the filetype enum:
```cpp
typedef enum filetype {
    BMP,
    PNG,
    PPM
};
```
2. The second takes a file name and automatically detects the file type based on its extension.

When called, this function loads the canvas's pixel values and image dimensions. If the operation fails, it returns a null pointer. Therefore, it's recommended to check the return value before invoking any methods on it to avoid a core dump.

## `save`
This method saves the image and provides two overloads:
1. The first accepts a reference to `std::ostream` along with a file type.
2. The second takes a file name and automatically detects the file type based on its extension.

Both overloads also accept a `void*` pointer to a header model specific to the file format (see <a href="formats/">formats</a> for more details). The image is written according to the standard of the selected format.

## `hue`, `saturation`, `value`
Each of these methods takes a single parameter: increment, a positive float. The process involves converting each element in the pixels array from ARGB to HSV, adjusting the relevant HSV component by multiplying it with the increment, and then converting it back to ARGB.

To reduce a component, use an increment between 0 and 1. To increase it, use a value greater than 1.

## `brightness`, `contrast`, `equalize`, `grayscale`, `negate`
See the <a href="enhancements/">enhancements documentation</a> for more information.

## `flip`, `flop`, `padding`, `roll`, `rotate`, `stretch`
See the <a href="trasformations/">trasformations documentation</a> for more information.

## `blur`, `raise`
See the <a href="effects/">effects documentation</a> for more information.

## `sepia`, `solarize`
See the <a href="FX/">FX documentation</a> for more information.
