#pragma once

#include <algorithm>
#include <string_view>

#include <eikon/utils.hpp>

template <size_t N>
struct FixedString
{
    char value[N];

    constexpr FixedString(const char (&str)[N])
    {
        std::copy_n(str, N, value);
    }

    constexpr std::string_view view() const noexcept
    {
        return {value, N - 1};
    }
};

constexpr uint32_t RGBMask = 0x00FFFFFF;

FORCE_INLINE constexpr
bool maskedcmp(uint32_t a, uint32_t b) noexcept
{
    return (a & RGBMask) != (b & RGBMask);
}
