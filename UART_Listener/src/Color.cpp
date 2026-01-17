/**
 ****************************************************************************************
 * @file   Color.cpp
 * @brief  Implementation of Color parsing utilities.
 *
 * @author Patrik Neunteufel
 * @date   Jan 2026
 ****************************************************************************************
 */

#include "Color.hpp"

namespace uart_listener
{

std::optional<std::string> parseColorToAnsi(const std::string& arg)
{
    if (arg.empty())
    {
        return std::nullopt;
    }

    // Check if already an ESC character (0x1B)
    if (static_cast<unsigned char>(arg[0]) == 0x1B)
    {
        return arg;
    }

    // Handle literal escape sequences from command line
    // e.g., "\033[32m" or "\x1b[32m" passed as string
    if (arg.size() >= 4)
    {
        bool isOctalEscape = (arg.substr(0, 4) == "\\033");
        bool isHexEscape   = (arg.substr(0, 4) == "\\x1b" || arg.substr(0, 4) == "\\x1B");

        if (isOctalEscape || isHexEscape)
        {
            std::string result;
            result += static_cast<char>(0x1B);
            result += arg.substr(4);
            return result;
        }
    }

    // Try named color lookup (case-insensitive)
    auto colorOpt = ColorTraits::fromString(arg);
    if (colorOpt.has_value())
    {
        return std::string(ColorTraits::toAnsi(*colorOpt));
    }

    return std::nullopt;
}

}