# 🌱 Ultra-Low Power Arduino Datalogger

**Accurate, battery-operated data logging with minimal power consumption**

---

### 📘 Overview

This project implements a highly energy-efficient Arduino-based datalogger that logs sensor data every 10 minutes to a microSD card, then shuts down to save power.

Two optimized variants:

- **Version A**: Arduino Pro Mini + DS3231 RTC + SD card, with MOSFET power gating
- **Version B**: Barebones ATmega328P + watchdog sleep, no RTC

---

### 🚀 Features

- ⏱ RTC- or WDT-triggered wakeups
- 📂 Unique file logging every 10 minutes
- 🔋 Sleep current as low as 0.1 µA
- 🗕 RTC-based timestamps (optional)
- 📆 Compact parts list with low BOM cost

---

### 🔋 Power Consumption Comparison

| Feature               | Version A (RTC + Pro Mini) | Version B (Barebones)   |
| --------------------- | -------------------------- | ----------------------- |
| **Sleep current**     | \~1.2 µA (RTC only)        | **\~0.1–0.35 µA**       |
| **Logging current**   | \~15–40 mA                 | \~10–20 mA              |
| **Wake mechanism**    | DS3231 RTC Alarm           | Watchdog Timer (WDT)    |
| **Time accuracy**     | ±2 ppm                     | ±1–10%                  |
| **Battery life**      | \~6–12 months              | **2–5+ years**          |
| **Timestamp support** | ✅ Yes                      | ❌ No (unless RTC added) |
| **Design complexity** | Moderate                   | Simple                  |

---

### 🧐 Architecture

#### ⏱ Version A – RTC-Driven Logging

- DS3231 RTC triggers alarm every 10 minutes via `SQW` pin
- P-Channel MOSFET turns on Arduino + SD card
- Arduino writes 5 rows to a **uniquely named file** (e.g., `DATA003.CSV`)
- Arduino sets next alarm and **cuts its own power**

#### 🧮 Version B – Barebones ATmega328P

- Runs internal 8 MHz or 128 kHz oscillator
- Watchdog timer sleeps in 8s chunks
- After 10 minutes, logs data, then sleeps again
- SD card optionally powered via GPIO or MOSFET

---

### 📟 Bill of Materials

| Component        | Suggested Part                               |
| ---------------- | -------------------------------------------- |
| Microcontroller  | Arduino Pro Mini (3.3 V, 8 MHz) / ATmega328P |
| RTC Module       | DS3231 (Adafruit or SparkFun)                |
| SD Card Module   | SPI microSD module                           |
| P-Channel MOSFET | IRLML6402 or similar                         |
| Backup Battery   | CR2032 coin cell + holder                    |
| Main Battery     | 3.7 V LiPo or 3×AA                           |

---

### 📁 Project Structure

```
LowPowerLogger/
├── README.md
├──Code Example
  ├── VersionA.ino        # Main Arduino sketch
  ├── VersionB.ino        # Main Arduino sketch
├── schematic
  ├── schematicA.png             # Circuit diagram A
  ├── schematicB.png             # Circuit diagram B
```

---

### 🛠 Setup Instructions

1. Connect RTC, Arduino, SD card, and MOSFET as per schematic
2. Flash code using FTDI or ISP programmer
3. Insert a formatted microSD card
4. Power via LiPo/+coin cell
5. Observe new file creation every 10 minutes

---

### ⚙️ Recommended Sleep Settings (Barebones)

- Use `SLEEP_MODE_PWR_DOWN`
- Disable: ADC, Serial, BOD
- Use: `power_all_disable()` to shut down internal peripherals
- Use: 8s WDT + loop counter for \~10 min total sleep

---

### ✅ Which Version to Choose?

| Use Case                         | Best Version         |
| -------------------------------- | -------------------- |
| Need accurate timestamps         | RTC + Pro Mini       |
| Want max battery life (>2 years) | Barebones ATmega328P |
| Simplest build                   | Barebones            |
| Easy prototyping                 | RTC + Pro Mini       |

---

### 📸 Visuals

- ✅ Added `schematicA.png` of RTC + MOSFET + SD card system
- ✅ Added `schematicB.png` showing barebones logger build

---

### 🤝 Credits

- Nick Gammon’s legendary guide: [gammon.com.au/power](http://www.gammon.com.au/power)
- Adafruit for the RTC design inspiration

---

