/**
 ****************************************************************************************
 * @file   Globals.hpp
 * @brief  Global definitions for UART Listener application.
 *
 * @author Patrik Neunteufel
 * @date   Jan 2026
 ****************************************************************************************
 */
#pragma once

#include "UART.hpp"

#include <atomic>
#include <windows.h>

namespace uart_listener
{
// ============================================================================
// Global State
// ============================================================================

    extern std::atomic<bool> g_stopRequested;
    extern HANDLE g_hStopEvent;  // Manual-reset event to signal stop
    extern PacketQueue g_packetQueue;
}