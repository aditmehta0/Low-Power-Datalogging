# Code Overview

**Version A**
Initialization:
The RTC SQW pin is configured to control a P‑channel MOSFET that supplies power to both the Arduino and SD card module.

The DS3231 RTC is initialized over I2C. (If the RTC isn’t running, you might want to initialize it with the current compile time.)

Unique File Creation & Data Logging:
A filename is generated (e.g., DATA000.CSV, DATA001.CSV, etc.) by checking for file existence.
Five rows of data (timestamp and a simulated sensor reading) are written into this file.

RTC Alarm Setting:
After logging, an alarm is set for 10 minutes in the future. This alarm should trigger the external circuit (via the RTC’s INT/SQW pin) to reapply power.

Power-Down / Deep Sleep:
The Arduino + Sd Card while it is still powered, power off is triggered over I2C by sending a reset alarm command to RTC.
With external circuitry, the RTC alarm can fully power off the Arduino and SD card between logging sessions.

