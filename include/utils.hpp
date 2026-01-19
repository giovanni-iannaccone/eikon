#pragma once

#include <concepts>
#include <cstdint>
#include <cstring>
#include <functional>
#include <istream>
#include <random>
#include <type_traits>
#include <unordered_set>

namespace eikon {

namespace utils {
        
enum Axis {
    Y,
    X
};

template <typename T>
concept numeric = std::integral<T> || std::floating_point<T>;

void free_pixels(uint32_t **pixels, uint height);

char get_byte(std::istream &file);
void skip_bytes(std::istream &file, uint bytes);
void write_byte(std::ostream &file, const char data);

inline uint32_t get_hex(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF) {
    return (((a << 8) | r) << 8 | g) << 8 | b;
}

inline void get_rgb(uint32_t pixel, uint8_t &r, uint8_t &g, uint8_t &b) {
    b = (pixel >> (8 * 0)) & 0xFF;
    g = (pixel >> (8 * 1)) & 0xFF;
    r = (pixel >> (8 * 2)) & 0xFF;
}

void alpha_blend_color(uint32_t &c1, const uint32_t &c2);
uint8_t get_pixel_brightness(uint32_t pixel);

void increase_brightness(uint32_t &pixel, float inc);

void hsi_2_rgb(uint H, float S, float I, uint8_t *R, uint8_t *G, uint8_t *B);
void rgb_2_hsi(uint8_t R, uint8_t G, uint8_t B, uint *H, float *S, float *I);

void hsv_2_rgb(uint H, float S, float V, uint8_t *R, uint8_t *G, uint8_t *B);
void rgb_2_hsv(uint8_t R, uint8_t G, uint8_t B, uint *H, float *S, float *v);

void write_repeated(std::ostream &file, uint32_t color, uint8_t reps);

inline std::mt19937 initialize_randomness() {
    return std::mt19937(std::random_device{}());
}

template <typename T>
inline bool in(const T& element, const std::unordered_set<T> &set) {
    return set.find(element) != set.end();
}

template <numeric T>
T max(T a, T b) {
    return a > b ? a : b;
}

template <numeric T, numeric ... Args>
T max(T a, T b, Args ... args) {
   return max(max(a,b), args...);
}

template <numeric T> 
T min(T a, T b) {
    return a < b ? a : b;
}

template <numeric T, numeric ... Args>
T min(T a, T b, Args ... args) {
   return min(min(a,b), args...);
}

namespace be {
    
    template <typename T>
    T get_bytes(std::istream &file) {
        T result = 0;
        char byte {};
        
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
            unsigned char buffer[sizeof(T)];
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
        char byte {};

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
            unsigned char buffer[sizeof(T)];
            for (size_t i = 0; i < sizeof(T); i++)
                buffer[i] = static_cast<uint8_t>((data >> (i * 8)) & 0xFF);

            file.write(reinterpret_cast<const char *>(buffer), sizeof(buffer));
        }
    }
}

namespace cache {

    template <typename T>
    struct Cache {
        T input;
        T output;
    };
    
    template <typename T>
    inline void handle(Cache<T> &ch, T &value, std::function<void (T &)> &f) {
        if (value == ch.input)
            value = ch.output;
        else
            update(ch, value, f);
    }

    template <typename T>
    inline Cache<T> initialize(T value, std::function<void (T &)> &f) {
        Cache<T> ch;
        update(ch, value, f);
        return ch;
    }

    template <typename T>
    inline void update(Cache<T> &ch, T &value, std::function<void (T &)> &f) {
        ch.input = value;
        f(value);
        ch.output = value;
    }
}

} // namespace utils

} // namespace eikon
