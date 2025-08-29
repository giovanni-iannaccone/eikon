# EikonCanvas

The `EikonCanvas` class includes a series of useful methods that we’ll explore in this section.

## The constructor
The constructor accepts three paramters:
- `pixels` an array of `uint32_t` representing the ARGB hex value of each image pixel (e.g., 0xFF00FF00). This value is stored in the object using an array of pointers, where each pointer refers to the beginning of a row. This method allows Eikon to easily implement several functions.
- `height` is a `size_t` storing the height of the image
- `width` is a `size_t` storing the width of the image

## The destructor
The destructor removes all variables created by Eikon for operation. This includes `pixels` and objects required by parsers (e.g., png). To preserve the PNG value and prevent its deletion, you can do the following:
```cpp
PNGData mypng = PNGData::get_data();
```

Refer to the <a href="formats/">formats documentation</a> to learn more.

>[!IMPORTANT]
> EikonCanvas does not free the pixel data—only the array of row pointers. You're responsible for manually releasing the pixel memory or using `free_pixels` to do it safely.
> **Why?** Because customization comes first: we can't assume whether you'll still need the pixels after deleting the canvas.

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
Prints an ASCII representation of the `pixels` array to standard output (`stdout`) based on the pixel's brightness.
```cpp
void ascii(uint scale) const {
    const std::string gradient = " `^\",:;Il!i~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
    
    for (uint y = 0; y < this->height; y += scale) {
        for (uint x = 0; x < this->width; x += scale) {
            uint8_t brightness = get_pixel_brightness(this->pixels[y][x]);
            std::cout << gradient[brightness * gradient.length() / 256];
        }

        std::cout << std::endl;
    }
}
```

Use the command `./ascii > file.txt` to redirect the output of the program into a file.

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
Provide an ARGB hex color code to uniformly paint the canvas.

For performance reasons, the `fill` method uses `memset` instead of iterating over each element.

## `flip`, `roll`, `rotate`, `stretch`
See the <a href="trasformations/">trasformations documentation</a> for more information.

## `hue`, `saturation`, `value`
Each of these methods accepts one parameter: an increment. They work by converting each element in the `pixels` array to HSV, adjusting the values, and converting it back to ARGB.

## `negate`
This method iterates over every pixel in the matrix and sets it to its negative. 

To compute a pixel's negative, subtract each of its color components from 255. For example, given a pixel (255, 140, 50), its negative would be: (255 - 255, 255 - 140, 255 - 50) -> (0, 115, 205)

```cpp
EikonCanvas *negate() {
    uint8_t r {}, g {}, b {};

    for (size_t y = 0; y < this->height; y++) {
        for (size_t x = 0; x < this->width; x++) {
            get_rgb(this->pixels[y][x], &r, &g, &b);

            r = 255 - r;
            g = 255 - g;
            b = 255 - b;

            this->pixels[y][x] = get_hex(r, g, b);
        }
    }

    return this;
}
```

## `read`
This method accepts two parameters: a file and a file type. The file is a reference to `std::istream`, and the type is a value from the filetype enum:
```cpp
typedef enum filetype {
    BMP,
    PNG,
    PPM
};
```

When called, it loads in the canvas pixel values and image dimensions.

## `save`
This method saves the image. It takes a reference to `std::ostream` and a filetype. The image is written using the standard of the selected format.
