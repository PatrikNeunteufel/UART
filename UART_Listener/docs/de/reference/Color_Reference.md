# Color — Referenz

> **Version:** 1.1.0  
> **Datum:** 2026-01-13  
> **Typ:** Reference  
> **Status:** Stabil  
> **Zielgruppe:** Alle Entwickler  
> **Sprache:** Deutsch  
> **English:** [Color_Reference.md](../../en/reference/Color_Reference.md)

---

## Inhaltsverzeichnis

1. [Übersicht](#1-übersicht)
2. [Konventionen](#2-konventionen)
3. [Farben nach Kategorie](#3-farben-nach-kategorie)
4. [Schnellreferenz](#4-schnellreferenz)
5. [Verwendung](#5-verwendung)
6. [Siehe auch](#6-siehe-auch)
7. [Changelog](#changelog)

---

## 1. Übersicht

Diese Referenz dokumentiert alle verfügbaren Farben für die Konsolenausgabe des UART Listeners.

### Farbeingabe

Farben können auf zwei Arten angegeben werden:

| Methode | Beispiel | Beschreibung |
|---------|----------|--------------|
| Name | `green` | Case-insensitive Farbname |
| ANSI-Code | `"\033[32m"` | Direkter Escape-Code |

---

## 2. Konventionen

### Notation

| Symbol | Bedeutung |
|--------|-----------|
| `\033[` | ANSI Escape-Sequenz Start (ESC + `[`) |
| `m` | Sequenz-Ende |
| `3x` | Vordergrundfarbe (Standard) |
| `9x` | Vordergrundfarbe (Hell/Bright) |

### Farbcodes

| Code | Farbe |
|------|-------|
| `0` | Schwarz |
| `1` | Rot |
| `2` | Grün |
| `3` | Gelb |
| `4` | Blau |
| `5` | Magenta |
| `6` | Cyan |
| `7` | Weiss |

---

## 3. Farben nach Kategorie

### 3.1 Standard-Farben

#### `default`

| Aspekt | Wert |
|--------|------|
| **ANSI-Code** | `\033[0m` |
| **Wirkung** | Reset auf Standardfarbe |
| **Seit** | v1.0.0 |

---

#### `black`

| Aspekt | Wert |
|--------|------|
| **ANSI-Code** | `\033[30m` |
| **Seit** | v1.0.0 |

---

#### `red`

| Aspekt | Wert |
|--------|------|
| **ANSI-Code** | `\033[31m` |
| **Seit** | v1.0.0 |

**Typische Verwendung:** TX-Kanal, Fehler

---

#### `green`

| Aspekt | Wert |
|--------|------|
| **ANSI-Code** | `\033[32m` |
| **Seit** | v1.0.0 |

**Typische Verwendung:** RX-Kanal, Erfolg

---

#### `yellow`

| Aspekt | Wert |
|--------|------|
| **ANSI-Code** | `\033[33m` |
| **Seit** | v1.0.0 |

**Typische Verwendung:** Warnungen

---

#### `blue`

| Aspekt | Wert |
|--------|------|
| **ANSI-Code** | `\033[34m` |
| **Seit** | v1.0.0 |

---

#### `magenta`

| Aspekt | Wert |
|--------|------|
| **ANSI-Code** | `\033[35m` |
| **Seit** | v1.0.0 |

---

#### `cyan`

| Aspekt | Wert |
|--------|------|
| **ANSI-Code** | `\033[36m` |
| **Seit** | v1.0.0 |

---

#### `white`

| Aspekt | Wert |
|--------|------|
| **ANSI-Code** | `\033[37m` |
| **Seit** | v1.0.0 |

---

#### `gray`

| Aspekt | Wert |
|--------|------|
| **ANSI-Code** | `\033[90m` |
| **Aliase** | `grey` (nicht unterstützt) |
| **Seit** | v1.0.0 |

**Hinweis:** Technisch eine "helle schwarze" Farbe.

---

### 3.2 Helle Farben (Bright)

#### `brightred`

| Aspekt | Wert |
|--------|------|
| **ANSI-Code** | `\033[91m` |
| **Seit** | v1.0.0 |

---

#### `brightgreen`

| Aspekt | Wert |
|--------|------|
| **ANSI-Code** | `\033[92m` |
| **Seit** | v1.0.0 |

---

#### `brightyellow`

| Aspekt | Wert |
|--------|------|
| **ANSI-Code** | `\033[93m` |
| **Seit** | v1.0.0 |

---

#### `brightblue`

| Aspekt | Wert |
|--------|------|
| **ANSI-Code** | `\033[94m` |
| **Seit** | v1.0.0 |

---

#### `brightmagenta`

| Aspekt | Wert |
|--------|------|
| **ANSI-Code** | `\033[95m` |
| **Seit** | v1.0.0 |

---

#### `brightcyan`

| Aspekt | Wert |
|--------|------|
| **ANSI-Code** | `\033[96m` |
| **Seit** | v1.0.0 |

---

#### `brightwhite`

| Aspekt | Wert |
|--------|------|
| **ANSI-Code** | `\033[97m` |
| **Seit** | v1.0.0 |

---

## 4. Schnellreferenz

### Standard-Farben

| Name | ANSI-Code |
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

### Helle Farben

| Name | ANSI-Code |
|------|-----------|
| `brightred` | `\033[91m` |
| `brightgreen` | `\033[92m` |
| `brightyellow` | `\033[93m` |
| `brightblue` | `\033[94m` |
| `brightmagenta` | `\033[95m` |
| `brightcyan` | `\033[96m` |
| `brightwhite` | `\033[97m` |

---

## 5. Verwendung

### 5.1 CLI-Beispiele

```bash
# Per Name (case-insensitive)
uart_listener --rx-port 5 --tx-port 6 --rx-color green --tx-color red
uart_listener --rx-port 5 --tx-port 6 --rx-color GREEN --tx-color RED
uart_listener --rx-port 5 --tx-port 6 --rx-color Green --tx-color Red

# Per ANSI-Code
uart_listener --rx-port 5 --tx-port 6 --rx-color "\033[32m" --tx-color "\033[31m"
uart_listener --rx-port 5 --tx-port 6 --rx-color "\x1b[92m" --tx-color "\x1b[91m"

# Helle Farben für bessere Sichtbarkeit
uart_listener --rx-port 5 --tx-port 6 --rx-color brightgreen --tx-color brightred
```

### 5.2 Empfohlene Kombinationen

| Szenario | RX-Farbe | TX-Farbe |
|----------|----------|----------|
| Standard | `green` | `red` |
| Hoher Kontrast | `brightgreen` | `brightred` |
| Dunkler Hintergrund | `cyan` | `yellow` |
| Heller Hintergrund | `blue` | `magenta` |

### 5.3 Code-Verwendung (C++)

```cpp
#include "Color.hpp"

using namespace uart_listener;

// Farbe per Name parsen
auto color = ColorTraits::fromString("green");
if (color.has_value())
{
    const char* ansi = ColorTraits::toAnsi(*color);
    std::cout << ansi << "Grüner Text" << "\033[0m" << std::endl;
}

// Direkt ANSI-Code oder Name akzeptieren
auto ansiCode = parseColorToAnsi("brightcyan");  // → "\033[96m"
auto ansiCode2 = parseColorToAnsi("\\033[96m");  // → "\033[96m"
```

---

## 6. Siehe auch

- [UART_Listener_Reference](UART_Listener_Reference.md) — CLI-Optionen

---

## Changelog

| Version | Datum | Änderungen |
|---------|-------|------------|
| **1.1.0** | **2026-01-13** | **Initial Release** |
