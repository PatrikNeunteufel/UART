# UART Listener — User Guide

> **Version:** 1.1.0  
> **Date:** 2026-01-13  
> **Type:** Guide  
> **Status:** Stable  
> **Audience:** Embedded Developers  
> **Language:** English  
> **Deutsch:** [UART_Listener_UserGuide.md](../../de/guide/UART_Listener_UserGuide.md)

---

## Table of Contents

1. [Overview](#1-overview)
2. [Prerequisites](#2-prerequisites)
3. [Quick Start](#3-quick-start)
4. [Configuration](#4-configuration)
5. [Output Formats](#5-output-formats)
6. [Common Pitfalls](#6-common-pitfalls)
7. [Troubleshooting](#7-troubleshooting)
8. [See Also](#8-see-also)
9. [Changelog](#changelog)

---

## 1. Overview

**UART Listener** is a Windows command-line tool for capturing UART communication on two separate COM ports. It enables simultaneous monitoring of RX and TX lines of a UART bus.

### Features

- Parallel monitoring of two COM ports (RX/TX)
- Single-port mode with `--dual-off` (RX or TX only)
- Color-coded console output via ANSI codes
- Multiple output formats (ASCII, Hex, C-Escape, Raw)
- Timestamps with millisecond precision
- Text or CSV log files
- Optional raw binary dumps
- Configurable baud rate (default: 115200)
- ESC or Q to exit

### Typical Use Cases

| Scenario | Description |
|----------|-------------|
| Bus Analysis | Eavesdropping on communication between two devices |
| Protocol Debugging | Tracing request/response sequences |
| Data Acquisition | Recording telemetry or sensor data |

---

## 2. Prerequisites

### Hardware

- [ ] Two USB-to-UART adapters (e.g., FTDI, CH340)
- [ ] Windows 10/11 with Virtual Terminal Processing support

### Software

- [ ] Visual Studio 2019/2022 with C++20 support
- [ ] Or: MinGW-w64 with C++20

### Wiring

```
┌─────────────┐                    ┌─────────────┐
│   Device A  │       TX ────────► │  Adapter 1  │ → COM5 (as RX)
│             │       RX ◄──────── │             │
└─────────────┘                    └─────────────┘
       │
       │ UART Bus
       │
┌─────────────┐                    ┌─────────────┐
│   Device B  │       TX ────────► │  Adapter 2  │ → COM6 (as TX)
│             │       RX ◄──────── │             │
└─────────────┘                    └─────────────┘
```

> **Note:** The adapters only need to connect RX (read-only mode).

### Detailed Wiring View

```
Device A                         Device B
   │                                │
   │ TX ────────────────────────┬─► │ RX
   │ RX ◄─┬─────────────────────│── │ TX
   │ GND ─│───────────┬─────────│── │ GND
   │      │           │         │   │
          │       ┌───┴───┐     │
          │       │  GND  │     │
          │       └───┬───┘     │
          │           │         │
          │  ┌────────┴──────┐  │
          │  │               │  │ 
      ┌───┴──┴───┐       ┌───┴──┴───┐
      │Adapter 1 │       │Adapter 2 │
      │  (RX)    │       │  (RX)    │
      │  COM5    │       │  COM6    │
      └──────────┘       └──────────┘
            │                │
            └────────┬───────┘
                     │
               ┌─────┴─────┐
               │    PC     │
               │UART Listen│
               └───────────┘
```

**Important:**
- Adapter 1 RX → Device A TX (to see what Device A sends)
- Adapter 2 RX → Device B TX (to see what Device B sends)
- All GND must be connected!

---

## 3. Quick Start

### 3.1 Compiling

```bash
# With Visual Studio Developer Command Prompt
cl /EHsc /std:c++20 *.cpp /Fe:uart_listener.exe

# Or with g++ (MinGW)
g++ -std=c++20 *.cpp -o uart_listener.exe
```

### 3.2 Running

**With CLI arguments:**
```bash
uart_listener --rx-port 5 --tx-port 6
```

**Interactive:**
```bash
uart_listener
# Program prompts for COM ports
```

**Expected output:**
```
[INFO] ANSI color support enabled.
[INFO] Opening COM5 for RX... OK
[INFO] Opening COM6 for TX... OK
Log file: C:\...\uart_COM5_COM6_20260113_143022.log

========================================
Mode: Dual (RX + TX)
RX Port: COM5
TX Port: COM6
Baud: 115200
Format: ascii
Log format: text
========================================

[RX] Port COM5 ready (no color)
[TX] Port COM6 ready (no color)

Press ESC or Q to quit.
----------------------------------------
```

### 3.3 Exiting

Press **ESC** or **Q** for clean shutdown.

---

## 4. Configuration

### 4.1 COM Port Specification

Ports can be specified in various formats:

| Input | Result |
|-------|--------|
| `5` | `COM5` |
| `COM5` | `COM5` |
| `com5` | `COM5` |

```bash
uart_listener --rx-port COM5 --tx-port 6
```

### 4.2 Baud Rate

Default is 115200. Change with `--baud`:

```bash
uart_listener --rx-port 5 --tx-port 6 --baud 9600
```

### 4.3 Color Coding

Colors can be specified by name or ANSI code:

**By name (case-insensitive):**
```bash
uart_listener --rx-port 5 --tx-port 6 --rx-color green --tx-color red
```

**By ANSI code:**
```bash
uart_listener --rx-port 5 --tx-port 6 --rx-color "\033[92m" --tx-color "\033[91m"
```

**Color test at startup:**

The program shows a test output with configured colors at startup:
```
[RX] Port COM5 ready (color test)      <- green
[TX] Port COM6 ready (color test)      <- red
```

If ANSI colors are not supported, a warning appears:
```
Warning: Could not enable ANSI colors (Virtual Terminal Processing).
         Color output may not work correctly.
```

**Available colors:**

| Name | ANSI Code | Description |
|------|-----------|-------------|
| `default` | `\033[0m` | Reset |
| `red` | `\033[31m` | Standard Red |
| `green` | `\033[32m` | Standard Green |
| `yellow` | `\033[33m` | Standard Yellow |
| `blue` | `\033[34m` | Standard Blue |
| `cyan` | `\033[36m` | Standard Cyan |
| `brightred` | `\033[91m` | Bright Red |
| `brightgreen` | `\033[92m` | Bright Green |
| `brightcyan` | `\033[96m` | Bright Cyan |

### 4.4 Disable Timestamps

```bash
uart_listener --rx-port 5 --tx-port 6 --no-ts
```

### 4.5 Single-Port Mode

With `--dual-off`, only one port can be monitored:

```bash
# Listen to RX only
uart_listener --rx-port 5 --dual-off

# Listen to TX only  
uart_listener --tx-port 6 --dual-off
```

**Output in single-port mode:**
```
========================================
Mode: Single (RX only)
RX Port: COM5
Baud: 115200
...
========================================

[RX] Port COM5 ready (no color)
```

---

## 5. Output Formats

### 5.1 Display Format (`--format`)

| Format | Description | Example |
|--------|-------------|---------|
| `ascii` | Printable characters, rest as `\xNN` | `Hello\x0D\x0A` |
| `hex` | Hexadecimal bytes | `48 65 6C 6C 6F 0D 0A` |
| `c-escape` | C-Style escapes | `Hello\r\n` |
| `raw` | Byte count only | `<raw 7 bytes>` |

```bash
uart_listener --rx-port 5 --tx-port 6 --format hex
```

### 5.2 Log Format (`--log-format`)

**Text (default):**
```
12:34:56.789 [RX] Hello World
12:34:56.801 [TX] ACK
```

**CSV:**
```csv
Timestamp;Channel;Data
12:34:56.789;RX;Hello World
12:34:56.801;TX;ACK
```

```bash
uart_listener --rx-port 5 --tx-port 6 --log-format csv
```

### 5.3 Raw Binary Output

For post-capture analysis, raw data can be saved separately:

```bash
uart_listener --rx-port 5 --tx-port 6 --rx-raw-out rx_dump.bin --tx-raw-out tx_dump.bin
```

---

## 6. Common Pitfalls

### 6.1 ANSI Colors Not Displayed

**Problem:** Instead of colors, escape sequences like `[32m` appear.

**Cause:** Windows Terminal Processing is not enabled.

**Diagnosis:** At startup, this appears:
```
Warning: Could not enable ANSI colors (Virtual Terminal Processing).
```

**Solution:** 
- Use Windows Terminal or newer ConEmu
- Windows 10 1809+ is required for automatic ANSI support

### 6.2 COM Port Already in Use

**Problem:** `[INFO] Opening COM5 for RX... FAILED`

Followed by:
```
Error opening port \\.\COM5 (Error code: 5)
```

**Cause:** Port is occupied by another program (Error 5 = Access Denied).

**Solution:**
1. Check Device Manager
2. Close other serial terminals (HTerm, PuTTY, etc.)
3. Use a different port

### 6.3 No Data Received

**Problem:** Program runs but no output.

**Possible causes:**

| Cause | Check |
|-------|-------|
| Wrong baud rate | Verify with oscilloscope or logic analyzer |
| Wiring | RX/TX swapped? GND connected? |
| No traffic | Other side is not sending |

---

## 7. Troubleshooting

### Checklist

- [ ] USB adapter recognized? (Device Manager)
- [ ] Correct COM port number?
- [ ] Baud rate correct?
- [ ] RX line connected (not TX)?
- [ ] GND connected between adapter and target device?

### Common Errors

| Error | Cause | Solution |
|-------|-------|----------|
| `Opening COMx... FAILED` | Port occupied or doesn't exist | Check port number, close other programs |
| `Error code: 5` | Access Denied | Port is already in use |
| `Error code: 2` | Port doesn't exist | Check COM number in Device Manager |
| No output | No traffic or wrong baud rate | Check wiring/baud rate |
| Unreadable characters | Wrong format | Use `--format hex` |
| `[ESC pressed]` doesn't appear | Console blocked | Use CLI arguments instead of interactive input |

---

## 8. See Also

- [UART_Listener_Reference](../reference/UART_Listener_Reference.md) — CLI Options Reference
- [Color_Reference](../reference/Color_Reference.md) — Available Colors

---

## Changelog

| Version | Date | Changes |
|---------|------|---------|
| **1.1.0** | **2026-01-13** | **New: Single-port mode (`--dual-off`)** |
| 1.0.0 | 2026-01-13 | Initial Release |
