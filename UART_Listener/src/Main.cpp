/**
 ****************************************************************************************
 * @file   main.cpp
 * @brief  Dual UART Listener - Listens on two COM ports (RX/TX) and logs tagged output.
 *
 *         Features:
 *         - Separate RX and TX port monitoring
 *         - Configurable output formats (ascii, hex, c-escape, raw)
 *         - Optional ANSI color coding per channel
 *         - Text or CSV log output
 *         - Optional raw binary dump files
 *         - Timestamps with millisecond precision
 *
 * @author Patrik Neunteufel
 * @date   Jan 2026
 ****************************************************************************************
 */

#define _CRT_SECURE_NO_WARNINGS

#include "Color.hpp"
#include "Config.hpp"
#include "Cli.hpp"
#include "Format.hpp"
#include "Time.hpp"
#include "UART.hpp"
#include"DataFormat.hpp"
#include "Worker.hpp"
#include "ANSI_support.hpp"
#include "Globals.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <conio.h>
#include <cstdint>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <optional>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

namespace fs = std::filesystem;
using namespace uart_listener;

int main(int argc, char* argv[])
{
    Config cfg;

    if (!parseArgs(argc, argv, cfg))
    {
        return (argc > 1 && (toLower(argv[1]) == "--help" || toLower(argv[1]) == "-h"))
                   ? 0
                   : 1;
    }

    // Enable ANSI colors on Windows
    bool ansiEnabled = enableVirtualTerminalProcessing();
    if (!ansiEnabled)
    {
        std::cerr << "Warning: Could not enable ANSI colors (Virtual Terminal Processing).\n";
        if (cfg.rxColor.has_value() || cfg.txColor.has_value())
        {
            std::cerr << "         Color output may not work correctly.\n";
        }
    }
    else
    {
        std::cout << "[INFO] ANSI color support enabled.\n";
    }

    // Create stop event (manual-reset, initially non-signaled)
    g_hStopEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    if (g_hStopEvent == NULL)
    {
        std::cerr << "Failed to create stop event\n";
        return 1;
    }

    // Open serial ports (conditionally based on mode)
    HANDLE hRx = INVALID_HANDLE_VALUE;
    HANDLE hTx = INVALID_HANDLE_VALUE;

    if (!cfg.rxPort.empty())
    {
        const std::string rxWinPort = "\\\\.\\" + cfg.rxPort;
        std::cout << "[INFO] Opening " << cfg.rxPort << " for RX... " << std::flush;
        hRx = openSerialPortReadOnly(rxWinPort, cfg.baudRate);
        if (hRx == INVALID_HANDLE_VALUE)
        {
            std::cout << "FAILED\n";
            CloseHandle(g_hStopEvent);
            return 1;
        }
        std::cout << "OK\n";
    }

    if (!cfg.txPort.empty())
    {
        const std::string txWinPort = "\\\\.\\" + cfg.txPort;
        std::cout << "[INFO] Opening " << cfg.txPort << " for TX... " << std::flush;
        hTx = openSerialPortReadOnly(txWinPort, cfg.baudRate);
        if (hTx == INVALID_HANDLE_VALUE)
        {
            std::cout << "FAILED\n";
            if (hRx != INVALID_HANDLE_VALUE) CloseHandle(hRx);
            CloseHandle(g_hStopEvent);
            return 1;
        }
        std::cout << "OK\n";
    }

    // Prepare log file path
    char    exePath[MAX_PATH];
    GetModuleFileNameA(nullptr, exePath, MAX_PATH);
    fs::path exeDir = fs::path(exePath).parent_path();

    std::string logPath;
    if (cfg.logFilePath.has_value())
    {
        logPath = *cfg.logFilePath;
    }
    else
    {
        std::string ts       = getTimestampFileSafe();
        std::string ext      = (cfg.logFormat == LogFormat::Csv) ? ".csv" : ".log";
        std::string portPart;
        if (!cfg.rxPort.empty() && !cfg.txPort.empty())
        {
            portPart = cfg.rxPort + "_" + cfg.txPort;
        }
        else if (!cfg.rxPort.empty())
        {
            portPart = cfg.rxPort + "_RX";
        }
        else
        {
            portPart = cfg.txPort + "_TX";
        }
        std::string fileName = "uart_" + portPart + "_" + ts + ext;
        logPath = (exeDir / fileName).string();
    }

    // Open log file
    std::ofstream logFile(logPath, std::ios::out);
    bool          loggingEnabled = true;

    if (!logFile.is_open())
    {
        std::cerr << "Error creating log file: " << logPath << "\n";
        std::cerr << "System error: " << strerror(errno) << "\n";
        std::cout << "Continue without logging? (y/n): ";
        char choice = 'n';
        std::cin >> choice;
        std::cin.ignore(1024, '\n');

        if (choice != 'y' && choice != 'Y')
        {
            CloseHandle(hRx);
            CloseHandle(hTx);
            return 1;
        }
        loggingEnabled = false;
    }
    else
    {
        std::cout << "Log file: " << logPath << "\n";

        // Write CSV header if needed
        if (cfg.logFormat == LogFormat::Csv)
        {
            logFile << "Timestamp;Channel;Data\n";
        }
    }

    // Open raw output files (optional)
    std::ofstream rxRawFile;
    std::ofstream txRawFile;

    if (cfg.rxRawOutPath.has_value())
    {
        rxRawFile.open(*cfg.rxRawOutPath, std::ios::binary);
        if (!rxRawFile.is_open())
        {
            std::cerr << "Warning: Failed to open RX raw output: "
                      << *cfg.rxRawOutPath << "\n";
            cfg.rxRawOutPath.reset();
        }
        else
        {
            std::cout << "RX raw output: " << *cfg.rxRawOutPath << "\n";
        }
    }

    if (cfg.txRawOutPath.has_value())
    {
        txRawFile.open(*cfg.txRawOutPath, std::ios::binary);
        if (!txRawFile.is_open())
        {
            std::cerr << "Warning: Failed to open TX raw output: "
                      << *cfg.txRawOutPath << "\n";
            cfg.txRawOutPath.reset();
        }
        else
        {
            std::cout << "TX raw output: " << *cfg.txRawOutPath << "\n";
        }
    }

    // Start worker threads (conditionally)
    std::thread rxThread;
    std::thread txThread;
    
    if (hRx != INVALID_HANDLE_VALUE)
    {
        rxThread = std::thread(serialReaderThread, hRx, Channel::RX);
    }
    if (hTx != INVALID_HANDLE_VALUE)
    {
        txThread = std::thread(serialReaderThread, hTx, Channel::TX);
    }
    std::thread kbThread(keyboardMonitorThread);  // Use conio.h version

    // Display info
    std::cout << "\n"
              << "========================================\n";
    if (!cfg.rxPort.empty() && !cfg.txPort.empty())
    {
        std::cout << "Mode: Dual (RX + TX)\n"
                  << "RX Port: " << cfg.rxPort << "\n"
                  << "TX Port: " << cfg.txPort << "\n";
    }
    else if (!cfg.rxPort.empty())
    {
        std::cout << "Mode: Single (RX only)\n"
                  << "RX Port: " << cfg.rxPort << "\n";
    }
    else
    {
        std::cout << "Mode: Single (TX only)\n"
                  << "TX Port: " << cfg.txPort << "\n";
    }
    std::cout << "Baud: " << cfg.baudRate << "\n"
              << "Format: " << OutputFormatTraits::toString(cfg.outputFormat) << "\n"
              << "Log format: " << LogFormatTraits::toString(cfg.logFormat) << "\n"
              << "========================================\n";

    // Show colored port status as first "messages"
    const std::string ansiReset = "\033[0m";
    const std::string rxTagColor = cfg.rxColor.value_or("");
    const std::string txTagColor = cfg.txColor.value_or("");

    std::cout << "\n";
    if (!cfg.rxPort.empty())
    {
        if (!rxTagColor.empty())
        {
            std::cout << rxTagColor << "[RX]" << ansiReset << " Port " << cfg.rxPort << " ready (color test)\n";
        }
        else
        {
            std::cout << "[RX] Port " << cfg.rxPort << " ready (no color)\n";
        }
    }

    if (!cfg.txPort.empty())
    {
        if (!txTagColor.empty())
        {
            std::cout << txTagColor << "[TX]" << ansiReset << " Port " << cfg.txPort << " ready (color test)\n";
        }
        else
        {
            std::cout << "[TX] Port " << cfg.txPort << " ready (no color)\n";
        }
    }

    std::cout << "\nPress ESC or Q to quit.\n"
              << "----------------------------------------\n" << std::flush;

    // Flush timing
    auto lastFlush = std::chrono::steady_clock::now();

    // Main processing loop
    while (!g_stopRequested.load())
    {
        Packet pkt;
        if (!g_packetQueue.pop(pkt, std::chrono::milliseconds(100)))
        {
            continue;
        }

        // Write raw bytes if enabled
        if (pkt.channel == Channel::RX && cfg.rxRawOutPath.has_value() && rxRawFile.is_open())
        {
            rxRawFile.write(
                reinterpret_cast<const char*>(pkt.data.data()),
                static_cast<std::streamsize>(pkt.data.size()));
        }
        else if (pkt.channel == Channel::TX && cfg.txRawOutPath.has_value() && txRawFile.is_open())
        {
            txRawFile.write(
                reinterpret_cast<const char*>(pkt.data.data()),
                static_cast<std::streamsize>(pkt.data.size()));
        }

        // Format data
        std::string payload = formatData(pkt.data, cfg.outputFormat);
        const char* tag     = (pkt.channel == Channel::RX) ? "[RX]" : "[TX]";

        // Build console line (with colors)
        std::ostringstream consoleLine;
        if (cfg.timestampsEnabled)
        {
            consoleLine << pkt.timestamp << " ";
        }

        if (pkt.channel == Channel::RX && !rxTagColor.empty())
        {
            consoleLine << rxTagColor << tag << ansiReset << " ";
        }
        else if (pkt.channel == Channel::TX && !txTagColor.empty())
        {
            consoleLine << txTagColor << tag << ansiReset << " ";
        }
        else
        {
            consoleLine << tag << " ";
        }
        consoleLine << payload;

        std::cout << consoleLine.str() << "\n";

        // Build log line (no colors)
        if (loggingEnabled && logFile.is_open())
        {
            if (cfg.logFormat == LogFormat::Csv)
            {
                // CSV: Timestamp;Channel;Data
                logFile << pkt.timestamp << ";"
                        << (pkt.channel == Channel::RX ? "RX" : "TX") << ";"
                        << payload << "\n";
            }
            else
            {
                // Text format
                if (cfg.timestampsEnabled)
                {
                    logFile << pkt.timestamp << " ";
                }
                logFile << tag << " " << payload << "\n";
            }

            // Periodic flush
            auto now = std::chrono::steady_clock::now();
            auto dt  = std::chrono::duration_cast<std::chrono::milliseconds>(
                          now - lastFlush).count();

            if (dt >= static_cast<long long>(cfg.flushTimeoutMs))
            {
                logFile.flush();
                lastFlush = now;
            }
        }
    }

    std::cout << "\n\n[INFO] Shutting down...\n" << std::flush;

    // Signal stop to all threads
    g_stopRequested.store(true);
    SetEvent(g_hStopEvent);
    g_packetQueue.notifyStop();

    // Wait for threads with timeout info
    if (rxThread.joinable())
    {
        std::cout << "[INFO] Waiting for RX thread..." << std::flush;
        rxThread.join();
        std::cout << " done\n" << std::flush;
    }

    if (txThread.joinable())
    {
        std::cout << "[INFO] Waiting for TX thread..." << std::flush;
        txThread.join();
        std::cout << " done\n" << std::flush;
    }

    std::cout << "[INFO] Waiting for keyboard thread..." << std::flush;
    if (kbThread.joinable()) kbThread.join();
    std::cout << " done\n" << std::flush;

    // Close files
    if (rxRawFile.is_open()) rxRawFile.close();
    if (txRawFile.is_open()) txRawFile.close();

    if (loggingEnabled && logFile.is_open())
    {
        logFile.flush();
        logFile.close();
    }

    // Close serial ports
    if (hRx != INVALID_HANDLE_VALUE) CloseHandle(hRx);
    if (hTx != INVALID_HANDLE_VALUE) CloseHandle(hTx);

    // Close stop event
    if (g_hStopEvent != NULL)
    {
        CloseHandle(g_hStopEvent);
        g_hStopEvent = NULL;
    }

    std::cout << "[INFO] Program terminated successfully.\n" << std::flush;

    return 0;
}