/**
 ****************************************************************************************
 * @file   Color.hpp
 * @brief  ANSI Color enum with EnumTraits for case-insensitive parsing.
 *
 * @author Patrik Neunteufel
 * @date   Jan 2026
 ****************************************************************************************
 */
#pragma once

#include <algorithm>
#include <array>
#include <cctype>
#include <optional>
#include <string>

namespace uart_listener
{

/**
 * @brief Supported ANSI colors for console output.
 */
enum class Color
{
    Default = 0,
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    Gray,
    BrightRed,
    BrightGreen,
    BrightYellow,
    BrightBlue,
    BrightMagenta,
    BrightCyan,
    BrightWhite,
    COUNT
};

/**
 * @brief Metadata traits for Color enum.
 */
template<typename E>
struct EnumMetaTraits;

template<>
struct EnumMetaTraits<Color>
{
    static constexpr size_t count = static_cast<size_t>(Color::COUNT);

    // clang-format off
    static constexpr std::array<const char*, count> names =
    {{
        "default",
        "black",
        "red",
        "green",
        "yellow",
        "blue",
        "magenta",
        "cyan",
        "white",
        "gray",
        "brightred",
        "brightgreen",
        "brightyellow",
        "brightblue",
        "brightmagenta",
        "brightcyan",
        "brightwhite"
    }};

    static constexpr std::array<const char*, count> ansiCodes =
    {{
        "\033[0m",    // Default (reset)
        "\033[30m",   // Black
        "\033[31m",   // Red
        "\033[32m",   // Green
        "\033[33m",   // Yellow
        "\033[34m",   // Blue
        "\033[35m",   // Magenta
        "\033[36m",   // Cyan
        "\033[37m",   // White
        "\033[90m",   // Gray
        "\033[91m",   // Bright Red
        "\033[92m",   // Bright Green
        "\033[93m",   // Bright Yellow
        "\033[94m",   // Bright Blue
        "\033[95m",   // Bright Magenta
        "\033[96m",   // Bright Cyan
        "\033[97m"    // Bright White
    }};
    // clang-format on
};

/**
 * @brief EnumTraits base template for enum parsing and conversion.
 */
template<typename E, typename Meta = EnumMetaTraits<E>>
struct EnumTraitsBase
{
    static constexpr size_t count() noexcept
    {
        return Meta::count;
    }

    /**
     * @brief Convert string to enum (case-insensitive).
     * @param input String to parse
     * @return Optional enum value, nullopt if not found
     */
    static std::optional<E> fromString(const std::string& input)
    {
        std::string key = toLower(input);
        for (size_t i = 0; i < count(); ++i)
        {
            if (key == Meta::names[i])
            {
                return static_cast<E>(i);
            }
        }
        return std::nullopt;
    }

    /**
     * @brief Get ANSI escape code for color.
     * @param value Color enum value
     * @return ANSI escape sequence string
     */
    static const char* toAnsi(E value)
    {
        const size_t idx = static_cast<size_t>(value);
        if (idx < count())
        {
            return Meta::ansiCodes[idx];
        }
        return "\033[0m";
    }

    /**
     * @brief Get name string for enum value.
     * @param value Enum value
     * @return Name string
     */
    static const char* toString(E value)
    {
        const size_t idx = static_cast<size_t>(value);
        if (idx < count())
        {
            return Meta::names[idx];
        }
        return "unknown";
    }

private:
    static std::string toLower(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });
        return s;
    }
};

using ColorTraits = EnumTraitsBase<Color>;

/**
 * @brief Parse color argument to ANSI code.
 *
 * Accepts either:
 * - Named color (e.g., "green", "RED", "BrightCyan")
 * - Raw ANSI escape (e.g., "\033[32m")
 *
 * @param arg Input argument string
 * @return Optional ANSI escape code, nullopt if invalid
 */
std::optional<std::string> parseColorToAnsi(const std::string& arg);

}