#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <istream>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#if defined(_MSC_VER)

    #define FORCE_INLINE __forceinline

    #define FAST_LOOP \
        __pragma(loop(ivdep)) \
        __pragma(loop(hint_parallel(8)))

    #define HOT

#elif defined(__clang__)

    #define FORCE_INLINE \
        __attribute__((always_inline)) inline

    #define FAST_LOOP \
        _Pragma("clang loop vectorize(enable)") \
        _Pragma("clang loop interleave(enable)") \
        _Pragma("clang loop unroll(enable)")

    #define HOT __attribute__((hot))

#elif defined(__GNUC__)

    #define FORCE_INLINE \
        __attribute__((always_inline)) inline

    #define FAST_LOOP \
        _Pragma("GCC ivdep") \
        _Pragma("GCC unroll 8")

    #define HOT __attribute__((hot))

#else

    #define FORCE_INLINE inline
    #define FAST_LOOP
    #define HOT

#endif

namespace eikon::utils {

enum class Axis: char {
    Y = 'y',
    X = 'x'
};

struct hsi {
    uint h;
    float s;
    float i;
};

struct hsv {
    uint h;
    float s;
    float i;
};

struct rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

constexpr uint32_t ABGR(uint32_t pixel) noexcept
{
    return ((pixel & 0x00FF0000) >> 16) | ((pixel & 0x000000FF) << 16);
}

constexpr uint32_t ARGB(uint32_t pixel) noexcept
{
    return pixel;
}

constexpr uint32_t BGR(uint32_t pixel) noexcept
{
    return ((pixel & 0x00FF0000) >> 16) | ((pixel & 0x000000FF) << 16) | 0xFF000000;
}

constexpr uint32_t RGB(uint32_t pixel) noexcept
{
    return pixel | 0xFF000000;
}

constexpr uint32_t L(uint8_t pixel) noexcept
{
    return 0xFF000000 | (pixel << 16) | (pixel << 8) | pixel;
}

constexpr uint32_t RGBA(uint32_t pixel) noexcept
{
    return ((pixel & 0xFFFFFF00) >> 8) | (pixel << 24);
}

template <typename T>
HOT constexpr T select(bool flag, T a, T b) noexcept
{
    if constexpr (!std::integral<T>) {
        return flag ? a : b;

    } else {
        const T mask = -T(flag);
        return (a & mask) | (b & ~mask);
    }
}

void free_pixels(uint32_t **pixels, uint height);

inline char get_byte(std::istream &file)
{
    char b;
    file.read(&b,1);
    return b;
}

HOT constexpr uint32_t get_hex(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF) noexcept
{
    return (static_cast<uint32_t>(a) << 24) |
           (static_cast<uint32_t>(r) << 16) |
           (static_cast<uint32_t>(g) << 8)  |
           static_cast<uint32_t>(b);
}

HOT constexpr uint32_t get_hex(rgb chans, uint8_t a = 0xFF) noexcept
{
    auto && [r, g, b] = chans;

    return (static_cast<uint32_t>(a) << 24) |
           (static_cast<uint32_t>(r) << 16) |
           (static_cast<uint32_t>(g) << 8)  |
           static_cast<uint32_t>(b);
}

HOT constexpr rgb get_rgb(uint32_t pixel) noexcept
{
    uint8_t r = (pixel >> (8 * 2)) & 0xFF;
    uint8_t g = (pixel >> (8 * 1)) & 0xFF;
    uint8_t b = (pixel >> (8 * 0)) & 0xFF;
    
    return {r, g, b};
}

HOT constexpr void get_rgb(uint32_t pixel, uint8_t &r, uint8_t &g, uint8_t &b) noexcept
{
    r = (pixel >> (8 * 2)) & 0xFF;
    g = (pixel >> (8 * 1)) & 0xFF;
    b = (pixel >> (8 * 0)) & 0xFF;
}

constexpr uint8_t get_pixel_brightness(uint32_t pixel) noexcept
{
    auto && [r, g, b] = get_rgb(pixel);
    return std::max({r, g, b});
}

rgb hsi_2_rgb(uint H, float S, float I) noexcept;
rgb hsv_2_rgb(uint H, float S, float V) noexcept;

template <typename T>
constexpr bool in(const T &element, const std::unordered_set<T> &set) noexcept
{
    return set.contains(element);
}

inline uint32_t increase_brightness(uint32_t &pixel, float inc) noexcept
{
    auto && [r, g, b] = get_rgb(pixel);

    r = std::min(255.0f, r * inc);
    g = std::min(255.0f, g * inc);
    b = std::min(255.0f, b * inc);

    pixel = get_hex(r, g, b);
    return pixel;
}

inline uint8_t random()
{
    thread_local static std::mt19937 gen ([]{
        std::random_device rd;
        return std::mt19937(rd());
    } ());

    return gen();
}

hsi rgb_2_hsi(uint8_t R, uint8_t G, uint8_t B) noexcept;

inline hsi rgb_2_hsi(rgb chans) noexcept
{
    auto && [r, g, b] = chans;
    return rgb_2_hsi(r, g, b);
}

hsv rgb_2_hsv(uint8_t R, uint8_t G, uint8_t B) noexcept;

inline hsv rgb_2_hsv(rgb chans) noexcept
{
    auto && [r, g, b] = chans;
    return rgb_2_hsv(r, g, b);
}

inline void skip_bytes(std::istream &file, uint bytes)
{
    file.ignore(bytes);
}

inline void write_byte(std::ostream &file, const char data)
{
    file.write(&data, sizeof(data));
}

template <typename T>
inline void write_repeated(std::ostream &file, T &&data, uint8_t reps)
{
    for (uint8_t i=0; i < reps; ++i)
        file.write(reinterpret_cast<const char*>(&data), sizeof(T));
}
    
namespace be {
    
    template <typename T>
    T get_bytes(std::istream &file)
    {
        T result = 0;
        char byte;
        
        for (size_t i = 0; i < sizeof(T); i++) {
            file.read(&byte, sizeof(char));
            (result <<= 8) |= static_cast<T>(static_cast<uint8_t>(byte));
        }
        
        return result;
    }

    template <typename T>
    void write_as_bytes(std::ostream &file, const T &data)
    {
        if constexpr (std::endian::native == std::endian::big) {
            file.write(reinterpret_cast<const char *>(&data), sizeof(data));
            return;
        } 

        uint8_t buffer[sizeof(T)];
        for (size_t i = 0; i < sizeof(T); i++)
            buffer[i] = static_cast<uint8_t>((data >> ((sizeof(T) - 1 - i) * 8)) & 0xFF);
        
        file.write(reinterpret_cast<const char *>(buffer), sizeof(buffer));
    }
}

namespace le {

    template <typename T>
    T get_bytes(std::istream &file)
    {
        T result = 0;

        for (size_t i = 0; i < sizeof(T); i++) {
            uint8_t byte = static_cast<uint8_t>(file.get());
            result |= static_cast<T>(byte) << (8 * i);
        }

        return result;
    }

    template <typename T>
    void write_as_bytes(std::ostream &file, const T &data)
    {
        if constexpr (std::endian::native == std::endian::little) {
            file.write(reinterpret_cast<const char *>(&data), sizeof(data));
            return;
        }
        
        uint8_t buffer[sizeof(T)];
        for (size_t i = 0; i < sizeof(T); i++)
            buffer[i] = static_cast<uint8_t>((data >> (i * 8)) & 0xFF);
        
        file.write(reinterpret_cast<const char *>(buffer), sizeof(buffer));
    }
}

enum UseCache {
    True,
    False
};

template <typename T, typename Key, size_t Size = 1024>
class Cache {

private:
    using K = std::decay_t<Key>;

    mutable std::unordered_map<K, T> values;
    std::function<T(Key)> func;

public:
    template <typename F>
    requires std::convertible_to<std::invoke_result_t<F&, Key>, T>
    explicit Cache(F&& f)
        : func(std::forward<F>(f)) {
        values.reserve(Size);
    }

    HOT const T& operator[](const K& value) const
    {
        auto it = values.find(value);

        if (it != values.end()) [[likely]]
            return it->second;

        auto [new_it, inserted] = values.emplace(
            value,
            func(value)
        );

        return new_it->second;
    }

    auto find(const K& value) const noexcept
    {
        return values.find(value);
    }
};

} // namespace utils
