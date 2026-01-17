/**
 ****************************************************************************************
 * @file   Time.cpp
 * @brief  time utilities for UART Listener application.
 *
 * @author Patrik Neunteufel
 * @date   Jan 2026
 ****************************************************************************************
 */

#include "Time.hpp"

#include <chrono>
#include <sstream>  
#include <iomanip>  

namespace uart_listener
{
    std::string getTimestampFileSafe()
    {
        auto   now = std::chrono::system_clock::now();
        time_t now_c = std::chrono::system_clock::to_time_t(now);
        tm     timeinfo;
        localtime_s(&timeinfo, &now_c);

        char buffer[32];
        strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", &timeinfo);
        return std::string(buffer);
    }

    std::string getTimestampWithMs()
    {
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        time_t now_c = std::chrono::system_clock::to_time_t(now);
        tm     timeinfo;
        localtime_s(&timeinfo, &now_c);

        std::ostringstream oss;
        oss << std::put_time(&timeinfo, "%H:%M:%S")
            << "." << std::setfill('0') << std::setw(3) << ms.count();
        return oss.str();
    }
}