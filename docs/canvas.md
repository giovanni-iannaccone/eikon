# RaccoonCanvas

The `RaccoonCanvas` class includes a series of useful methods that we’ll explore in this section.

## The constructor
The constructor assigns values to four private variables:
- `pixels` an array of `uint32_t` representing the ARGB hex value of each image pixel (e.g., 0xFF00FF00)
- `height` is a `size_t` storing the height of the image
- `width` is a `size_t` storing the width of the image
- `delete_data` is an optional parameter of type `bool`, it specifies whether all values should be deleted when the destructor is called. Its default value is `true`

## The destructor
The destructor removes all variables created by Raccoon for operation (if `delete_data` is set to `true`). This includes `pixels` and objects required by parsers (e.g., png). To preserve the PNG value and prevent its deletion, you can do the following:
```cpp
PNGData *mypng = png;
set_png(nullptr);
```

Refer to the <a href="formats/">formats documentation</a> to learn more.

## `area`
This method is particularly useful for executing code on a specific subsection of the canvas. It takes four parameters:
- `x1` and `y1` are the coordinates of the top-left corner of the area
- `h` and `b` are the height and width of the region, respectively

It returns a `std::shared_ptr` to a `RaccoonCanvas` object, which will be automatically deleted when no longer in use.

You can chain this method with others to apply any operation to a specific area:
```cpp
canvas->area(100, 100, 100, 100)
    ->flip();
```

## `ascii`
Prints an ASCII representation of the `pixels` array to standard output (`stdout`).

## `draw`
This method is used to draw shapes. Create an instance of a shape class and pass it to this method:
```cpp
auto rec = new Rectangle(150, 200, 100, 200, 0xFFA1FF15);

canvas->fill(0xFF000000)
    ->draw(rec);
```

Check the <a href="shapes/">shapes documentation</a> for more details on default and custom shapes.

## `fill`
This method fills the entire canvas with a single color. Internally, it sets every element in `pixels` to the specified value:
```cpp
RaccoonCanvas* fill(uint32_t color) {
    for (size_t i = 0; i < this->height * this->width; i++)
        this->pixels[i] = color;

    return this;
}
```
Provide an ARGB hex color code to uniformly paint the canvas.

## `flip`, `rotate`, `stretch`
See the <a href="effects/">effects documentation</a> for more information.

## `hue`, `saturation`, `value`
Each of these methods accepts one parameter: an increment. They work by converting each element in the `pixels` array to HSV, adjusting the values, and converting it back to ARGB.

## `read`
This method accepts two parameters: a file and a file type. The file is a reference to `std::istream`, and the type is a value from the filetype enum:
```cpp
typedef enum filetype {
    PPM,
    PNG,
    JPEG
};
```

When called, it loads in the canvas pixel values and image dimensions.

## `save`
This method saves the image. It takes a reference to `std::ostream` and a filetype. The image is written using the standard of the selected format.