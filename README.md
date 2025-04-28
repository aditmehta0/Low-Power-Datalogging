# 🌱 Ultra-Low Power Arduino Datalogger

**Accurate, battery-operated data logging with minimal power consumption**

---

### 📘 Overview

This project implements a highly energy-efficient Arduino-based datalogger that logs sensor data every 10 minutes to a microSD card, then shuts down to save power.

Two optimized variants:

- **Version A**: Arduino Pro Mini + DS3231 RTC + SD card, with MOSFET full power gating
- **Version B**: Barebones ATmega328P + SD card + watchdog sleep, no RTC

One basic variant: 
- **Version C**: Arduino Pro Mini + P-channel MOSFET driven by a GPIO pin + watchdog sleep, no RTC

---

### 🚀 Features

- ⏱ RTC- or WDT-triggered wakeups
- 📂 Unique file logging every 10 minutes
- 🔋 Sleep current as low as 0.1 µA to 1.2 µA
- 📅 Optional highly accurate RTC timestamps
- 📆 Compact BOM and low system cost

---

### 🔋 Power Consumption Comparison

| Feature               | Version A (RTC + Pro Mini) | Version B (Barebones) | Version C (Pro Mini + MOSFET) |
| --------------------- | -------------------------- | --------------------- | ----------------------------- |
| **Sleep current**     | ∼1.2 µA (RTC only)       | **∼0.1–0.35 µA**       | ∼0.1 µA (true shutdown)   |
| **Logging current**   | ∼30–50 mA               | ∼10–20 mA            | ∼10–20 mA                 |
| **Wake mechanism**    | DS3231 RTC Alarm           | Watchdog Timer (WDT)   | Watchdog Timer (WDT)          |
| **Time accuracy**     | ±2 ppm                  | ±1–10%              | ±1–10%                     |
| **Battery life**      | ∼15–20 months             | **2–5+ years**         | ∼1.5–2 years                 |
| **Timestamp support** | ✅ Yes                   | ❌ No (unless RTC added) | ❌ No (unless added)         |
| **Design complexity** | Moderate                   | Simple                 | Very Simple                   |

---

### 🧐 Architectures

#### ⏱ Version A: RTC-Driven Full System Shutdown
- DS3231 RTC triggers alarm via SQW pin (pulsed low)
- P-Channel MOSFET turns on Arduino + SD card
- Arduino logs data to uniquely named file
- Arduino programs next alarm, cuts its own power

#### 🧮 Version B: Barebones Watchdog Sleep
- ATmega328P runs internal oscillator (8 MHz or 128 kHz)
- WDT wakes system every 8 seconds
- After 10 minutes, data is logged
- SD powered optionally via MOSFET/GPIO

#### 🧮 Version C: Simple MOSFET SD Card Cutoff
- Arduino Pro Mini sleeps using WDT
- MOSFET switches SD card power on during logging
- No RTC; approximate 10-minute cycle using watchdog

---

### 📏 Bill of Materials

| Component             | Suggested Part                            |
| --------------------- | ----------------------------------------- |
| Microcontroller       | Arduino Pro Mini (3.3V, 8 MHz) or ATmega328P |
| RTC Module (Version A) | DS3231 Module (Adafruit, SparkFun)         |
| SD Card Module         | SPI microSD module                       |
| P-Channel MOSFET       | NDP6020P (TO-220) / IRLML6402 (SMD)        |
| Pull-up Resistors      | 10kΩ for MOSFET Gate pull-up             |
| Backup Battery         | CR2032 coin cell + holder (RTC)           |
| Main Battery           | 3.2V LiFePO₄ 14500 / 3xAA NiMH            |
| Capacitors             | 0.1µF across SD card VCC/GND (optional)   |

---

### 📁 Project Structure

```
LowPowerLogger/
├── README.md
├── Version A
│   ├── VersionA.ino            # Main Arduino sketch vA
│   ├── schematicA.png          # Circuit diagram A
├──  Version B
│   ├── VersionB.ino            # Main Arduino sketch vB
│   ├── schematicB.png          # Circuit diagram B
├── Simple Version
│   ├── Code Example C
│   │   ├── VersionC.ino        # Main Arduino sketch vC
│   ├── LowPowerVerC_SD_bb.jpg  # Circuit diagram C
```

---

### 🛠️ Setup Instructions

- Connect components per relevant schematic
- Remove power LED and regulator from Pro Mini for best idle currents
- Flash sketches using FTDI or ISP
- Insert formatted microSD card (FAT32)
- Insert coin cell backup battery for RTC if using Version A
- Power the system with main battery (LiFePO₄ or 3xAA)
- Watch new files being created every 10 minutes!

---

### 🌓 RTC Initial Configuration (Version A)

When flashing Version A code:
- Disable 32kHz output (EN32kHz bit)
- Disable continuous SQW output
- Enable alarm 1 interrupt only

✅ This is done automatically in `setup()` the first time Arduino runs.
✅ No manual intervention needed.

---

### 💪 Power Consumption Summaries

#### Version A (RTC + Pro Mini)

| Phase                            | Current Estimate    |
| -------------------------------- | -------------------- |
| RTC Standby (Off)                | ~1.2 µA             |
| Boot + Setup Phase               | ~15–20 mA          |
| SD Write Phase                   | ~30–50 mA          |
| RTC Alarm Set                    | ~3–5 mA             |
| Sleep (System Off)                | ~1.2 µA             |

- Active duration: ~2 seconds
- Sleep duration: ~598 seconds

**Average Current**: ~133 µA

Estimated Battery Life:
- 3.2V LiFePO₄ 14500: ~15 months
- 3xAA NiMH: ~18–20 months

---

#### Version C (Pro Mini + MOSFET SD Switch)

| Phase                            | Current Estimate    |
| -------------------------------- | -------------------- |
| Pro Mini Standby (Deep Sleep)    | ~0.1–0.35 µA       |
| Boot + Setup Phase               | ~10–20 mA          |
| SD Write Phase                   | ~10–20 mA          |

- Active duration: ~2 seconds
- Sleep duration: ~598 seconds

**Average Current**: ~110 µA

Estimated Battery Life:
- 3.2V LiFePO₄ 14500: ~18–20 months
- 3xAA NiMH: ~24+ months

---

### 📸 Visuals

- 📍 Added schematicA.png for Version A
- 📍 Yet to add schematicB.png for Version B
- 📍 Added schematicC.png for Version C

---

### 📚 References

- [DS3231 RTC Datasheet](https://datasheets.maximintegrated.com/en/ds/DS3231.pdf)
- [Nick Gammon’s Low Power Techniques](http://gammon.com.au/power)
- [Forum: MicroSD card current draw](https://forum.arduino.cc/t/current-draw-of-microsd-card-module/623331/2)
- [Adafruit RTC Forum - SQW Behavior](https://forums.adafruit.com/viewtopic.php?t=45933)

---
