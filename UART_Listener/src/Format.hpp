/**
 ****************************************************************************************
 * @file   Format.hpp
 * @brief  Output format enum for display and logging.
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
 * @brief Output format for data display.
 */
enum class OutputFormat
{
    Ascii = 0, ///< Printable ASCII, non-printable as \xNN
    Hex,       ///< Hexadecimal bytes (e.g., "48 65 6C 6C 6F")
    CEscape,   ///< C-style escapes (\r, \n, \t, \xNN)
    Raw,       ///< Raw bytes (console shows "<raw N bytes>")
    COUNT
};

/**
 * @brief Log file format container.
 */
enum class LogFormat
{
    Text = 0, ///< Plain text log
    Csv,      ///< CSV format with separator
    COUNT
};

// EnumTraits for OutputFormat
template<typename E>
struct FormatMetaTraits;

template<>
struct FormatMetaTraits<OutputFormat>
{
    static constexpr size_t count = static_cast<size_t>(OutputFormat::COUNT);

    // clang-format off
    static constexpr std::array<const char*, count> names =
    {{
        "ascii",
        "hex",
        "c-escape",
        "raw"
    }};
    // clang-format on
};

template<>
struct FormatMetaTraits<LogFormat>
{
    static constexpr size_t count = static_cast<size_t>(LogFormat::COUNT);

    // clang-format off
    static constexpr std::array<const char*, count> names =
    {{
        "text",
        "csv"
    }};
    // clang-format on
};

template<typename E, typename Meta = FormatMetaTraits<E>>
struct FormatTraitsBase
{
    static constexpr size_t count() noexcept
    {
        return Meta::count;
    }

    static std::optional<E> fromString(const std::string& input)
    {
        std::string key = toLower(input);

        // Handle aliases
        if constexpr (std::is_same_v<E, OutputFormat>)
        {
            if (key == "cescape" || key == "c_escape")
            {
                key = "c-escape";
            }
        }

        for (size_t i = 0; i < count(); ++i)
        {
            if (key == Meta::names[i])
            {
                return static_cast<E>(i);
            }
        }
        return std::nullopt;
    }

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

using OutputFormatTraits = FormatTraitsBase<OutputFormat>;
using LogFormatTraits    = FormatTraitsBase<LogFormat>;

}