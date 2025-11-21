# Integration

We assume that you already have Eikon properly installed and included in your project using your preferred method. If not, please follow the <a href="installation.md">installation guide</a>.

First, create a new `Canvas` instance, as it will serve as the foundation for all other functions:
```cpp
eikon::Canvas canvas {height, width};
```

`height` and `width` represent the dimensions of the image.
If you declare `canvas` as a global variable, it's good practice to make it `static`.

Now you can add shapes, apply effects, save files, and easily convert between formats:
```cpp
std::ofstream output_png {"output.png", std::ios::out};

Triangle t {100, 100, 600, 200, 400, 500, 0xFFFF00EE};

canvas->fill(0xFFFFFFFF)
    ->draw(t)
    ->save("output.ppm");

canvas->save(output_png, PNG);
```

You can also load existing files into your project with `EikonCanvas` constructor
```cpp
eikon::Canvas canvas {file_name};
```
or with `read` method
```cpp
eikon::Canvas canvas {HEIGHT, WIDTH};
canvas->read(file_name);
```

If you prioritize compilation time over execution time (you’re probably wrong—unless you’re working on a huge project), it’s a good idea to implement the <a href="https://medium.com/@weidagang/modern-c-the-pimpl-idiom-53173b16a60a">pImpl idiom</a>. Large objects with frequent internal changes can trigger widespread recompilation, and pImpl helps mitigate that by isolating implementation details.

Obviously, writing colors code by hand is difficult, which is why eikon supports all CSS colors. You can access all of them simply by adding
```cpp
#include <eikon/colors.hpp>
```
to your file and then use them in your code like this:
```cpp
canvas->fill(eikon::colors::ALICE_BLUE);
```

These colors have their alpha channel set to 0xFF, which makes the shapes fully opaque. If you want to implement transparency, you can do so as follows:
```cpp
eikon::Circle cirlce {100.0, 400, 400,  0x7FFFFFFF & ALICE_BLUE};
```
The first byte (7F) represents the transparency level.

Make sure to include the `-leikon` flag in your g++ command; otherwise, the function implementations won't be linked and the program will fail to run. 