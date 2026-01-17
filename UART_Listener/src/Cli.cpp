/**
 ****************************************************************************************
 * @file   cli.cpp
 * @brief  parsing cli arguments for UART Listener application.
 *
 * @author Patrik Neunteufel
 * @date   Jan 2026
 ****************************************************************************************
 */

#include "Cli.hpp"
#include "Config.hpp"
#include "Color.hpp"

#include <iostream>
#include <string>
#include <algorithm>

namespace uart_listener
{
    void printHelp()
    {
        std::cout <<
            R"(uart_listener - Dual COM port sniffer/logger (RX/TX)

Usage:
  uart_listener [--rx-port N|COMx] [--tx-port N|COMx] [options]

Port Configuration:
  --rx-port N|COMx        RX listening port (e.g., 5 or COM5)
  --tx-port N|COMx        TX listening port (e.g., 6 or COM6)
  --dual-off              Single port mode (only RX or TX required)
  
  In dual mode (default): Both ports are required.
  In single mode: At least one port is required.
  If ports are missing, the app asks interactively at startup.

Serial Options:
  --baud RATE             Baud rate (default: 115200)

Output Format:
  --format FMT            Display format: ascii|hex|c-escape|raw (default: ascii)

Logging:
  --log-format FMT        Log container: text|csv (default: text)
  --log-file PATH         Log file path (default: auto-generated)
  --rx-raw-out PATH       Write raw RX bytes to file
  --tx-raw-out PATH       Write raw TX bytes to file

Display:
  --rx-color COLOR        Color for [RX] tag (e.g., green, cyan, "\033[32m")
  --tx-color COLOR        Color for [TX] tag (e.g., red, yellow)
  --no-ts                 Disable timestamps

Timing:
  --flush-timeout MS      Flush log file interval in ms (default: 250)

Other:
  --help, -h              Show this help

Available Colors:
  default, black, red, green, yellow, blue, magenta, cyan, white, gray,
  brightred, brightgreen, brightyellow, brightblue, brightmagenta,
  brightcyan, brightwhite

Examples:
  uart_listener --rx-port 5 --tx-port 6
  uart_listener --rx-port COM5 --tx-port COM6 --baud 9600 --format hex
  uart_listener --rx-port 5 --tx-port 6 --rx-color green --tx-color red
  uart_listener --rx-port 5 --dual-off                   (RX only)
  uart_listener --tx-port 6 --dual-off                   (TX only)

Press ESC or Q to quit during operation.
)";
    }

    bool isNumber(const std::string& s)
    {
        if (s.empty()) return false;
        for (char c : s)
        {
            if (c < '0' || c > '9') return false;
        }
        return true;
    }

    std::string normalizePortToCOM(const std::string& input)
    {
        std::string s = input;

        // Trim whitespace
        while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front())))
        {
            s.erase(s.begin());
        }
        while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back())))
        {
            s.pop_back();
        }

        if (s.empty())
        {
            return "";
        }

        // Convert to uppercase
        std::string upper = s;
        std::transform(upper.begin(), upper.end(), upper.begin(),
            [](unsigned char c) { return static_cast<char>(std::toupper(c)); });

        // Already starts with COM
        if (upper.rfind("COM", 0) == 0)
        {
            return upper;
        }

        // Just a number
        if (isNumber(upper))
        {
            return "COM" + upper;
        }

        return upper;
    }

    std::string toLower(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return s;
    }

    bool parseArgs(int argc, char* argv[], Config& cfg)
    {
        bool rxSet = false;
        bool txSet = false;

        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            std::string argLow = toLower(arg);

            if (argLow == "--help" || argLow == "-h")
            {
                printHelp();
                return false;
            }
            else if (argLow == "--rx-port")
            {
                if (i + 1 >= argc)
                {
                    std::cerr << "--rx-port requires an argument\n";
                    return false;
                }
                cfg.rxPort = normalizePortToCOM(argv[++i]);
                rxSet = true;
            }
            else if (argLow == "--tx-port")
            {
                if (i + 1 >= argc)
                {
                    std::cerr << "--tx-port requires an argument\n";
                    return false;
                }
                cfg.txPort = normalizePortToCOM(argv[++i]);
                txSet = true;
            }
            else if (argLow == "--baud")
            {
                if (i + 1 >= argc)
                {
                    std::cerr << "--baud requires an argument\n";
                    return false;
                }
                cfg.baudRate = static_cast<uint32_t>(std::stoul(argv[++i]));
                if (cfg.baudRate == 0)
                {
                    std::cerr << "Invalid baud rate\n";
                    return false;
                }
            }
            else if (argLow == "--format")
            {
                if (i + 1 >= argc)
                {
                    std::cerr << "--format requires an argument\n";
                    return false;
                }
                auto fmt = OutputFormatTraits::fromString(argv[++i]);
                if (!fmt.has_value())
                {
                    std::cerr << "Invalid --format: use ascii|hex|c-escape|raw\n";
                    return false;
                }
                cfg.outputFormat = *fmt;
            }
            else if (argLow == "--log-format")
            {
                if (i + 1 >= argc)
                {
                    std::cerr << "--log-format requires an argument\n";
                    return false;
                }
                auto fmt = LogFormatTraits::fromString(argv[++i]);
                if (!fmt.has_value())
                {
                    std::cerr << "Invalid --log-format: use text|csv\n";
                    return false;
                }
                cfg.logFormat = *fmt;
            }
            else if (argLow == "--log-file")
            {
                if (i + 1 >= argc)
                {
                    std::cerr << "--log-file requires a path\n";
                    return false;
                }
                cfg.logFilePath = argv[++i];
            }
            else if (argLow == "--rx-raw-out")
            {
                if (i + 1 >= argc)
                {
                    std::cerr << "--rx-raw-out requires a path\n";
                    return false;
                }
                cfg.rxRawOutPath = argv[++i];
            }
            else if (argLow == "--tx-raw-out")
            {
                if (i + 1 >= argc)
                {
                    std::cerr << "--tx-raw-out requires a path\n";
                    return false;
                }
                cfg.txRawOutPath = argv[++i];
            }
            else if (argLow == "--rx-color")
            {
                if (i + 1 >= argc)
                {
                    std::cerr << "--rx-color requires an argument\n";
                    return false;
                }
                auto ansi = parseColorToAnsi(argv[++i]);
                if (!ansi.has_value())
                {
                    std::cerr << "Invalid --rx-color (use e.g., green or \"\\033[32m\")\n";
                    return false;
                }
                cfg.rxColor = *ansi;
            }
            else if (argLow == "--tx-color")
            {
                if (i + 1 >= argc)
                {
                    std::cerr << "--tx-color requires an argument\n";
                    return false;
                }
                auto ansi = parseColorToAnsi(argv[++i]);
                if (!ansi.has_value())
                {
                    std::cerr << "Invalid --tx-color (use e.g., red or \"\\033[31m\")\n";
                    return false;
                }
                cfg.txColor = *ansi;
            }
            else if (argLow == "--no-ts")
            {
                cfg.timestampsEnabled = false;
            }
            else if (argLow == "--flush-timeout")
            {
                if (i + 1 >= argc)
                {
                    std::cerr << "--flush-timeout requires an argument\n";
                    return false;
                }
                cfg.flushTimeoutMs = static_cast<uint32_t>(std::stoul(argv[++i]));
            }
            else if (argLow == "--dual-off")
            {
                cfg.dualMode = false;
            }
            else
            {
                std::cerr << "Unknown argument: " << arg << "\n";
                std::cerr << "Use --help for usage.\n";
                return false;
            }
        }

        // Interactive fallback for missing ports
        if (cfg.dualMode)
        {
            // Dual mode: both ports required
            if (!rxSet)
            {
                std::cout << "Enter RX COM port (e.g., COM5 or 5): ";
                std::string input;
                std::getline(std::cin, input);
                cfg.rxPort = normalizePortToCOM(input);
            }

            if (!txSet)
            {
                std::cout << "Enter TX COM port (e.g., COM6 or 6): ";
                std::string input;
                std::getline(std::cin, input);
                cfg.txPort = normalizePortToCOM(input);
            }

            if (cfg.rxPort.empty() || cfg.txPort.empty())
            {
                std::cerr << "Both RX and TX ports are required.\n";
                return false;
            }
        }
        else
        {
            // Single port mode: at least one port required
            if (!rxSet && !txSet)
            {
                std::cout << "Enter COM port (e.g., COM5 or 5): ";
                std::string input;
                std::getline(std::cin, input);
                std::string port = normalizePortToCOM(input);

                if (port.empty())
                {
                    std::cerr << "At least one port is required.\n";
                    return false;
                }

                // Ask which channel
                std::cout << "Use as RX or TX? (r/t): ";
                char choice = 'r';
                std::cin >> choice;
                std::cin.ignore(1024, '\n');

                if (choice == 't' || choice == 'T')
                {
                    cfg.txPort = port;
                }
                else
                {
                    cfg.rxPort = port;
                }
            }

            if (cfg.rxPort.empty() && cfg.txPort.empty())
            {
                std::cerr << "At least one port (RX or TX) is required.\n";
                return false;
            }
        }

        return true;
    }
}