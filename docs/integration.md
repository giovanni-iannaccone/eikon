# Integration

We assume that you already have Raccoon properly installed and included in your project using your preferred method. If not, please follow the <a href="installation.md">installation guide</a>.

First, create a new `RaccoonCanvas` instance, as it will serve as the foundation for all other functions:
```cpp
RaccoonCanvas* canvas = new RaccoonCanvas(pixels, HEIGHT, WIDTH);
```

`pixels` is a flat matrix (an array), and `HEIGHT` and `WIDTH` represent the dimensions of the file. If you declare `canvas` as a global variable, it's good practice to make it static.

Now you can add shapes, apply effects, save files, and easily convert between formats:
```cpp
std::ofstream output_ppm {"output.ppm", std::ios::out};
std::ofstream output_png {"output.png", std::ios::out};

canvas->fill(0xFFFFFFFF)
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

If your projects uses the `RaccoonCanvas` class, it would be a good idea to implement the <a href="https://medium.com/@weidagang/modern-c-the-pimpl-idiom-53173b16a60a">pImpl idiom</a>, as the object can be big and any change will force you to recompile your whole code.