# UART Listener

Dual COM port sniffer for monitoring RX/TX UART communication on Windows.

## Features

- Listen on two COM ports simultaneously (RX/TX channels)
- Single-port mode with `--dual-off` option
- Color-coded console output with ANSI support
- Multiple output formats: ASCII, Hex, C-Escape, Raw
- Millisecond-precision timestamps
- Text or CSV log files
- Optional raw binary dumps
- Configurable baud rate (default: 115200)

## Technical Highlights

| Area | Details |
|------|---------|
| **Modern C++20** | Concepts with `requires` expressions, `std::optional`, `std::filesystem` |
| **Multithreading** | Thread-safe queue with `std::mutex`, `std::condition_variable`, `std::atomic` |
| **Windows System Programming** | Overlapped I/O, COM port API, Virtual Terminal Processing |
| **Architecture** | Modular design with clean separation of concerns |
| **Documentation** | Doxygen-style comments, comprehensive CLI reference |

## Quick Start

```bash
# Build with MSVC (Visual Studio Developer Command Prompt)
cl /EHsc /std:c++20 *.cpp /Fe:uart_listener.exe

# Run (dual mode)
uart_listener --rx-port 5 --tx-port 6

# Run with colors
uart_listener --rx-port COM5 --tx-port COM6 --rx-color green --tx-color red

# Run (single port mode)
uart_listener --rx-port 5 --dual-off
```

## CLI Options

| Option | Description |
|--------|-------------|
| `--rx-port N\|COMx` | RX COM port |
| `--tx-port N\|COMx` | TX COM port |
| `--dual-off` | Single port mode (only RX or TX required) |
| `--baud RATE` | Baud rate (default: 115200) |
| `--format FMT` | ascii \| hex \| c-escape \| raw |
| `--log-format FMT` | text \| csv |
| `--rx-color COLOR` | Color for [RX] tag |
| `--tx-color COLOR` | Color for [TX] tag |
| `--no-ts` | Disable timestamps |
| `--help` | Show help |

## Output Formats

| Format | Example Output |
|--------|----------------|
| `ascii` | `Hello\x0D\x0A` |
| `hex` | `48 65 6C 6C 6F 0D 0A` |
| `c-escape` | `Hello\r\n` |
| `raw` | `<raw 7 bytes>` |

## Colors

Available colors: `red`, `green`, `yellow`, `blue`, `magenta`, `cyan`, `white`, `gray`,
`brightred`, `brightgreen`, `brightyellow`, `brightblue`, `brightmagenta`, `brightcyan`, `brightwhite`

Or use ANSI codes directly: `"\033[32m"`

## Example Output

```
[INFO] ANSI color support enabled.
[INFO] Opening COM5 for RX... OK
[INFO] Opening COM6 for TX... OK
Log file: uart_COM5_COM6_20260113_143022.log

========================================
Mode: Dual (RX + TX)
RX Port: COM5
TX Port: COM6
Baud: 115200
Format: ascii
Log format: text
========================================

[RX] Port COM5 ready (color test)
[TX] Port COM6 ready (color test)

Press ESC or Q to quit.
----------------------------------------
12:34:56.123 [RX] Hello World
12:34:56.145 [TX] ACK
```

## Documentation

| Language | Documents |
|----------|-----------|
| English | [User Guide](docs/en/guide/UART_Listener_UserGuide.md) · [CLI Reference](docs/en/reference/UART_Listener_Reference.md) · [Color Reference](docs/en/reference/Color_Reference.md) |
| Deutsch | [Benutzerhandbuch](docs/de/guide/UART_Listener_UserGuide.md) · [CLI-Referenz](docs/de/reference/UART_Listener_Reference.md) · [Farbreferenz](docs/de/reference/Color_Reference.md) |

## Requirements

- Windows 10/11
- Two USB-to-UART adapters (directly connect to bus, RX only)
- C++20 compiler (MSVC 2019 v16.10+)

## Project Structure

```
UART_Listener/
├── Main.cpp              # Application entry point
├── Config.hpp            # Configuration structure
├── Cli.hpp/.cpp          # Command line parsing
├── UART.hpp/.cpp         # Serial port handling, PacketQueue
├── Worker.hpp/.cpp       # Reader and keyboard threads
├── DataFormat.hpp/.cpp   # Output formatting (ASCII, Hex, etc.)
├── Time.hpp/.cpp         # Timestamp utilities
├── Color.hpp/.cpp        # ANSI color handling with EnumTraits
├── Format.hpp            # Output/Log format enums with EnumTraits
├── Globals.hpp/.cpp      # Shared state (thread-safe)
├── ANSI_support.hpp/.cpp # Windows Virtual Terminal setup
└── docs/
    ├── de/
    │   ├── guide/       # German user guide
    │   └── reference/    # German reference docs
    └── en/
        ├── guide/       # English user guide
        └── reference/    # English reference docs
```

## License

MIT License
