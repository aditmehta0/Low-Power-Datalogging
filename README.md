🌱 Ultra-Low Power Arduino Datalogger

Accurate, battery-operated data logging with minimal power consumption

📘 Overview

This project demonstrates a highly energy-efficient Arduino-based datalogger that logs sensor data every 10 minutes to an SD card and sleeps the rest of the time. It leverages the precision timing of the DS3231 RTC and selectively powers only essential components to minimize current consumption.

Two versions are considered:

Version A: Arduino Pro Mini + DS3231 RTC + microSD + MOSFET Power Gating

Version B: Barebones ATmega328P + microSD + watchdog sleep (no RTC)

🚀 Features

Ultra-low power sleep: ~1.2 µA (RTC) or <0.5 µA (barebones)

SD card logging with uniquely named files every 10 minutes

RTC-alarm or watchdog wake-up mechanism

Complete BOM and schematics

🔋 Power Comparison

Feature

Version A (RTC)

Version B (Barebones)

Sleep Current

~1.2 µA (RTC only)

~0.1–0.35 µA

Logging Current

~15–40 mA

~10–20 mA

Time Accuracy

±2 ppm (DS3231)

~±1–10% (WDT drift)

Timestamp Support

✅ Yes

❌ No (unless RTC added)

Complexity

Medium

Low

Battery Life (Est.)

6 months – 1 year

2–5+ years

🧠 Architecture

⏱ Version A: RTC-Controlled Logging

DS3231 RTC alarm triggers every 10 minutes

RTC INT pin pulls down a P-MOSFET gate

Arduino + SD power on

Arduino logs 5 data points to a unique CSV file

Arduino resets alarm & cuts its own power

🧮 Version B: Watchdog Timer Logging

ATmega328P uses WDT to wake up every 8s

Counts up to 10 minutes, logs data, then sleeps again

All peripherals (ADC, SPI, Serial, etc.) are disabled in software

SD card powered via MOSFET or digital pin

📦 Bill of Materials

Component

Recommended Part

Microcontroller

Arduino Pro Mini (3.3V, 8 MHz) / ATmega328P

RTC Module

DS3231 (Adafruit or SparkFun recommended)

SD Card Module

SPI microSD module

MOSFET (Power Switch)

P-Channel MOSFET (e.g. IRLML6402)

Backup Battery

CR2032 + holder

Main Battery

3.7V LiPo / 3xAA alkaline

📁 File Structure

LowPowerLogger/
├── README.md
├── LowPowerLogger.ino       // Main Arduino sketch
├── circuit_diagram.png      // Annotated schematic
├── parts_list.csv           // Full BOM
├── power_budget.xlsx        // Battery life estimation

🧰 Setup Instructions

Connect hardware according to schematic (MOSFET-controlled VCC, RTC INT to gate)

Upload sketch via FTDI/ISP

Insert formatted microSD card

Supply 3.3V via battery

Monitor file creation every 10 minutes

🧪 Performance Summary

SD writes: ~300 ms total

RTC sleep: ~1.2 µA standby

WDT sleep: ~0.1 µA on ATmega

RTC coin cell lasts: 5–10 years

📸 Gallery / Diagrams



✅ When to Use Which Version?

Use Case

Best Version

Need accurate timestamps

RTC + Pro Mini

Need >1 year battery life

Barebones ATmega

Want fastest prototyping

RTC + Pro Mini

Want fewest parts, lowest draw

Barebones ATmega328

🤝 Acknowledgments

Nick Gammon's deep-dive into low-power AVR design: gammon.com.au/power

Adafruit for RTC breakout design


