# Utils

These helper functions support image processing tasks like pixel manipulation, color conversion, memory allocation, and file I/O. You will find these useful if you decide to implement a new shape, a new effect or a new format. 
Here's what each one does:

## `uint32_t **allocate_pixels(uint height, uint width)`
Creates a 2D array of pixels (uint32_t**) with the specified dimensions. Each row is dynamically allocated.

## `FileType detect_filetype(const std::string &file_name)`
Identifies the file format (BMP, PNG, or PPM) by checking the file extension. Relies on `split()` internally.

## `void free_pixels(uint32_t **pixels, uint height)`
Safely deallocates a 2D pixel array. Should be called after `allocate_pixels()` to prevent memory leaks.

## `uint32_t get_alpha_blend_color(uint32_t c1, uint32_t c2)`
Blends two colors based on the alpha channel of the second one. Returns the resulting color as a `uint32_t`.

## `uint8_t get_byte(std::istream &file)`
Reads a single byte from a binary input stream. Handy for low-level file parsing.

## `uint32_t get_hex(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF)`
Combines RGBA values into a single 32-bit integer. This is the internal format used for storing pixel data.

## `uint8_t get_pixel_brightness(uint32_t pixel)`
Returns the brightness of a pixel by taking the maximum of its R, G, and B components.

## `void get_rgb(uint32_t pixel, uint8_t *r, uint8_t *g, uint8_t *b)`
Extracts the red, green, and blue components from a packed pixel value.

## `void hsi_2_rgb(uint H, float S, float I, uint8_t *R, uint8_t *G, uint8_t *B)`
Converts a color from HSI (Hue, Saturation, Intensity) to RGB.

## `void hsv_2_rgb(uint H, float S, float V, uint8_t *R, uint8_t *G, uint8_t *B)`
Converts HSV (Hue, Saturation, Value) to RGB. Similar to HSI but with a different approach to brightness.

## `uint32_t increase_brightness(uint32_t *pixel, float inc)`
Multiplies the RGB components of a pixel by a factor to make it brighter. Values are clamped to avoid overflow.

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

## `template <typename T> T tmax(const T &a, const T &b, const T &c)`
Returns the maximum of three values. Works with any comparable type.

## `template <typename T> T tmin(const T &a, const T &b, const T &c)`
Returns the minimum of three values. Same deal.

## `template <typename T> T BE_get_bytes(std::istream &file)`
Reads a value of type T from a stream in big-endian order.

## `template <typename T> T LE_get_bytes(std::istream &file)`
Reads a value of type T from a stream in little-endian order.

## ` template <typename T> T BE_write_as_bytes(std::istream &file, T data)`
Writes a value of type T to a stream in big-endian format, regardless of system endianness.

## ` template <typename T> T LE_write_as_bytes(std::istream &file, T data)`
Same but in little-endian format.