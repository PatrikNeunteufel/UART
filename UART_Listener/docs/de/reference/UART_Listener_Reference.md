# UART Listener CLI — Referenz

> **Version:** 1.1.0  
> **Datum:** 2026-01-13  
> **Typ:** Reference  
> **Status:** Stabil  
> **Zielgruppe:** Alle Entwickler  
> **Sprache:** Deutsch  
> **English:** [UART_Listener_Reference.md](../../en/reference/UART_Listener_Reference.md)

---

## Inhaltsverzeichnis

1. [Übersicht](#1-übersicht)
2. [Konventionen](#2-konventionen)
3. [CLI-Optionen nach Kategorie](#3-cli-optionen-nach-kategorie)
4. [Schnellreferenz](#4-schnellreferenz)
5. [Verwendung in Code](#5-verwendung-in-code)
6. [Siehe auch](#6-siehe-auch)
7. [Changelog](#changelog)

---

## 1. Übersicht

Diese Referenz dokumentiert alle CLI-Optionen des UART Listener Tools.

### Syntax

```
uart_listener [--rx-port PORT] [--tx-port PORT] [OPTIONS]
```

---

## 2. Konventionen

### Notation

| Symbol | Bedeutung |
|--------|-----------|
| `N\|COMx` | Entweder Zahl oder COM-Name |
| `<path>` | Dateipfad (relativ oder absolut) |
| `<ms>` | Millisekunden als Ganzzahl |
| `✓` | Option ist Pflicht |
| `—` | Kein Standardwert |

### Werte-Typen

| Typ | Beispiele |
|-----|-----------|
| `PORT` | `5`, `COM5`, `com5` |
| `RATE` | `9600`, `115200`, `921600` |
| `FMT` | `ascii`, `hex`, `c-escape`, `raw` |
| `COLOR` | `green`, `red`, `"\033[32m"` |

---

## 3. CLI-Optionen nach Kategorie

### 3.1 Port-Konfiguration

#### `--rx-port`

| Aspekt | Wert |
|--------|------|
| **Typ** | `PORT` |
| **Pflicht** | ✓ (im Dual-Mode) |
| **Default** | — |
| **Seit** | v1.0.0 |

**Beschreibung:**  
COM-Port für den RX-Kanal (Empfangsleitung).

**Gültige Werte:**
- `N` — Port-Nummer (wird zu `COMN`)
- `COMx` — Vollständiger Port-Name

**Beispiel:**
```bash
--rx-port 5
--rx-port COM5
```

---

#### `--tx-port`

| Aspekt | Wert |
|--------|------|
| **Typ** | `PORT` |
| **Pflicht** | ✓ (im Dual-Mode) |
| **Default** | — |
| **Seit** | v1.0.0 |

**Beschreibung:**  
COM-Port für den TX-Kanal (Sendeleitung).

**Beispiel:**
```bash
--tx-port 6
--tx-port COM6
```

---

#### `--dual-off`

| Aspekt | Wert |
|--------|------|
| **Typ** | Flag |
| **Pflicht** | — |
| **Default** | Dual-Mode aktiv |
| **Seit** | v1.1.0 |

**Beschreibung:**  
Aktiviert den Single-Port-Modus. Es ist dann nur ein Port (RX oder TX) erforderlich.

**Beispiel:**
```bash
# Nur RX abhören
uart_listener --rx-port 5 --dual-off

# Nur TX abhören
uart_listener --tx-port 6 --dual-off
```

**Hinweise:**
- Log-Dateiname enthält `_RX` oder `_TX` Suffix statt zweitem Port
- Nur der konfigurierte Kanal wird abgehört

---

#### `--baud`

| Aspekt | Wert |
|--------|------|
| **Typ** | `RATE` |
| **Pflicht** | — |
| **Default** | `115200` |
| **Seit** | v1.0.0 |

**Beschreibung:**  
Baudrate für beide COM-Ports.

**Gültige Werte:**
- Alle gängigen Baudraten: `300`, `1200`, `2400`, `4800`, `9600`, `19200`, `38400`, `57600`, `115200`, `230400`, `460800`, `921600`

**Beispiel:**
```bash
--baud 9600
```

---

### 3.2 Ausgabeformat

#### `--format`

| Aspekt | Wert |
|--------|------|
| **Typ** | `FMT` |
| **Pflicht** | — |
| **Default** | `ascii` |
| **Seit** | v1.0.0 |

**Beschreibung:**  
Anzeigeformat für Konsole und Text-Log.

**Gültige Werte:**

| Wert | Beschreibung | Ausgabe für `Hello\r\n` |
|------|--------------|-------------------------|
| `ascii` | Druckbar + `\xNN` | `Hello\x0D\x0A` |
| `hex` | Hexadezimal | `48 65 6C 6C 6F 0D 0A` |
| `c-escape` | C-Style | `Hello\r\n` |
| `raw` | Byte-Count | `<raw 7 bytes>` |

**Beispiel:**
```bash
--format hex
```

**Hinweise:**
- Aliase `cescape` und `c_escape` werden akzeptiert

---

#### `--log-format`

| Aspekt | Wert |
|--------|------|
| **Typ** | `FMT` |
| **Pflicht** | — |
| **Default** | `text` |
| **Seit** | v1.0.0 |

**Beschreibung:**  
Container-Format der Log-Datei.

**Gültige Werte:**

| Wert | Beschreibung |
|------|--------------|
| `text` | Plain-Text mit Tags |
| `csv` | Semikolon-separiert |

**Beispiel:**
```bash
--log-format csv
```

---

### 3.3 Logging

#### `--log-file`

| Aspekt | Wert |
|--------|------|
| **Typ** | `<path>` |
| **Pflicht** | — |
| **Default** | Auto: `uart_COMx_COMy_YYYYMMDD_HHMMSS.log` |
| **Seit** | v1.0.0 |

**Beschreibung:**  
Pfad zur Log-Datei.

**Beispiel:**
```bash
--log-file C:\logs\capture.log
```

---

#### `--rx-raw-out`

| Aspekt | Wert |
|--------|------|
| **Typ** | `<path>` |
| **Pflicht** | — |
| **Default** | — (deaktiviert) |
| **Seit** | v1.0.0 |

**Beschreibung:**  
Pfad für Raw-Binary-Dump des RX-Kanals.

**Beispiel:**
```bash
--rx-raw-out rx_data.bin
```

---

#### `--tx-raw-out`

| Aspekt | Wert |
|--------|------|
| **Typ** | `<path>` |
| **Pflicht** | — |
| **Default** | — (deaktiviert) |
| **Seit** | v1.0.0 |

**Beschreibung:**  
Pfad für Raw-Binary-Dump des TX-Kanals.

**Beispiel:**
```bash
--tx-raw-out tx_data.bin
```

---

### 3.4 Anzeige

#### `--rx-color`

| Aspekt | Wert |
|--------|------|
| **Typ** | `COLOR` |
| **Pflicht** | — |
| **Default** | — (keine Farbe) |
| **Seit** | v1.0.0 |

**Beschreibung:**  
ANSI-Farbe für den `[RX]`-Tag in der Konsole.

**Gültige Werte:**
- Farbname: `red`, `green`, `yellow`, `blue`, `magenta`, `cyan`, `white`, `gray`, `brightred`, `brightgreen`, `brightyellow`, `brightblue`, `brightmagenta`, `brightcyan`, `brightwhite`
- ANSI-Escape: `"\033[32m"`, `"\x1b[92m"`

**Beispiel:**
```bash
--rx-color green
--rx-color "\033[92m"
```

---

#### `--tx-color`

| Aspekt | Wert |
|--------|------|
| **Typ** | `COLOR` |
| **Pflicht** | — |
| **Default** | — (keine Farbe) |
| **Seit** | v1.0.0 |

**Beschreibung:**  
ANSI-Farbe für den `[TX]`-Tag in der Konsole.

**Beispiel:**
```bash
--tx-color red
```

---

#### `--no-ts`

| Aspekt | Wert |
|--------|------|
| **Typ** | Flag |
| **Pflicht** | — |
| **Default** | Timestamps aktiv |
| **Seit** | v1.0.0 |

**Beschreibung:**  
Deaktiviert Timestamps in Konsole und Log.

**Beispiel:**
```bash
--no-ts
```

---

### 3.5 Timing

#### `--flush-timeout`

| Aspekt | Wert |
|--------|------|
| **Typ** | `<ms>` |
| **Pflicht** | — |
| **Default** | `250` |
| **Seit** | v1.0.0 |

**Beschreibung:**  
Intervall in Millisekunden für Log-File-Flush.

**Beispiel:**
```bash
--flush-timeout 1000
```

---

### 3.6 Hilfe

#### `--help`, `-h`

| Aspekt | Wert |
|--------|------|
| **Typ** | Flag |
| **Pflicht** | — |
| **Seit** | v1.0.0 |

**Beschreibung:**  
Zeigt Hilfetext und beendet.

---

### 3.7 Programm beenden

Das Programm kann jederzeit mit **ESC** oder **Q** beendet werden. 

Beim Beenden erscheint:
```
[ESC pressed]

[INFO] Shutting down...
[INFO] Waiting for RX thread... done
[INFO] Waiting for TX thread... done
[INFO] Waiting for keyboard thread... done
[INFO] Program terminated successfully.
```

---

## 4. Schnellreferenz

| Option | Typ | Default | Beschreibung |
|--------|-----|---------|--------------|
| `--rx-port` | PORT | — | RX COM-Port |
| `--tx-port` | PORT | — | TX COM-Port |
| `--dual-off` | flag | — | Single-Port-Modus |
| `--baud` | RATE | `115200` | Baudrate |
| `--format` | FMT | `ascii` | Anzeigeformat |
| `--log-format` | FMT | `text` | Log-Container |
| `--log-file` | path | auto | Log-Dateipfad |
| `--rx-raw-out` | path | — | RX Raw-Dump |
| `--tx-raw-out` | path | — | TX Raw-Dump |
| `--rx-color` | COLOR | — | RX-Tag-Farbe |
| `--tx-color` | COLOR | — | TX-Tag-Farbe |
| `--no-ts` | flag | — | Timestamps aus |
| `--flush-timeout` | ms | `250` | Flush-Intervall |
| `--help` | flag | — | Hilfe anzeigen |

---

## 5. Verwendung in Code

### 5.1 EnumTraits für Farben

```cpp
#include "Color.hpp"

using namespace uart_listener;

// Case-insensitive Lookup
auto color = ColorTraits::fromString("Green");
if (color.has_value())
{
    const char* ansi = ColorTraits::toAnsi(*color);
    std::cout << ansi << "Colored text" << "\033[0m" << std::endl;
}
```

### 5.2 EnumTraits für Formate

```cpp
#include "Format.hpp"

using namespace uart_listener;

auto fmt = OutputFormatTraits::fromString("c-escape");
if (fmt.has_value())
{
    std::cout << OutputFormatTraits::toString(*fmt); // "c-escape"
}
```

### 5.3 Farbe parsen (Name oder ANSI)

```cpp
#include "Color.hpp"

auto ansi = parseColorToAnsi("green");      // → "\033[32m"
auto ansi2 = parseColorToAnsi("\\033[91m"); // → "\x1b[91m"
```

---

## 6. Siehe auch

- [Color_Reference](Color_Reference.md) — Verfügbare Farben

---

## Changelog

| Version | Datum | Änderungen |
|---------|-------|------------|
| **1.1.0** | **2026-01-13** | **Neu: `--dual-off` Option für Single-Port-Modus** |
| 1.0.0 | 2026-01-13 | Initial Release |
