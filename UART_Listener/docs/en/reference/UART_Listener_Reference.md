# UART Listener CLI — Reference

> **Version:** 1.1.0  
> **Date:** 2026-01-13  
> **Type:** Reference  
> **Status:** Stable  
> **Audience:** All Developers  
> **Language:** English  
> **Deutsch:** [UART_Listener_Reference.md](../../de/reference/UART_Listener_Reference.md)

---

## Table of Contents

1. [Overview](#1-overview)
2. [Conventions](#2-conventions)
3. [CLI Options by Category](#3-cli-options-by-category)
4. [Quick Reference](#4-quick-reference)
5. [Code Usage](#5-code-usage)
6. [See Also](#6-see-also)
7. [Changelog](#changelog)

---

## 1. Overview

This reference documents all CLI options of the UART Listener tool.

### Syntax

```
uart_listener [--rx-port PORT] [--tx-port PORT] [OPTIONS]
```

---

## 2. Conventions

### Notation

| Symbol | Meaning |
|--------|---------|
| `N\|COMx` | Either number or COM name |
| `<path>` | File path (relative or absolute) |
| `<ms>` | Milliseconds as integer |
| `✓` | Option is required |
| `—` | No default value |

### Value Types

| Type | Examples |
|------|----------|
| `PORT` | `5`, `COM5`, `com5` |
| `RATE` | `9600`, `115200`, `921600` |
| `FMT` | `ascii`, `hex`, `c-escape`, `raw` |
| `COLOR` | `green`, `red`, `"\033[32m"` |

---

## 3. CLI Options by Category

### 3.1 Port Configuration

#### `--rx-port`

| Aspect | Value |
|--------|-------|
| **Type** | `PORT` |
| **Required** | ✓ (in dual mode) |
| **Default** | — |
| **Since** | v1.0.0 |

**Description:**  
COM port for the RX channel (receive line).

**Valid Values:**
- `N` — Port number (becomes `COMN`)
- `COMx` — Full port name

**Example:**
```bash
--rx-port 5
--rx-port COM5
```

---

#### `--tx-port`

| Aspect | Value |
|--------|-------|
| **Type** | `PORT` |
| **Required** | ✓ (in dual mode) |
| **Default** | — |
| **Since** | v1.0.0 |

**Description:**  
COM port for the TX channel (transmit line).

**Example:**
```bash
--tx-port 6
--tx-port COM6
```

---

#### `--dual-off`

| Aspect | Value |
|--------|-------|
| **Type** | Flag |
| **Required** | — |
| **Default** | Dual mode active |
| **Since** | v1.1.0 |

**Description:**  
Enables single-port mode. Only one port (RX or TX) is then required.

**Example:**
```bash
# Listen to RX only
uart_listener --rx-port 5 --dual-off

# Listen to TX only
uart_listener --tx-port 6 --dual-off
```

**Notes:**
- Log filename contains `_RX` or `_TX` suffix instead of second port
- Only the configured channel is monitored

---

#### `--baud`

| Aspect | Value |
|--------|-------|
| **Type** | `RATE` |
| **Required** | — |
| **Default** | `115200` |
| **Since** | v1.0.0 |

**Description:**  
Baud rate for both COM ports.

**Valid Values:**
- All common baud rates: `300`, `1200`, `2400`, `4800`, `9600`, `19200`, `38400`, `57600`, `115200`, `230400`, `460800`, `921600`

**Example:**
```bash
--baud 9600
```

---

### 3.2 Output Format

#### `--format`

| Aspect | Value |
|--------|-------|
| **Type** | `FMT` |
| **Required** | — |
| **Default** | `ascii` |
| **Since** | v1.0.0 |

**Description:**  
Display format for console and text log.

**Valid Values:**

| Value | Description | Output for `Hello\r\n` |
|-------|-------------|------------------------|
| `ascii` | Printable + `\xNN` | `Hello\x0D\x0A` |
| `hex` | Hexadecimal | `48 65 6C 6C 6F 0D 0A` |
| `c-escape` | C-Style | `Hello\r\n` |
| `raw` | Byte count | `<raw 7 bytes>` |

**Example:**
```bash
--format hex
```

**Notes:**
- Aliases `cescape` and `c_escape` are accepted

---

#### `--log-format`

| Aspect | Value |
|--------|-------|
| **Type** | `FMT` |
| **Required** | — |
| **Default** | `text` |
| **Since** | v1.0.0 |

**Description:**  
Container format of the log file.

**Valid Values:**

| Value | Description |
|-------|-------------|
| `text` | Plain text with tags |
| `csv` | Semicolon-separated |

**Example:**
```bash
--log-format csv
```

---

### 3.3 Logging

#### `--log-file`

| Aspect | Value |
|--------|-------|
| **Type** | `<path>` |
| **Required** | — |
| **Default** | Auto: `uart_COMx_COMy_YYYYMMDD_HHMMSS.log` |
| **Since** | v1.0.0 |

**Description:**  
Path to the log file.

**Example:**
```bash
--log-file C:\logs\capture.log
```

---

#### `--rx-raw-out`

| Aspect | Value |
|--------|-------|
| **Type** | `<path>` |
| **Required** | — |
| **Default** | — (disabled) |
| **Since** | v1.0.0 |

**Description:**  
Path for raw binary dump of the RX channel.

**Example:**
```bash
--rx-raw-out rx_data.bin
```

---

#### `--tx-raw-out`

| Aspect | Value |
|--------|-------|
| **Type** | `<path>` |
| **Required** | — |
| **Default** | — (disabled) |
| **Since** | v1.0.0 |

**Description:**  
Path for raw binary dump of the TX channel.

**Example:**
```bash
--tx-raw-out tx_data.bin
```

---

### 3.4 Display

#### `--rx-color`

| Aspect | Value |
|--------|-------|
| **Type** | `COLOR` |
| **Required** | — |
| **Default** | — (no color) |
| **Since** | v1.0.0 |

**Description:**  
ANSI color for the `[RX]` tag in the console.

**Valid Values:**
- Color name: `red`, `green`, `yellow`, `blue`, `magenta`, `cyan`, `white`, `gray`, `brightred`, `brightgreen`, `brightyellow`, `brightblue`, `brightmagenta`, `brightcyan`, `brightwhite`
- ANSI escape: `"\033[32m"`, `"\x1b[92m"`

**Example:**
```bash
--rx-color green
--rx-color "\033[92m"
```

---

#### `--tx-color`

| Aspect | Value |
|--------|-------|
| **Type** | `COLOR` |
| **Required** | — |
| **Default** | — (no color) |
| **Since** | v1.0.0 |

**Description:**  
ANSI color for the `[TX]` tag in the console.

**Example:**
```bash
--tx-color red
```

---

#### `--no-ts`

| Aspect | Value |
|--------|-------|
| **Type** | Flag |
| **Required** | — |
| **Default** | Timestamps active |
| **Since** | v1.0.0 |

**Description:**  
Disables timestamps in console and log.

**Example:**
```bash
--no-ts
```

---

### 3.5 Timing

#### `--flush-timeout`

| Aspect | Value |
|--------|-------|
| **Type** | `<ms>` |
| **Required** | — |
| **Default** | `250` |
| **Since** | v1.0.0 |

**Description:**  
Interval in milliseconds for log file flush.

**Example:**
```bash
--flush-timeout 1000
```

---

### 3.6 Help

#### `--help`, `-h`

| Aspect | Value |
|--------|-------|
| **Type** | Flag |
| **Required** | — |
| **Since** | v1.0.0 |

**Description:**  
Shows help text and exits.

---

### 3.7 Exiting the Program

The program can be exited at any time with **ESC** or **Q**.

On exit, the following appears:
```
[ESC pressed]

[INFO] Shutting down...
[INFO] Waiting for RX thread... done
[INFO] Waiting for TX thread... done
[INFO] Waiting for keyboard thread... done
[INFO] Program terminated successfully.
```

---

## 4. Quick Reference

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `--rx-port` | PORT | — | RX COM port |
| `--tx-port` | PORT | — | TX COM port |
| `--dual-off` | flag | — | Single-port mode |
| `--baud` | RATE | `115200` | Baud rate |
| `--format` | FMT | `ascii` | Display format |
| `--log-format` | FMT | `text` | Log container |
| `--log-file` | path | auto | Log file path |
| `--rx-raw-out` | path | — | RX raw dump |
| `--tx-raw-out` | path | — | TX raw dump |
| `--rx-color` | COLOR | — | RX tag color |
| `--tx-color` | COLOR | — | TX tag color |
| `--no-ts` | flag | — | Timestamps off |
| `--flush-timeout` | ms | `250` | Flush interval |
| `--help` | flag | — | Show help |

---

## 5. Code Usage

### 5.1 EnumTraits for Colors

```cpp
#include "Color.hpp"

using namespace uart_listener;

// Case-insensitive lookup
auto color = ColorTraits::fromString("Green");
if (color.has_value())
{
    const char* ansi = ColorTraits::toAnsi(*color);
    std::cout << ansi << "Colored text" << "\033[0m" << std::endl;
}
```

### 5.2 EnumTraits for Formats

```cpp
#include "Format.hpp"

using namespace uart_listener;

auto fmt = OutputFormatTraits::fromString("c-escape");
if (fmt.has_value())
{
    std::cout << OutputFormatTraits::toString(*fmt); // "c-escape"
}
```

### 5.3 Parse Color (Name or ANSI)

```cpp
#include "Color.hpp"

auto ansi = parseColorToAnsi("green");      // → "\033[32m"
auto ansi2 = parseColorToAnsi("\\033[91m"); // → "\x1b[91m"
```

---

## 6. See Also

- [Color_Reference](Color_Reference.md) — Available colors

---

## Changelog

| Version | Date | Changes |
|---------|------|---------|
| **1.1.0** | **2026-01-13** | **New: `--dual-off` option for single-port mode** |
| 1.0.0 | 2026-01-13 | Initial Release |
