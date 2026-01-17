/**
 ****************************************************************************************
 * @file   config.hpp
 * @brief  configuration structure for UART Listener application.
 *
 * @author Patrik Neunteufel
 * @date   Jan 2026
 ****************************************************************************************
 */
#pragma once

#include "Format.hpp"

namespace uart_listener
{
    struct Config
    {
        std::string rxPort;
        std::string txPort;
        uint32_t    baudRate = 115200;
        OutputFormat outputFormat = OutputFormat::Ascii;
        LogFormat   logFormat = LogFormat::Text;
        bool        timestampsEnabled = true;
        uint32_t    flushTimeoutMs = 250;
        bool        dualMode = true;  // false = single port mode (--dual-off)

        std::optional<std::string> rxColor;
        std::optional<std::string> txColor;
        std::optional<std::string> logFilePath;
        std::optional<std::string> rxRawOutPath;
        std::optional<std::string> txRawOutPath;
    };
}