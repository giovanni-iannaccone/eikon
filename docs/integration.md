# Integration

We assume that you already have Eikon properly installed and included in your project using your preferred method. If not, please follow the <a href="installation.md">installation guide</a>.

First, create a new `EikonCanvas` instance, as it will serve as the foundation for all other functions:
```cpp
EikonCanvas* canvas = new EikonCanvas(pixels, height, width);
```

To reduce redundancy, write it as:
```cpp
auto canvas = new EikonCanvas(pixels, height, width);
```

`pixels` can be either an array or a matrix, while `height` and `width` represent the dimensions of the image. When `pixels` is passed as an array, it will be converted into a matrix. This conversion is necessary both to optimize the use of the `area` method and to speed up operations that modify the image dimensions.

If you declare `canvas` as a global variable, it's good practice to make it `static`.

Now you can add shapes, apply effects, save files, and easily convert between formats:
```cpp
std::ofstream output_ppm {"output.ppm", std::ios::out};
std::ofstream output_png {"output.png", std::ios::out};

Triangle t {100, 100, 600, 200, 400, 500, 0xFFFF00EE};

canvas->fill(0xFFFFFFFF)
    ->draw(t)
    ->save(output_ppm, PPM);

canvas->save(output_png, PNG);
```

You can also load existing files into your project:
```cpp
std::ifstream input {file_name, std::ios::in};
if (!input) return 1;

canvas->read(input, PPM);
```

Remember to delete the `canvas` object when you don't need it anymore:
```cpp
delete canvas;
```

If you prioritize compilation time over execution time (you’re probably wrong—unless you’re working on a huge project), it’s a good idea to implement the <a href="https://medium.com/@weidagang/modern-c-the-pimpl-idiom-53173b16a60a">pImpl idiom</a>. Large objects with frequent internal changes can trigger widespread recompilation, and pImpl helps mitigate that by isolating implementation details.

Obviously, writing colors code by hand is difficult, which is why eikon supports all CSS colors. You can access all of them simply by adding
```cpp
#include <eikon/colors.hpp>
```
to your file and then use them in your code like this:
```cpp
canvas->fill(ALICE_BLUE);
```

Make sure to include the `-leikon` flag in your g++ command; otherwise, the function implementations won't be linked and the program will fail to run. 