# UART Tools

A collection of Windows command-line utilities for UART communication, debugging, and analysis.

## Overview

This repository contains various tools for working with UART/serial communication. Each tool is designed for a specific use case and can be built independently.

## Tools

| Tool | Description | Status |
|------|-------------|--------|
| [UART_Listener](UART_Listener/README.md) | Dual COM port sniffer for monitoring RX/TX communication | ✅ Stable |

## Technical Highlights

All tools in this collection share common design principles:

- **Modern C++20** — Concepts, `requires` expressions, `std::optional`, `std::filesystem`
- **Multithreading** — Thread-safe queues with `std::mutex`, `std::condition_variable`, `std::atomic`
- **Windows System Programming** — Overlapped I/O, COM port API, Console handling
- **Clean Architecture** — Modular design with separated concerns
- **Documentation** — Bilingual docs (English/German), Doxygen-style comments

## Building

### Requirements

- Windows 10/11
- Visual Studio 2019/2022 with C++20 support
- Or: MinGW-w64 with C++20

### Using Visual Studio

1. Open `UART.sln`
2. Select configuration (Debug/Release)
3. Build Solution (F7)

### Using Command Line

```bash
# Navigate to specific tool
cd UART_Listener

# Build with MSVC
cl /EHsc /std:c++20 src/*.cpp /Fe:uart_listener.exe
```

## Repository Structure

```
UART/
├── UART.sln                 # Visual Studio Solution
├── README.md                # This file
│
└── UART_Listener/           # Dual port UART sniffer
    ├── README.md
    ├── src/
    └── docs/
        ├── en/              # English documentation
        └── de/              # German documentation
```

## Possible Tools

- **UART_Bridge** — Bidirectional bridge between two COM ports with logging
- **UART_Injector** — Send test patterns or recorded data to a COM port
- **UART_Analyzer** — Protocol decoder for common UART-based protocols (Modbus, etc.)

## License

MIT License

## Author

Patrik Neunteufel
