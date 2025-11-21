# Testing framework

## TL;DR
- The framework uses `std::unique_ptr` to manage memory safely.
- It compares expected pixels with saved image's pixels ignoring the alpha channel.
- It logs results using a custom logs module, which tells you if a test passed, failed, or created a new image.
- At the end, it prints a summary of how many tests failed.

This setup is great for regression testing. If you change your image processing code and something breaks, you’ll know immediately. If you add a new test it will log that a new image is being created. You can extend it easily—add new tests, swap out formats. It’s minimal, but powerful.

--- 

This framework is designed to help you test image transformations using the `EikonCanvas` class. It’s built to be flexible, clean, and safe. You can use it to verify that your image operations (like blur, solarize, pad, etc.) produce consistent results over time.

Tests are encapsulated in `TestEnv` class.
You pass in a function that takes a canvas and it runs the transformation. The framework then compares the output pixels with a previously saved image to check if they match.

If the reference image doesn’t exist yet, it creates one and logs that it’s a new file. Otherwise, it loads the old image and compares it pixel by pixel (ignoring alpha).

You start by registering your tests:
```cpp
Test test;

test.register_test("blur", [](eikon::Canvas &canvas) {
    return canvas->blur();
});
```

Then you run everything with:
```cpp
test.run("ppm");
```

The "ppm" is the file extension used for saving and comparing images.