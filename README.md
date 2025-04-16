# Low-Power-Datalogging

🧠 GOAL: Compare Two Designs for Ultra-Low-Power Logging

✳️ Design 1: RTC + SD + MOSFET Controlled Arduino
(Our "Problem A" design)

Wake-up via DS3231 RTC alarm (SQW/INT)

Arduino + SD powered only during logging via P-MOSFET

5 rows of data written every 10 minutes

Arduino sleeps via software OR powers off completely via RTC-controlled gate

✳️ Design 2: Barebones ATmega328P + SD Card
(Nick Gammon’s minimalist logger: https://www.gammon.com.au/power)

Custom ATmega328P running from internal 8 MHz or 128 kHz RC oscillator

Wake-up via watchdog timer (max 8s, repeated for longer delay)

No RTC, only internal timer

Uses PRR, ADC shutdown, brownout disable, and fuse tuning

SD card powered via MOSFET or digital pin

🔋 Power Consumption Comparison

State	RTC + Arduino (Problem A)	Barebones ATmega328P
Sleep Current	~1.2 µA (RTC only)	0.1–0.35 µA
Logging Current	~15–40 mA (Arduino + SD)	~10–20 mA (ATmega + SD)
Sleep Duration	10 minutes (RTC alarm)	10 minutes (75 × WDT 8s)
Wake Duration	~300–600 ms	~100–200 ms
Average Current	~3–8 µA	~1–4 µA
Timekeeping Accuracy	±2 ppm (DS3231)	±1–10% (WDT drift)
Battery Life	1+ year (depending on setup)	2–5+ years
✅ Winner in power: Barebones ATmega328P
✅ Winner in time accuracy + simplicity: RTC-based system

🧩 Hardware Complexity

Feature	RTC + Arduino	Barebones ATmega328P
Time source	DS3231 external RTC	Internal Watchdog (WDT)
SD Card control	P-MOSFET or digital pin	Digital pin / MOSFET
Power management	RTC-driven gate + sleep	Software sleep + ADC/BOD off
Programming method	USB-Serial (Pro Mini)	ISP or bootloader via FTDI
BOM complexity	Medium	Low
✅ Winner in minimal parts: Barebones ATmega328P
✅ Winner in modularity and scalability: RTC-based
