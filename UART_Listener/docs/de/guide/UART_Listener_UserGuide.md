# UART Listener — Benutzerhandbuch

> **Version:** 1.1.0  
> **Datum:** 2026-01-13  
> **Typ:** Guide  
> **Status:** Stabil  
> **Zielgruppe:** Embedded-Entwickler  
> **Sprache:** Deutsch  
> **English:** [UART_Listener_UserGuide.md](../../en/guide/UART_Listener_UserGuide.md)

---

## Inhaltsverzeichnis

1. [Überblick](#1-überblick)
2. [Voraussetzungen](#2-voraussetzungen)
3. [Schnellstart](#3-schnellstart)
4. [Konfiguration](#4-konfiguration)
5. [Ausgabeformate](#5-ausgabeformate)
6. [Stolpersteine und Lösungen](#6-stolpersteine-und-lösungen)
7. [Troubleshooting](#7-troubleshooting)
8. [Siehe auch](#8-siehe-auch)
9. [Changelog](#changelog)

---

## 1. Überblick

Der **UART Listener** ist ein Windows-Kommandozeilenwerkzeug zum Mitschneiden von UART-Kommunikation auf zwei separaten COM-Ports. Er ermöglicht das gleichzeitige Abhören von RX- und TX-Leitungen eines UART-Busses.

### Features

- Paralleles Abhören von zwei COM-Ports (RX/TX)
- Single-Port-Modus mit `--dual-off` (nur RX oder TX)
- Farbkodierte Konsolenausgabe per ANSI-Codes
- Mehrere Ausgabeformate (ASCII, Hex, C-Escape, Raw)
- Timestamps mit Millisekunden-Auflösung
- Text- oder CSV-Logdateien
- Optionale Raw-Binary-Dumps
- Konfigurierbare Baudrate (Standard: 115200)
- ESC oder Q zum Beenden

### Typische Anwendungsfälle

| Szenario | Beschreibung |
|----------|--------------|
| Bus-Analyse | Mithören der Kommunikation zwischen zwei Geräten |
| Protokoll-Debugging | Nachverfolgung von Request/Response-Abläufen |
| Datenerfassung | Aufzeichnung von Telemetrie oder Sensordaten |

---

## 2. Voraussetzungen

### Hardware

- [ ] Zwei USB-zu-UART-Adapter (z.B. FTDI, CH340)
- [ ] Windows 10/11 mit Virtual Terminal Processing Support

### Software

- [ ] Visual Studio 2019/2022 mit C++20 Support
- [ ] Oder: MinGW-w64 mit C++20

### Verkabelung

```
┌─────────────┐                    ┌─────────────┐
│   Device A  │       TX ────────► │  Adapter 1  │ → COM5 (als RX)
│             │       RX ◄──────── │             │
└─────────────┘                    └─────────────┘
       │
       │ UART Bus
       │
┌─────────────┐                    ┌─────────────┐
│   Device B  │       TX ────────► │  Adapter 2  │ → COM6 (als TX)
│             │       RX ◄──────── │             │
└─────────────┘                    └─────────────┘
```

> **Hinweis:** Die Adapter müssen nur RX verbinden (Read-Only-Modus).

### Detailansicht der Verkabelung

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

**Wichtig:**
- Adapter 1 RX → Device A TX (um zu sehen was Device A sendet)
- Adapter 2 RX → Device B TX (um zu sehen was Device B sendet)
- Alle GND müssen verbunden sein!

---

## 3. Schnellstart

### 3.1 Kompilieren

```bash
# Mit Visual Studio Developer Command Prompt
cl /EHsc /std:c++20 *.cpp /Fe:uart_listener.exe

# Oder mit g++ (MinGW)
g++ -std=c++20 *.cpp -o uart_listener.exe
```

### 3.2 Starten

**Mit CLI-Argumenten:**
```bash
uart_listener --rx-port 5 --tx-port 6
```

**Interaktiv:**
```bash
uart_listener
# Programm fragt nach COM-Ports
```

**Erwartete Ausgabe:**
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

### 3.3 Beenden

Drücke **ESC** oder **Q** zum sauberen Beenden.

---

## 4. Konfiguration

### 4.1 COM-Port-Angabe

Ports können in verschiedenen Formaten angegeben werden:

| Eingabe | Ergebnis |
|---------|----------|
| `5` | `COM5` |
| `COM5` | `COM5` |
| `com5` | `COM5` |

```bash
uart_listener --rx-port COM5 --tx-port 6
```

### 4.2 Baudrate

Standard ist 115200. Ändern mit `--baud`:

```bash
uart_listener --rx-port 5 --tx-port 6 --baud 9600
```

### 4.3 Farbkodierung

Farben können per Name oder ANSI-Code angegeben werden:

**Per Name (case-insensitive):**
```bash
uart_listener --rx-port 5 --tx-port 6 --rx-color green --tx-color red
```

**Per ANSI-Code:**
```bash
uart_listener --rx-port 5 --tx-port 6 --rx-color "\033[92m" --tx-color "\033[91m"
```

**Farbtest beim Start:**

Das Programm zeigt beim Start eine Testausgabe mit den konfigurierten Farben:
```
[RX] Port COM5 ready (color test)      <- grün
[TX] Port COM6 ready (color test)      <- rot
```

Falls ANSI-Farben nicht unterstützt werden, erscheint eine Warnung:
```
Warning: Could not enable ANSI colors (Virtual Terminal Processing).
         Color output may not work correctly.
```

**Verfügbare Farben:**

| Name | ANSI-Code | Beschreibung |
|------|-----------|--------------|
| `default` | `\033[0m` | Reset |
| `red` | `\033[31m` | Standard Rot |
| `green` | `\033[32m` | Standard Grün |
| `yellow` | `\033[33m` | Standard Gelb |
| `blue` | `\033[34m` | Standard Blau |
| `cyan` | `\033[36m` | Standard Cyan |
| `brightred` | `\033[91m` | Helles Rot |
| `brightgreen` | `\033[92m` | Helles Grün |
| `brightcyan` | `\033[96m` | Helles Cyan |

### 4.4 Timestamps deaktivieren

```bash
uart_listener --rx-port 5 --tx-port 6 --no-ts
```

### 4.5 Single-Port-Modus

Mit `--dual-off` kann nur ein Port überwacht werden:

```bash
# Nur RX abhören
uart_listener --rx-port 5 --dual-off

# Nur TX abhören  
uart_listener --tx-port 6 --dual-off
```

**Ausgabe im Single-Port-Modus:**
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

## 5. Ausgabeformate

### 5.1 Anzeigeformat (`--format`)

| Format | Beschreibung | Beispiel |
|--------|--------------|----------|
| `ascii` | Druckbare Zeichen, Rest als `\xNN` | `Hello\x0D\x0A` |
| `hex` | Hexadezimale Bytes | `48 65 6C 6C 6F 0D 0A` |
| `c-escape` | C-Style Escapes | `Hello\r\n` |
| `raw` | Nur Byte-Anzahl | `<raw 7 bytes>` |

```bash
uart_listener --rx-port 5 --tx-port 6 --format hex
```

### 5.2 Log-Format (`--log-format`)

**Text (Standard):**
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

### 5.3 Raw-Binary-Ausgabe

Für nachträgliche Analyse können die Rohdaten separat gespeichert werden:

```bash
uart_listener --rx-port 5 --tx-port 6 --rx-raw-out rx_dump.bin --tx-raw-out tx_dump.bin
```

---

## 6. Stolpersteine und Lösungen

### 6.1 ANSI-Farben werden nicht angezeigt

**Problem:** Statt Farben erscheinen Escape-Sequenzen wie `[32m`.

**Ursache:** Windows Terminal Processing ist nicht aktiviert.

**Diagnose:** Beim Start erscheint:
```
Warning: Could not enable ANSI colors (Virtual Terminal Processing).
```

**Lösung:** 
- Windows Terminal oder neueres ConEmu verwenden
- Windows 10 1809+ ist erforderlich für automatische ANSI-Unterstützung

### 6.2 COM-Port bereits in Verwendung

**Problem:** `[INFO] Opening COM5 for RX... FAILED`

Gefolgt von:
```
Error opening port \\.\COM5 (Error code: 5)
```

**Ursache:** Port ist durch anderes Programm belegt (Error 5 = Access Denied).

**Lösung:**
1. Geräte-Manager prüfen
2. Andere serielle Terminals schliessen (HTerm, PuTTY, etc.)
3. Anderen Port verwenden

### 6.3 Keine Daten empfangen

**Problem:** Programm läuft, aber keine Ausgabe.

**Mögliche Ursachen:**

| Ursache | Prüfung |
|---------|---------|
| Falsche Baudrate | Mit Oszilloskop oder Logic Analyzer verifizieren |
| Verkabelung | RX/TX vertauscht? GND verbunden? |
| Kein Traffic | Andere Seite sendet nicht |

---

## 7. Troubleshooting

### Checkliste

- [ ] USB-Adapter erkannt? (Geräte-Manager)
- [ ] Richtige COM-Port-Nummer?
- [ ] Baudrate korrekt?
- [ ] RX-Leitung verbunden (nicht TX)?
- [ ] GND zwischen Adapter und Zielgerät verbunden?

### Häufige Fehler

| Fehler | Ursache | Lösung |
|--------|---------|--------|
| `Opening COMx... FAILED` | Port belegt oder existiert nicht | Port-Nummer prüfen, andere Programme schliessen |
| `Error code: 5` | Access Denied | Port wird bereits verwendet |
| `Error code: 2` | Port existiert nicht | COM-Nummer im Geräte-Manager prüfen |
| Keine Ausgabe | Kein Traffic oder Baudrate falsch | Verkabelung/Baudrate prüfen |
| Unlesbare Zeichen | Falsches Format | `--format hex` verwenden |
| `[ESC pressed]` erscheint nicht | Konsole blockiert | CLI-Argumente statt interaktiver Eingabe nutzen |

---

## 8. Siehe auch

- [UART_Listener_Reference](../reference/UART_Listener_Reference.md) — CLI-Optionen Referenz
- [Color_Reference](../reference/Color_Reference.md) — Verfügbare Farben

---

## Changelog

| Version | Datum | Änderungen |
|---------|-------|------------|
| **1.1.0** | **2026-01-13** | **Neu: Single-Port-Modus (`--dual-off`)** |
| 1.0.0 | 2026-01-13 | Initial Release |
