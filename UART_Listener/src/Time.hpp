/**
 ****************************************************************************************
 * @file   Time.hpp
 * @brief  time utilities for UART Listener application.
 *
 * @author Patrik Neunteufel
 * @date   Jan 2026
 ****************************************************************************************
 */
#pragma once

#include <string>

namespace uart_listener
{
	std::string getTimestampFileSafe();
	std::string getTimestampWithMs();
}