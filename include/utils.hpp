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

template <typename T>
concept numeric = std::integral<T> || std::floating_point<T>;

using rgb = std::tuple<uint8_t, uint8_t, uint8_t>;

template <numeric T>
constexpr inline T max(T a, T b) noexcept {
    return a > b ? a : b;
}

template <numeric T, numeric ... Args>
constexpr inline T max(T a, T b, Args ... args) noexcept {
   return max(max(a,b), args...);
}

template <numeric T> 
constexpr inline T min(T a, T b) noexcept {
    return a < b ? a : b;
}

template <numeric T, numeric ... Args>
constexpr inline T min(T a, T b, Args ... args) noexcept {
   return min(min(a,b), args...);
}

void free_pixels(uint32_t **pixels, uint height);

inline char get_byte(std::istream &file) {
    char byte {};
    file.read(&byte, sizeof(char));
    return byte;
}

constexpr inline uint32_t get_hex(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF) noexcept {
    return (((a << 8) | r) << 8 | g) << 8 | b;
}

constexpr inline uint32_t get_hex(rgb chans, uint8_t a = 0xFF) noexcept {
    auto [r, g, b] = chans;
    return (((a << 8) | r) << 8 | g) << 8 | b;
}

constexpr inline rgb get_rgb(uint32_t pixel) noexcept {
    return {
        (pixel >> (8 * 2)) & 0xFF,
        (pixel >> (8 * 1)) & 0xFF,
        (pixel >> (8 * 0)) & 0xFF
    };
}

constexpr inline uint8_t get_pixel_brightness(uint32_t pixel) noexcept {
    return std::apply([] (auto... vals) {
        return max(vals...);
    }, get_rgb(pixel));
}

rgb hsi_2_rgb(uint H, float S, float I);
rgb hsv_2_rgb(uint H, float S, float V);

template <typename T>
constexpr inline bool in(const T& element, const std::unordered_set<T> &set) noexcept {
    return set.find(element) != set.end();
}

inline uint32_t increase_brightness(uint32_t &pixel, float inc) noexcept {
    auto [r, g, b] = get_rgb(pixel);

    r = min(255.0f, r * inc);
    g = min(255.0f, g * inc);
    b = min(255.0f, b * inc);

    pixel = get_hex(r, g, b);
    return pixel;
}

inline std::mt19937 initialize_randomness() {
    static std::random_device rd;
    return std::mt19937(rd());
}

std::tuple<uint, float, float> rgb_2_hsi(uint8_t R, uint8_t G, uint8_t B);

inline std::tuple<uint, float, float> rgb_2_hsi(rgb chans) {
    auto [r, g, b] = chans;
    return rgb_2_hsi(r, g, b);
}

std::tuple<uint, float, float> rgb_2_hsv(uint8_t R, uint8_t G, uint8_t B);

inline std::tuple<uint, float, float> rgb_2_hsv(rgb chans) {
    auto [r, g, b] = chans;
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

    for (size_t i = 0; i < reps; i++)
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
    void write_as_bytes(std::ostream &file, T data) {
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
    void write_as_bytes(std::ostream &file, T data) {
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
    Cache(size_t size = 1024) {
        values.reserve(size);
    }

    template <std::invocable<T&> F>
    inline void handle(F &&f, T &value) noexcept {
        auto [it, inserted] = values.try_emplace(value);
        
        if (!inserted) {
            value = it->second;
            return;
        }
        
        T prev = value;
        std::invoke(std::forward<F>(f), value);
        it->second = value;
    }
    
    template <std::invocable<T&> F>
    inline void update(F &&f, T &value) noexcept {
        T prev = value;
        std::invoke(std::forward<F>(f), value);
        values.emplace(std::move(prev), value);
    }
};
    
} // namespace utils
