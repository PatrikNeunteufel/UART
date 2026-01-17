/**
 ****************************************************************************************
 * @file   DataFormat.hpp
 * @brief  Data formatting utilities for UART Listener application.
 *
 * @author Patrik Neunteufel
 * @date   Jan 2026
 ****************************************************************************************
 */
#pragma once

#include "Format.hpp"

#include <string>
#include <vector>

namespace uart_listener
{
	std::string bytesToHex(const std::vector<uint8_t>& data);
	std::string bytesToAscii(const std::vector<uint8_t>& data, bool cEscape);
	std::string formatData(const std::vector<uint8_t>& data, OutputFormat fmt);
}