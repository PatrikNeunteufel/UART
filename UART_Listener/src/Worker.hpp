/**
 ****************************************************************************************
 * @file   Worker.hpp
 * @brief  Worker thread implementations for UART Listener application.
 *
 * @author Patrik Neunteufel
 * @date   Jan 2026
 ****************************************************************************************
 */
#pragma once

#include "UART.hpp"

namespace uart_listener
{
	void serialReaderThread(HANDLE hSerial, Channel channel);
	void keyboardMonitorThread();
	// Alternative: Use Windows Console API directly for more reliable key detection
	void keyboardMonitorThreadWinAPI();
}