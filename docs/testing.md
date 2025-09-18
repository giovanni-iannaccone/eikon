# Testing framework

## TL;DR
- The framework uses `std::unique_ptr` to manage memory safely.
- It compares expected pixels with saved image's pixels ignoring the alpha channel.
- It logs results using a custom logs module, which tells you if a test passed, failed, or created a new image.
- At the end, it prints a summary of how many tests failed.

This setup is great for regression testing. If you change your image processing code and something breaks, you’ll know immediately. If you add a new test it will log that a new image is being created. You can extend it easily—add new tests, swap out formats. It’s minimal, but powerful.

--- 

This framework is designed to help you test image transformations using the `EikonCanvas` class. It’s built to be flexible, clean, and safe—especially when working with dynamically allocated pixel data. You can use it to verify that your image operations (like blur, solarize, pad, etc.) produce consistent results over time.

There are two main ways to write tests here:

1. Standard Tests (`TestEnv`)

These are for functions that modify the canvas directly. You pass in a function that takes a canvas and a filename, and it runs the transformation and saves the result. The framework then compares the output pixels with a previously saved image to check if they match.

If the reference image doesn’t exist yet, it creates one and logs that it’s a new file. Otherwise, it loads the old image and compares it pixel by pixel (ignoring alpha).

2. Isolated Tests (`IsolatedEnv`)

These are for functions that return a new pixel matrix instead of modifying the canvas. It’s useful when you want to isolate the transformation logic or avoid side effects.

The framework runs your function, gets the new matrix, and compares it with the saved image. It also handles memory cleanup for you—just make sure your function returns a heap-allocated matrix so it can be freed properly.

You start by registering your tests:
```cpp
Test test;

test.register_test("blur", [](EikonCanvas* canvas, const std::string& file) {
    return canvas->blur()->save(file);
});

test.register_isolated("stretch", IsolatedEnv(
    HEIGHT, WIDTH,
    [](EikonCanvas* canvas, const std::string& file) {
        return canvas->solarize(0.5f, file);
    }
));
```

Then you run everything with:
```cpp
test.run("ppm", Resource::INITIALIZE);
```

The "ppm" is the file extension used for saving and comparing images. You can skip `Resource::INITIALIZE` if your test doesn’t need a shared canvas.