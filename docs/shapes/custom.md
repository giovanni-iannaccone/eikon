# Custom shapes

Raccoon uses the dependency injection design pattern. This means that, behind the scenes, its code looks like this:
```cpp
RaccoonCanvas* draw(Drawable &obj) {
    obj.draw(this->pixels, this->height, this->width);
    return this;
}
```

Here, `Drawable` is simply an abstract class that all shapes inherit from. The `Drawable` class is implemented as follows:
```cpp
class Drawable {

public:
    virtual void draw(uint32_t **pixels, size_t height, size_t width) = 0;
};
```

Every shape—including the default ones—must extend this class and override the `draw` method. Thanks to this structure, developers can implement their own shapes without modifying the Raccoon source code. To add a custom shape, create a class that extends `Drawable`, override the `draw` method, and insert your drawing logic inside it.

Here’s an example of how to draw a rectangle:
```cpp
class Rectangle: public Drawable {

private:
    size_t x1, y1, h, b;
    uint32_t color;

public: 

    Rectangle(size_t x1, size_t y1, size_t h, size_t b, uint32_t color)
        : x1(x1), y1(y1), h(h), b(b),
        color(color) {}

    void draw(uint32_t pixels[], size_t height, size_t width) override {
        for (size_t y = y1; y < y1 + h; y++)
            for (size_t x = x1; x < x1 + b; x++)
                pixels[y][x] = get_alpha_blend_color(pixels[y][x], color);
    }
};
```

This code isn't thoroughly explained, as it's just a proof of concept. You can find a more detailed explanation in the <a href="rectangle.md">rectangle documentation</a>.

Create private class variables to store the data Raccoon needs to properly render the image, and use them inside the `draw` method to modify the pixel array. Feel free to experiment.

To actually draw your shape, instantiate a new shape object (using its constructor) and pass it to the `draw` method:
```cpp
Rectangle rec {150, 200, 100, 200, 0xFF15FFA1};

canvas->fill(0xFF000000)
    ->draw(rec);
```