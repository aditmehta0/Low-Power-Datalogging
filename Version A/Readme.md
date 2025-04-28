# 🌱 Ultra-Low Power Arduino Datalogger - Version A (RTC-based)

**Accurate, battery-operated data logging with RTC-triggered wakeups and full system power shutdown**

---

### 📘 Overview

This project implements an ultra-low-power Arduino datalogger using a Real-Time Clock (RTC) to control system wake-ups every 10 minutes.  
The RTC powers on the Arduino and SD card via a P-Channel MOSFET, logging 5 rows of data into a uniquely named file, then cleanly powers the entire system off.

---

### 🚀 Features

- ⏱ RTC alarm-triggered wakeups (DS3231)
- 📂 Unique file logging every 10 minutes
- 🔋 Sleep current as low as 1.2 µA (RTC only)
- 🛑 Full system shutdown between events
- 📆 Highly accurate timestamps (±2 ppm)
- 🗕 Low-cost, easy-to-source components

---

## ⚡ Power Consumption Breakdown (Version A)

| Phase                            | Current Estimate    | Notes                                          |
| -------------------------------- | -------------------- | ---------------------------------------------- |
| **RTC Standby (System Off)**      | ~1.2 µA              | Only RTC running on coin cell                  |
| **MOSFET Turns ON → Boot Phase**  | ~15–20 mA            | Arduino boots up, SD initializes               |
| **SD Card File Writing Phase**    | ~30–50 mA            | Writing 5 rows to microSD (short burst)         |
| **RTC Alarm Setting Phase**       | ~3–5 mA              | RTC programmed for next 10-minute alarm        |
| **System Shutdown (Cutoff)**      | back to ~1.2 µA      | Arduino, SD powered off, RTC standby only      |

---

## 📈 Energy Usage Estimation

- **Active time** (Boot + Write + Setup) ≈ ~2 seconds per 600 seconds (10 minutes)
- **Sleep time** (RTC only) ≈ ~598 seconds
- **Average Current** ≈ (2 seconds × 40 mA + 598 seconds × 1.2 µA) ÷ 600 seconds ≈ 133 µA average

---

## 🔋 Battery Life Estimation

| Battery Type        | Capacity (mAh) | Estimated Life        |
| -------------------- | -------------- | ---------------------- |
| 3.2V LiFePO₄ 14500   | ~1500 mAh       | ~11,000+ hours (~15 months) |
| 3×AA NiMH (Eneloop)  | ~2000 mAh       | ~18–20 months          |

✅ **Easily achieves 1+ year battery life** on modest-sized batteries!

---

### 🧐 Architecture

- RTC (DS3231) triggers alarm on SQW pin.
- P-Channel MOSFET is normally OFF (SQW HIGH = OFF).
- Alarm time (SQW LOW) → MOSFET ON → Arduino + SD powered.
- Arduino logs data → sets next RTC alarm → disables SQW output.
- Arduino loses power; system idles at RTC standby current (~1.2 µA).

---

### 📟 Bill of Materials

| Component             | Suggested Part                            |
| ---------------------- | ----------------------------------------- |
| Microcontroller        | Arduino Pro Mini (3.3V, 8MHz)             |
| RTC Module             | DS3231 Module (Adafruit, SparkFun, AliExpress) |
| SD Card Module         | microSD SPI module                       |
| P-Channel MOSFET       | NDP6020P (TO-220 for breadboard friendly) |
| Pull-up Resistor       | 10kΩ Gate pull-up to battery +            |
| Battery (Backup RTC)   | CR2032 coin cell + holder                 |
| Battery (Main Power)   | 3.2V LiFePO₄ 14500 or 3×AA                |
| Capacitor (optional)   | 0.1µF SD module VCC to GND (filter)       |

---

### 📋 Wiring

| Arduino Pin   | Connected To                           |
|---------------|-----------------------------------------|
| VCC           | Drain of MOSFET (shared with SD Module) |
| GND           | Ground (shared with battery - and SD)   |
| D2            | RTC SQW output (input to Arduino for optional monitoring) |
| I2C (A4, A5)  | RTC SDA, SCL lines                      |
| D10           | SD Card CS                              |
| D11           | SD Card MOSI                            |
| D12           | SD Card MISO                            |
| D13           | SD Card SCK                             |
| Battery +     | Source of P-Channel MOSFET              |
| MOSFET Gate   | RTC SQW pin (pulled up with 10kΩ resistor) |

- Add a 10kΩ pull-up resistor between MOSFET Gate and battery +
- 3.2V battery directly powers MOSFET Source
- SD card and Arduino powered only during active phase

✅ **Note**: Coin cell powers only the RTC backup Vbat pin.  
✅ **Note**: Main battery powers the Arduino and SD Card via MOSFET.

---

### 🛠 Setup Instructions

1. Wire Arduino, RTC, SD Card, MOSFET exactly as shown in schematic.
2. Remove the Arduino Pro Mini **power LED** and (optionally) **regulator** for best sleep performance.
3. Flash Arduino sketch via FTDI adapter at 3.3V logic.
4. Insert FAT32-formatted microSD card.
5. Insert backup CR2032 for RTC module.
6. Power the system with a 3.2V LiFePO₄ or 3×AA battery pack.
7. System will create a uniquely named file every 10 minutes after wakeup!

---

### 🛠 RTC Initial Configuration (First Flash)

> When you first upload your sketch or after battery replacement, you must configure the DS3231 control registers:

- Disable continuous square wave output (SQWE = 0)
- Disable 32kHz output (EN32kHz = 0)
- Enable Alarm 1 interrupt (A1IE = 1)
- Interrupts will pulse LOW when alarm matches

✅ **No manual I2C register setup needed!**  
✅ **The Arduino VersionA.ino sketch will automatically configure the RTC in `setup()`**.

⚡ **Warning:**  
If you skip this step, the RTC will output a continuous square wave instead of pulsing LOW at alarm time, and the system will never shut down.

---

### 📸 Visuals

- ✅ Schematic showing RTC Alarm control of full system power
- ✅ Full wiring diagram with battery, MOSFET, Arduino, SD card
- ✅ Example data logs pending testing phase

---

### 📚 References

- [DS3231 RTC Datasheet](https://datasheets.maximintegrated.com/en/ds/DS3231.pdf)
- [Nick Gammon's Low Power Techniques](http://gammon.com.au/power)
- [Measured average SD write current](https://forum.arduino.cc/t/current-draw-of-microsd-card-module/623331/2)
- [RTC SQW/Alarm Pin Behavior Details](https://forums.adafruit.com/viewtopic.php?t=45933)
---
