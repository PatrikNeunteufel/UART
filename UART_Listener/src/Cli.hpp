/**
 ****************************************************************************************
 * @file   cli.hpp
 * @brief  handle cli arguments for UART Listener application.
 *
 * @author Patrik Neunteufel
 * @date   Jan 2026
 ****************************************************************************************
 */
#pragma once

#include "Config.hpp"

#include <string>

namespace uart_listener
{
	void printHelp();
	bool isNumber(const std::string& s);
	std::string normalizePortToCOM(const std::string& input);
	std::string toLower(std::string s);
	bool parseArgs(int argc, char* argv[], Config& cfg);

}