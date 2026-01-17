/**
 ****************************************************************************************
 * @file   DataFormat.cpp
 * @brief  Data formatting utilities for UART Listener application.
 *
 * @author Patrik Neunteufel
 * @date   Jan 2026
 ****************************************************************************************
 */

#include "DataFormat.hpp"

#include <iomanip>
#include <sstream>

namespace uart_listener
{

    std::string bytesToHex(const std::vector<uint8_t>& data)
    {
        std::ostringstream oss;
        oss << std::hex << std::uppercase << std::setfill('0');

        for (size_t i = 0; i < data.size(); ++i)
        {
            oss << std::setw(2) << static_cast<int>(data[i]);
            if (i + 1 < data.size())
            {
                oss << " ";
            }
        }
        return oss.str();
    }

    std::string bytesToAscii(const std::vector<uint8_t>& data, bool cEscape)
    {
        std::ostringstream oss;

        for (uint8_t b : data)
        {
            char c = static_cast<char>(b);

            if (cEscape)
            {
                switch (c)
                {
                case '\r': oss << "\\r"; continue;
                case '\n': oss << "\\n"; continue;
                case '\t': oss << "\\t"; continue;
                case '\0': oss << "\\0"; continue;
                default: break;
                }
            }

            if (b >= 32 && b <= 126)
            {
                oss << c;
            }
            else
            {
                oss << "\\x" << std::hex << std::uppercase
                    << std::setw(2) << std::setfill('0')
                    << static_cast<int>(b) << std::dec;
            }
        }
        return oss.str();
    }

    std::string formatData(const std::vector<uint8_t>& data, OutputFormat fmt)
    {
        switch (fmt)
        {
        case OutputFormat::Ascii:
            return bytesToAscii(data, false);
        case OutputFormat::Hex:
            return bytesToHex(data);
        case OutputFormat::CEscape:
            return bytesToAscii(data, true);
        case OutputFormat::Raw:
        {
            std::ostringstream oss;
            oss << "<raw " << data.size() << " bytes>";
            return oss.str();
        }
        default:
            return bytesToHex(data);
        }
    }
}