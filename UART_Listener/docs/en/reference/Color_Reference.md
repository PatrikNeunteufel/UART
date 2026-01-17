# Color — Reference

> **Version:** 1.1.0  
> **Date:** 2026-01-13  
> **Type:** Reference  
> **Status:** Stable  
> **Audience:** All Developers  
> **Language:** English  
> **Deutsch:** [Color_Reference.md](../../de/reference/Color_Reference.md)

---

## Table of Contents

1. [Overview](#1-overview)
2. [Conventions](#2-conventions)
3. [Colors by Category](#3-colors-by-category)
4. [Quick Reference](#4-quick-reference)
5. [Usage](#5-usage)
6. [See Also](#6-see-also)
7. [Changelog](#changelog)

---

## 1. Overview

This reference documents all available colors for the console output of the UART Listener.

### Color Input

Colors can be specified in two ways:

| Method | Example | Description |
|--------|---------|-------------|
| Name | `green` | Case-insensitive color name |
| ANSI code | `"\033[32m"` | Direct escape code |

---

## 2. Conventions

### Notation

| Symbol | Meaning |
|--------|---------|
| `\033[` | ANSI escape sequence start (ESC + `[`) |
| `m` | Sequence end |
| `3x` | Foreground color (standard) |
| `9x` | Foreground color (bright) |

### Color Codes

| Code | Color |
|------|-------|
| `0` | Black |
| `1` | Red |
| `2` | Green |
| `3` | Yellow |
| `4` | Blue |
| `5` | Magenta |
| `6` | Cyan |
| `7` | White |

---

## 3. Colors by Category

### 3.1 Standard Colors

#### `default`

| Aspect | Value |
|--------|-------|
| **ANSI Code** | `\033[0m` |
| **Effect** | Reset to default color |
| **Since** | v1.0.0 |

---

#### `black`

| Aspect | Value |
|--------|-------|
| **ANSI Code** | `\033[30m` |
| **Since** | v1.0.0 |

---

#### `red`

| Aspect | Value |
|--------|-------|
| **ANSI Code** | `\033[31m` |
| **Since** | v1.0.0 |

**Typical Usage:** TX channel, errors

---

#### `green`

| Aspect | Value |
|--------|-------|
| **ANSI Code** | `\033[32m` |
| **Since** | v1.0.0 |

**Typical Usage:** RX channel, success

---

#### `yellow`

| Aspect | Value |
|--------|-------|
| **ANSI Code** | `\033[33m` |
| **Since** | v1.0.0 |

**Typical Usage:** Warnings

---

#### `blue`

| Aspect | Value |
|--------|-------|
| **ANSI Code** | `\033[34m` |
| **Since** | v1.0.0 |

---

#### `magenta`

| Aspect | Value |
|--------|-------|
| **ANSI Code** | `\033[35m` |
| **Since** | v1.0.0 |

---

#### `cyan`

| Aspect | Value |
|--------|-------|
| **ANSI Code** | `\033[36m` |
| **Since** | v1.0.0 |

---

#### `white`

| Aspect | Value |
|--------|-------|
| **ANSI Code** | `\033[37m` |
| **Since** | v1.0.0 |

---

#### `gray`

| Aspect | Value |
|--------|-------|
| **ANSI Code** | `\033[90m` |
| **Aliases** | `grey` (not supported) |
| **Since** | v1.0.0 |

**Note:** Technically a "bright black" color.

---

### 3.2 Bright Colors

#### `brightred`

| Aspect | Value |
|--------|-------|
| **ANSI Code** | `\033[91m` |
| **Since** | v1.0.0 |

---

#### `brightgreen`

| Aspect | Value |
|--------|-------|
| **ANSI Code** | `\033[92m` |
| **Since** | v1.0.0 |

---

#### `brightyellow`

| Aspect | Value |
|--------|-------|
| **ANSI Code** | `\033[93m` |
| **Since** | v1.0.0 |

---

#### `brightblue`

| Aspect | Value |
|--------|-------|
| **ANSI Code** | `\033[94m` |
| **Since** | v1.0.0 |

---

#### `brightmagenta`

| Aspect | Value |
|--------|-------|
| **ANSI Code** | `\033[95m` |
| **Since** | v1.0.0 |

---

#### `brightcyan`

| Aspect | Value |
|--------|-------|
| **ANSI Code** | `\033[96m` |
| **Since** | v1.0.0 |

---

#### `brightwhite`

| Aspect | Value |
|--------|-------|
| **ANSI Code** | `\033[97m` |
| **Since** | v1.0.0 |

---

## 4. Quick Reference

### Standard Colors

| Name | ANSI Code |
|------|-----------|
| `default` | `\033[0m` |
| `black` | `\033[30m` |
| `red` | `\033[31m` |
| `green` | `\033[32m` |
| `yellow` | `\033[33m` |
| `blue` | `\033[34m` |
| `magenta` | `\033[35m` |
| `cyan` | `\033[36m` |
| `white` | `\033[37m` |
| `gray` | `\033[90m` |

### Bright Colors

| Name | ANSI Code |
|------|-----------|
| `brightred` | `\033[91m` |
| `brightgreen` | `\033[92m` |
| `brightyellow` | `\033[93m` |
| `brightblue` | `\033[94m` |
| `brightmagenta` | `\033[95m` |
| `brightcyan` | `\033[96m` |
| `brightwhite` | `\033[97m` |

---

## 5. Usage

### 5.1 CLI Examples

```bash
# By name (case-insensitive)
uart_listener --rx-port 5 --tx-port 6 --rx-color green --tx-color red
uart_listener --rx-port 5 --tx-port 6 --rx-color GREEN --tx-color RED
uart_listener --rx-port 5 --tx-port 6 --rx-color Green --tx-color Red

# By ANSI code
uart_listener --rx-port 5 --tx-port 6 --rx-color "\033[32m" --tx-color "\033[31m"
uart_listener --rx-port 5 --tx-port 6 --rx-color "\x1b[92m" --tx-color "\x1b[91m"

# Bright colors for better visibility
uart_listener --rx-port 5 --tx-port 6 --rx-color brightgreen --tx-color brightred
```

### 5.2 Recommended Combinations

| Scenario | RX Color | TX Color |
|----------|----------|----------|
| Standard | `green` | `red` |
| High contrast | `brightgreen` | `brightred` |
| Dark background | `cyan` | `yellow` |
| Light background | `blue` | `magenta` |

### 5.3 Code Usage (C++)

```cpp
#include "Color.hpp"

using namespace uart_listener;

// Parse color by name
auto color = ColorTraits::fromString("green");
if (color.has_value())
{
    const char* ansi = ColorTraits::toAnsi(*color);
    std::cout << ansi << "Green text" << "\033[0m" << std::endl;
}

// Accept ANSI code or name directly
auto ansiCode = parseColorToAnsi("brightcyan");  // → "\033[96m"
auto ansiCode2 = parseColorToAnsi("\\033[96m");  // → "\033[96m"
```

---

## 6. See Also

- [UART_Listener_Reference](UART_Listener_Reference.md) — CLI Options

---

## Changelog

| Version | Date | Changes |
|---------|------|---------|
| **1.1.0** | **2026-01-13** | **Initial Release** |
