# Ultra-Low Power Arduino Datalogger - Version C (Simple)

## Overview
This project implements a simple low-power datalogger using:
- Arduino Pro Mini (3.3V, 8MHz)
- P-Channel logic level MOSFET (NDP6020P) (Intentionally picked for non-smd package) https://www.onsemi.com/download/data-sheet/pdf/ndp6020p-d.pdf
- microSD card module
- Watchdog Timer (WDT) for wake-up

Every 10 minutes, the Arduino wakes up, powers on the SD card, saves 5 lines of data into a uniquely named file, and goes back to deep sleep. The SD card is properly powered off between events to conserve battery life.

---

## Features
- Wake up every 10 minutes using WDT
- Save 5 data entries to a uniquely named file (DATAxxx.CSV)
- Sleep current: < 0.1 µA
- Logging current: ~10-20 mA
- Estimated battery life: 1-2+ years (LiFePO4 or 3xAA)

---

## BOM (Bill of Materials)

| Component             | Suggested Part                              |
|-----------------------|---------------------------------------------|
| Microcontroller       | Arduino Pro Mini (3.3V, 8MHz)               |
| SD Card Module        | SPI microSD module                         |
| P-Channel MOSFET      | NDP6020P (TO-220 package)                   |
| Battery               | 3.2V LiFePO4 14500 or 3xAA NiMH             |
| Resistor 1            | 10kΩ pull-up for MOSFET gate               |
| Resistor 2            | 100Ω in series to gate (optional protection) |
| Capacitor (optional)  | 0.1µF across SD module VCC/GND             |

---

## Wiring

| Arduino Pin   | Connected To                          |
|---------------|----------------------------------------|
| D4            | Gate of MOSFET (through 100Ω resistor) |
| 3.3V Battery +| Source of MOSFET                      |
| MOSFET Drain  | SD Module VCC                         |
| GND           | GND (Arduino, SD module, Battery -)    |
| D10           | SD Card CS pin                         |
| D11           | SD Card MOSI                           |
| D12           | SD Card MISO                           |
| D13           | SD Card SCK                            |

- Add a 10kΩ resistor between MOSFET Gate and Source.
- 3.3V directly powers Pro Mini VCC pin.

---

## Complete Arduino Code (VersionC.ino)

---

## Setup Instructions

1. Wire Pro Mini, MOSFET, and SD card as described.
2. Flash the code via an FTDI adapter (use 3.3V logic).
3. Insert a formatted FAT32 microSD card.
4. Power the system with a 3.2V LiFePO4 battery.
5. Observe new files being created every 10 minutes.

---

## Requirements Mapping to Version C

| Requirement             | Version C Implementation                    |
|--------------------------|------------------------------------------------------------- |
| MCU                      | Arduino Pro Mini (3.3V, 8 MHz)                               |
| Sleep to save power      | Pro Mini sleeps in SLEEP_MODE_PWR_DOWN                       |
| Wake every 10 minutes    | Use WWDT interrupt, 8s x 75 cycles                           |    
| Data logging             | Save 5 rows into uniquely named file (e.g., DATA001.CSV)     |
| Minimize SD card power   | SD card powered by P-MOSFET (NDP6020P) controlled by D4      |
| Minimal idle current     | SD card and Pro Mini turned off between loggings, Pro Mini consumes <0.1 µA when asleep |
| Robust SD handling       | Using SdFat library (not SD.h) to reliably reinitialize after power-off |

---

## Notes
- Use SdFat instead of SD.h to avoid SD card initialization failure after reboot.
- Always ensure the MOSFET cleanly turns off SD card power between events.
- Use a pull-up resistor on the MOSFET gate to prevent accidental turn-on.
