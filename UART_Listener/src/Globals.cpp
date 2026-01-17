/**
 ****************************************************************************************
 * @file   Globals.cpp
 * @brief  Global definitions for UART Listener application.
 *
 * @author Patrik Neunteufel
 * @date   Jan 2026
 ****************************************************************************************
 */

#include "Globals.hpp"

namespace uart_listener
{
    std::atomic<bool> g_stopRequested{ false };
    HANDLE g_hStopEvent = NULL;  // Manual-reset event to signal stop
    PacketQueue g_packetQueue;
}