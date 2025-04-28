# 🌱 Ultra-Low Power Arduino Datalogger

**Accurate, battery-operated data logging with minimal power consumption**

---

### 📘 Overview

This project implements a highly energy-efficient Arduino-based datalogger that logs sensor data every 10 minutes to a microSD card, then shuts down to save power.

Two optimized variants:

- **Version A**: Arduino Pro Mini + DS3231 RTC + SD card, with MOSFET full system power gating
- **Version B**: Barebones ATmega328P + SD card + watchdog sleep (no RTC)

One basic variant:
- **Version C**: Arduino Pro Mini + P-channel MOSFET driven by a single GPIO pin + watchdog sleep (no RTC)

---

### 🚀 Features

- ⏱ RTC- or WDT-triggered wakeups
- 📂 Unique file logging every 10 minutes
- 🔋 Sleep current as low as 0.1–1.2 µA
- 🗕 RTC-based timestamps (optional)
- 📆 Compact parts list with low BOM cost

---

### 🔋 Power Consumption Comparison

| Feature               | Version A (RTC + Pro Mini) | Version B (Barebones)   |
| --------------------- | -------------------------- | ----------------------- |
| **Sleep current**     | ~1.2 µA (RTC only)         | ~0.1–0.35 µA             |
| **Logging current**   | ~15–40 mA                  | ~10–20 mA               |
| **Wake mechanism**    | DS3231 RTC Alarm           | Watchdog Timer (WDT)    |
| **Time accuracy**     | ±2 ppm                     | ±1–10%                  |
| **Battery life**      | ~6–12 months               | ~2–5+ years             |
| **Timestamp support** | ✅ Yes                     | ❌ No (unless RTC added) |
| **Design complexity** | Moderate                   | Simple                  |

---

### 🧐 Architecture

#### ⏱ Version A – RTC-Driven Logging
- DS3231 RTC triggers alarm via SQW pin.
- P-Channel MOSFET (NDP6020P) turns ON Arduino + SD Card.
- Arduino writes 5 rows to a uniquely named file (e.g., `DATA003.CSV`).
- Arduino programs next RTC alarm → System auto-powers OFF.
- Sleep current during OFF state: ~1.2 µA (RTC only).

#### 🧮 Version B – Barebones ATmega328P
- Uses internal oscillator (8 MHz / 128 kHz).
- Watchdog timer sleeps in 8s chunks.
- After ~10 minutes, logs data to SD.
- SD can be optionally power-gated via MOSFET or GPIO.

#### 🧮 Version C – MOSFET driven SD card power off
- No RTC used.
- Watchdog timer wakes Arduino every ~10 minutes.
- SD Card powered through P-MOSFET (NDP6020P) controlled via D4 pin.
- Arduino sleeps deeply between loggings (~0.1 µA consumption).

---

### 📟 Bill of Materials

| Component        | Version A                                  | Version C                        |
| ---------------- | ------------------------------------------ | -------------------------------- |
| Microcontroller  | Arduino Pro Mini (3.3 V, 8 MHz)            | Arduino Pro Mini (3.3 V, 8 MHz)  |
| RTC Module       | DS3231 (Adafruit/SparkFun)                 | —                                |
| SD Card Module   | microSD SPI module                         | microSD SPI module               |
| P-Channel MOSFET | NDP6020P (TO-220)                          | NDP6020P (TO-220)                |
| Pull-up Resistor | 10kΩ MOSFET gate pull-up                   | 10kΩ MOSFET gate pull-up         |
| Capacitor        | 0.1µF VCC decoupling (optional)             | 0.1µF VCC decoupling (optional)  |
| Backup Battery   | CR2032 for RTC                              | —                                |
| Main Battery     | 3.2 V LiFePO₄ 14500 / 3×AA (NiMH preferred) | 3.2 V LiFePO₄ 14500 / 3×AA       |

---

### 📁 Project Structure

LowPowerLogger/
├── README.md
├── Code Example A&B/
│   ├── VersionA.ino           # Arduino sketch for Version A
│   ├── VersionB.ino           # Arduino sketch for Version B
│   ├── schematicA.png         # Circuit diagram for Version A
│   ├── schematicB.png         # Circuit diagram for Version B
├── Simple Version/
│   ├── VersionC.ino           # Arduino sketch for Version C
│   ├── schematicC.png         # Circuit diagram for Version C
│   ├── LowPowerVerC_SD_bb.jpg # Breadboard visual Version C

---

### 🛠 Setup Instructions

#### For Version A:
1. Wire Arduino, RTC, SD Card, and MOSFET per schematicA.png.
2. Remove Arduino Pro Mini **power LED** and optionally **regulator** for best sleep performance.
3. Flash Arduino using FTDI adapter at 3.3V logic.
4. Insert formatted FAT32 microSD card.
5. Insert CR2032 coin cell into RTC module.
6. Power the system with 3.2 V LiFePO₄ or 3×AA battery.
7. The sketch will configure the RTC registers:
   - Disable SQW continuous square wave output
   - Enable RTC alarm interrupt
8. System wakes every 10 minutes, saves 5 rows, then fully shuts off.

#### For Version C:
1. Wire Arduino, SD Card, and MOSFET per schematicC.png.
2. Remove Arduino Pro Mini **power LED** and optionally **regulator** for best sleep performance.
3. Flash Arduino using FTDI adapter at 3.3V logic.
4. Insert formatted FAT32 microSD card.
5. Power via 3.2 V LiFePO₄ or 3×AA battery.
6. System wakes every ~10 minutes using Watchdog Timer (WDT), logs 5 rows, and powers down SD between events.

---

### 📸 Visuals

- ✅ `schematicA.png`: RTC triggers full power cycle
- ✅ `schematicB.png`: Barebones WDT-driven logger
- ✅ `schematicC.png`: Simplest GPIO-driven SD logger
- ✅ `LowPowerVerC_SD_bb.jpg`: Breadboard build for Version C

---

### 🧮 Final Power Consumption Summary

#### Version A:

| Phase                  | Current Estimate    |
| ----------------------- | ------------------- |
| RTC Sleep (System OFF)  | ~1.2 µA             |
| Wake and Log Phase      | ~30–50 mA           |
| Average Current         | ~133 µA             |
| Estimated Battery Life  | 12–18 months        |

#### Version C:

| Phase                  | Current Estimate    |
| ----------------------- | ------------------- |
| Sleep Mode             | ~0.1–0.35 µA         |
| Wake and Log Phase      | ~20–30 mA           |
| Average Current         | ~80–100 µA          |
| Estimated Battery Life  | 2–5+ years          |

---

### 📚 References

- [DS3231 RTC Datasheet](https://datasheets.maximintegrated.com/en/ds/DS3231.pdf)
- [Nick Gammon’s Low Power Techniques](http://gammon.com.au/power)
- [Using SdFat for reliable SD reboot handling](https://github.com/greiman/SdFat)
- [RTC SQW/Alarm Details](https://forums.adafruit.com/viewtopic.php?t=45933)

---
