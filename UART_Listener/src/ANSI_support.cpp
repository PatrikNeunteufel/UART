/**
 ****************************************************************************************
 * @file   ANSI_support.cpp
 * @brief  ANSI escape code support utilities for UART Listener application.
 *
 * @author Patrik Neunteufel
 * @date   Jan 2026
 ****************************************************************************************
 */

#include "ANSI_support.hpp"
#include <windows.h>

bool enableVirtualTerminalProcessing()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE || hOut == nullptr)
    {
        return false;
    }

    DWORD mode = 0;
    if (!GetConsoleMode(hOut, &mode))
    {
        return false;
    }

    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    return SetConsoleMode(hOut, mode) != 0;
}