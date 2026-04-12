#pragma once

#include <concepts>
#include <cstdint>
#include <cstring>
#include <functional>
#include <istream>
#include <random>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

namespace eikon::utils {
        
enum class Axis: char {
    Y = 'y',
    X = 'x'
};

using rgb = std::tuple<uint8_t, uint8_t, uint8_t>;

template <typename T>
constexpr inline T select(bool flag, T a, T b) noexcept {
    if constexpr (std::integral<T>) {
        T mask = -T(flag);
        return (a & mask) | (b & ~mask);
    }
    
    return flag ? a : b;
}

template <typename T, typename ...Args>
constexpr inline T max(const T &a, const T &b, const Args& ... tail) noexcept {
    if constexpr (sizeof...(tail) == 0)
        return select(a > b, a, b);
    else
        return max(max(a, b), tail...);
}

template <typename T, typename ...Args>
constexpr inline T min(const T &a, const T &b, const Args& ... tail) noexcept {
    if constexpr (sizeof...(tail) == 0)
        return select(a < b, a, b);
    else
        return min(min(a, b), tail...);
}

void free_pixels(uint32_t **pixels, uint height);

inline char get_byte(std::istream &file) {
    char byte;
    file.read(&byte, sizeof(char));
    return byte;
}

constexpr inline uint32_t get_hex(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF) noexcept {
    return (static_cast<uint32_t>(a) << 24) |
           (static_cast<uint32_t>(r) << 16) |
           (static_cast<uint32_t>(g) << 8)  |
           static_cast<uint32_t>(b);
}

constexpr inline uint32_t get_hex(rgb chans, uint8_t a = 0xFF) noexcept {
    auto && [r, g, b] = chans;

    return (static_cast<uint32_t>(a) << 24) |
           (static_cast<uint32_t>(r) << 16) |
           (static_cast<uint32_t>(g) << 8)  |
           static_cast<uint32_t>(b);
}

constexpr inline rgb get_rgb(uint32_t pixel) noexcept {
    return std::move(rgb{
        (pixel >> (8 * 2)) & 0xFF,
        (pixel >> (8 * 1)) & 0xFF,
        (pixel >> (8 * 0)) & 0xFF
    });
}

constexpr inline void get_rgb(uint32_t pixel, uint8_t &r, uint8_t &g, uint8_t &b) noexcept {
    r = (pixel >> (8 * 2)) & 0xFF;
    g = (pixel >> (8 * 1)) & 0xFF;
    b = (pixel >> (8 * 0)) & 0xFF;
}

constexpr inline uint8_t get_pixel_brightness(uint32_t pixel) noexcept {
    auto && [r, g, b] = get_rgb(pixel);
    return max(r, g, b);
}

rgb hsi_2_rgb(uint H, float S, float I) noexcept;
rgb hsv_2_rgb(uint H, float S, float V) noexcept;

template <typename T>
constexpr inline bool in(const T &element, const std::unordered_set<T> &set) noexcept {
    return set.contains(element);
}

inline uint32_t increase_brightness(uint32_t &pixel, float inc) noexcept {
    auto [r, g, b] = get_rgb(pixel);

    r = min(255.0f, r * inc);
    g = min(255.0f, g * inc);
    b = min(255.0f, b * inc);

    pixel = get_hex(r, g, b);
    return pixel;
}

inline std::mt19937& initialize_randomness() {
    static std::mt19937 gen([]{
        std::random_device rd;
        return std::mt19937(rd());
    }());

    return gen;
}

std::tuple<uint, float, float> rgb_2_hsi(uint8_t R, uint8_t G, uint8_t B) noexcept;

inline std::tuple<uint, float, float> rgb_2_hsi(rgb chans) noexcept {
    auto && [r, g, b] = chans;
    return rgb_2_hsi(r, g, b);
}

std::tuple<uint, float, float> rgb_2_hsv(uint8_t R, uint8_t G, uint8_t B) noexcept;

inline std::tuple<uint, float, float> rgb_2_hsv(rgb chans) noexcept {
    auto && [r, g, b] = chans;
    return rgb_2_hsv(r, g, b);
}

inline void skip_bytes(std::istream &file, uint bytes) {
    uint curr = file.tellg();
    file.seekg(curr + bytes);
}

inline void write_byte(std::ostream &file, const char data) {
    file.write(&data, sizeof(data));
}

template <typename T>
inline void write_repeated(std::ostream &file, T &&data, uint8_t reps) {

    char *buffer = new char[reps * sizeof(T)];

    for (size_t i = 0; i < reps; i++) [[likely]]
        std::memcpy(buffer, &data, reps * sizeof(T));

    file.write(buffer, reps * sizeof(T));
    delete[] buffer;
}
    
namespace be {
    
    template <typename T>
    T get_bytes(std::istream &file) {
        T result = 0;
        char byte;
        
        for (size_t i = 0; i < sizeof(T); i++) {
            file.read(&byte, sizeof(char));
            (result <<= 8) |= static_cast<T>(static_cast<uint8_t>(byte));
        }
        
        return result;
    }

    template <typename T>
    void write_as_bytes(std::ostream &file, const T &data) {
        if constexpr (std::endian::native == std::endian::big) {
            file.write(reinterpret_cast<const char *>(&data), sizeof(data));
    
        } else {
            uint8_t buffer[sizeof(T)];
            for (size_t i = 0; i < sizeof(T); i++)
                buffer[i] = static_cast<uint8_t>((data >> ((sizeof(T) - 1 - i) * 8)) & 0xFF);

            file.write(reinterpret_cast<const char *>(buffer), sizeof(buffer));
        }
    }
}

namespace le {

    template <typename T>
    T get_bytes(std::istream &file) {
        T result = 0;
        char byte;

        for (size_t i = 0; i < sizeof(T); i++) {
            byte = get_byte(file);
            result |= static_cast<T>(static_cast<uint8_t>(byte)) << (8 * i);
        }

        return result;
    }

    template <typename T>
    void write_as_bytes(std::ostream &file, const T &data) {
        if constexpr (std::endian::native == std::endian::little) {
            file.write(reinterpret_cast<const char *>(&data), sizeof(data));
        
        } else {
            uint8_t buffer[sizeof(T)];
            for (size_t i = 0; i < sizeof(T); i++)
                buffer[i] = static_cast<uint8_t>((data >> (i * 8)) & 0xFF);

            file.write(reinterpret_cast<const char *>(buffer), sizeof(buffer));
        }
    }
}

template <typename T>
class Cache {

private:
    std::unordered_map<T, T> values;

public:
    explicit Cache(size_t size = 1024) {
        values.reserve(size);
    }

    template <std::invocable<T&> F>
    inline void handle(F&& f, T& value) {
        auto it = values.find(value);

        if (it != values.end()) {
            value = it->second;
            return;
        }

        T key = value;
        std::invoke(f, value);
        values.emplace(std::move(key), value);
    }

    template <std::invocable<T&> F>
    inline void update(F&& f, T& value) {
        T key = value;

        std::invoke(f, value);
        values.insert_or_assign(std::move(key), value);
    }
};

} // namespace utils
