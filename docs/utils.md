# Utils

These helper functions support image processing tasks like pixel manipulation, color conversion, memory allocation, and file I/O. You will find these useful if you decide to implement a new shape, a new effect or a new format. 
Here's what each one does:

## `FileType detect_filetype(const std::string &file_name)`
Identifies the file format (BMP, PNG, or PPM) by checking the file extension. Relies on `split()` internally.

## `void free_pixels(uint32_t **pixels, uint height)`
Safely deallocates a 2D pixel array.

## `uint32_t get_alpha_blend_color(uint32_t c1, uint32_t c2)`
Blends two colors based on the alpha channel of the second one. Returns the resulting color as a `uint32_t`.

## `uint8_t get_byte(std::istream &file)`
Reads a single byte from a binary input stream. Handy for low-level file parsing.

## `std::unique_ptr<FormatHandler> get_format_handler(FileType ft)`
This function returns the default handler for the `FileType`.

## `uint32_t get_hex(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF)`
Combines RGBA values into a single 32-bit integer. This is the internal format used for storing pixel data.

## `uint8_t get_pixel_brightness(uint32_t pixel)`
Returns the brightness of a pixel by taking the maximum of its R, G, and B components.

## `void get_rgb(uint32_t pixel, uint8_t &r, uint8_t &g, uint8_t &b)`
Extracts the red, green, and blue components from a packed pixel value.

## `void hsi_2_rgb(uint H, float S, float I, uint8_t *R, uint8_t *G, uint8_t *B)`
Converts a color from HSI (Hue, Saturation, Intensity) to RGB.

## `void hsv_2_rgb(uint H, float S, float V, uint8_t *R, uint8_t *G, uint8_t *B)`
Converts HSV (Hue, Saturation, Value) to RGB. Similar to HSI but with a different approach to brightness.

## `uint32_t increase_brightness(uint32_t *pixel, float inc)`
Multiplies the RGB components of a pixel by a factor to make it brighter. Values are clamped to avoid overflow.

## `std::mt19937 initialize_randomness()`
Returns a random number generator, useful in `add_noise` method.

## `void rgb_2_hsi(uint8_t R, uint8_t G, uint8_t B, uint *H, float *S, float *I)`
Converts an RGB color to HSI.

## `void rgb_2_hsv(uint8_t R, uint8_t G, uint8_t B, uint *H, float *S, float *V)`
Converts RGB to HSV.

## `void write_byte(std::ostream &file, const char data)`
Writes a single byte to a binary output stream. Used for saving raw image data.

## `void write_repeated(std::ostream &file, uint32_t color, uint8_t reps)`
Writes a color multiple times to a file, along with its repetition count. Useful for simple run-length encoding (RLE).

## `template <typename T> bool in(const T& element, const std::vector<T>& vec)`
Checks if an element exists in a vector.

## `template <numeric T, numeric ... Args> T max(T a, T b, Args ... args)`
Returns the maximum of n values, works only on numerical types

## `template <numeric T, numeric ... Args> T min(T a, T b, Args ... args)`
Returns the minimum of n values. Same deal.

## `template <typename T> T be::get_bytes(std::istream &file)`
Reads a value of type T from a stream in big-endian order.

## ` template <typename T> T be::write_as_bytes(std::istream &file, T data)`
Writes a value of type T to a stream in big-endian format, regardless of system endianness.

## `template <typename T> T le::get_bytes(std::istream &file)`
Reads a value of type T from a stream in little-endian order.

## ` template <typename T> T le::write_as_bytes(std::istream &file, T data)`
Same but in little-endian format.
