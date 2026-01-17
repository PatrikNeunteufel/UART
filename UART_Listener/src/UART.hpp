/**
 ****************************************************************************************
 * @file   UART.hpp
 * @brief  UART handling utilities for UART Listener application.
 *
 * @author Patrik Neunteufel
 * @date   Jan 2026
 ****************************************************************************************
 */
#pragma once

#include <deque>
#include <chrono>
#include <condition_variable>
#include <vector>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <string>
#include <windows.h>

namespace uart_listener
{
    HANDLE openSerialPortReadOnly(const std::string& portName, uint32_t baudRate);

    // ============================================================================
    // Packet Queue (Thread-Safe)
    // ============================================================================

    enum class Channel
    {
        RX = 0,
        TX
    };

    struct Packet
    {
        Channel              channel{};
        std::string          timestamp{};
        std::vector<uint8_t> data{};
    };

    class PacketQueue
    {
    public:
        void push(Packet&& pkt);

        bool pop(Packet& out, std::chrono::milliseconds timeout);

        void notifyStop();

    private:
        std::mutex              m_mutex;
        std::condition_variable m_cv;
        std::deque<Packet>      m_queue;
    };
}