/**
 ****************************************************************************************
 * @file   UART.cpp
 * @brief  UART handling utilities for UART Listener application.
 *
 * @author Patrik Neunteufel
 * @date   Jan 2026
 ****************************************************************************************
 */

#include "UART.hpp"
#include "Globals.hpp"

namespace uart_listener
{
    HANDLE openSerialPortReadOnly(const std::string& portName, uint32_t baudRate)
    {
        HANDLE hSerial = CreateFileA(
            portName.c_str(),
            GENERIC_READ,
            0,
            nullptr,
            OPEN_EXISTING,
            FILE_FLAG_OVERLAPPED,  // Enable overlapped I/O
            nullptr);

        if (hSerial == INVALID_HANDLE_VALUE)
        {
            DWORD err = GetLastError();
            std::cerr << "Error opening port " << portName
                << " (Error code: " << err << ")\n";
            return INVALID_HANDLE_VALUE;
        }

        DCB dcbSerialParams{};
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

        if (!GetCommState(hSerial, &dcbSerialParams))
        {
            std::cerr << "Error reading port parameters: " << portName << "\n";
            CloseHandle(hSerial);
            return INVALID_HANDLE_VALUE;
        }

        dcbSerialParams.BaudRate = baudRate;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.StopBits = ONESTOPBIT;
        dcbSerialParams.Parity = NOPARITY;

        if (!SetCommState(hSerial, &dcbSerialParams))
        {
            std::cerr << "Error setting port parameters: " << portName << "\n";
            CloseHandle(hSerial);
            return INVALID_HANDLE_VALUE;
        }

        // Timeouts for overlapped mode
        COMMTIMEOUTS timeouts{};
        timeouts.ReadIntervalTimeout = MAXDWORD;
        timeouts.ReadTotalTimeoutConstant = 0;
        timeouts.ReadTotalTimeoutMultiplier = 0;
        SetCommTimeouts(hSerial, &timeouts);

        return hSerial;
    }


    void PacketQueue::push(Packet&& pkt)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push_back(std::move(pkt));
        }
        m_cv.notify_one();
    }

    bool PacketQueue::pop(Packet& out, std::chrono::milliseconds timeout)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        if (!m_cv.wait_for(lock, timeout, [this] {
            return g_stopRequested.load() || !m_queue.empty();
            }))
        {
            return false; // Timeout
        }

        if (m_queue.empty())
        {
            return false;
        }

        out = std::move(m_queue.front());
        m_queue.pop_front();
        return true;
    }

    void PacketQueue::notifyStop()
    {
        m_cv.notify_all();
    }
}