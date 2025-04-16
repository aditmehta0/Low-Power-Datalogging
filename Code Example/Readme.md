
# Code Overview

Initialization:
The SD power pin is configured to control a P‑channel MOSFET that supplies power to both the Arduino and SD card module.

The DS3231 RTC is initialized over I2C. (If the RTC isn’t running, you might want to initialize it with the current compile time.)

Unique File Creation & Data Logging:
A filename is generated (e.g., DATA000.CSV, DATA001.CSV, etc.) by checking for file existence.
Five rows of data (timestamp and a simulated sensor reading) are written into this file.

RTC Alarm Setting:
After logging, an alarm is set for 10 minutes in the future. This alarm should trigger the external circuit (via the RTC’s INT/SQW pin) to reapply power.

Power-Down / Deep Sleep:
The SD card module is powered down by setting its control pin HIGH.
The Arduino enters deep sleep in 8‑second intervals (75 times to equal roughly 10 minutes).

With external circuitry, the RTC alarm can fully power off the Arduino and SD card between logging sessions.
