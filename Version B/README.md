# ⚡️ Ultra-Low Power Arduino Datalogger - Version B (Barebones)

**A minimal, ultra-low-power datalogger using a barebones ATmega328P and watchdog sleep**  

---

## 📘 Overview

Version B runs on a standalone ATmega328P at 8 MHz with no USB-serial chip or power LED, using a low-quiescent LDO for 3.3 V. It wakes via the Watchdog Timer every 10 minutes (8 s × 75) to power the SD card through a P-channel MOSFET, logs 5 rows to a uniquely named CSV file, then returns to deep sleep at ~0.36 µA :contentReference[oaicite:0]{index=0} :contentReference[oaicite:1]{index=1}.

---

## 🚀 Features

- ⏱ **Watchdog wake-ups:** 8 s intervals, 75× for ~10 min :contentReference[oaicite:2]{index=2}  
- 🔋 **Deep sleep current:** ~0.36 µA (ADC & BOD disabled) :contentReference[oaicite:3]{index=3}  
- ⚡️ **Active burst current:** ~30–50 mA during SD writes :contentReference[oaicite:4]{index=4}  
- 🛑 **SD power-gating:** IRLML6402 P-channel MOSFET :contentReference[oaicite:5]{index=5}  
- 🗕 **Minimal components:** MCU, LDO, MOSFET, pull-ups, decoupling

---

## ⚙️ Regulator Decision

We use the **MCP1700-33** LDO (3.3 V, 250 mA, 1.6 µA quiescent, 178 mV dropout), ideal for 3×AA or LiPo, extending sleep life :contentReference[oaicite:6]{index=6}.

---

## 📟 Bill of Materials

| Component                | Specification & Notes                                                                                               | Reference |
|--------------------------|---------------------------------------------------------------------------------------------------------------------|:---------:|
| **ATmega328P MCU**       | Barebones DIP, 8 MHz Optiboot-bootloaded                                                                            |    [8]    |
| **LDO**                  | MCP1700-33 (3.3 V, 250 mA, 1.6 µA I<sub>Q</sub>)                                                                    |    [3]    |
| **P-Channel MOSFET**     | IRLML6402 (V<sub>DS</sub>=20 V, R<sub>DS(on)</sub><75 mΩ @ V<sub>GS</sub>=–2.5 V)                                   |   [4][5]  |
| **Pull-up Resistors**    | 10 kΩ on RESET (PC6), 10 kΩ on MOSFET gate; 100 Ω series on MOSFET gate (optional for gate protection)              |    [7]    |
| **Decoupling Capacitors**| 0.1 µF on VCC/GND near MCU and SD module; 4.7 µF on LDO VIN→VOUT                                                    |    [3]    |
| **Crystal & Caps**       | 8 MHz crystal with two 22 pF caps to GND                                                                            |    [1]    |
| **microSD Module**       | SPI microSD breakout                                                                                                |    [6]    |
| **Battery**              | 3×AA NiMH (≈4.0 V) or single-cell LiPo (3.7 V) feeding LDO VIN                                                      |    —      |

---

## 📋 Wiring & Small Components

| Pin/Node               | Connection & Component                                      |
|------------------------|-------------------------------------------------------------|
| **Battery + (4.5 V)**  | → MCP1700 VIN                                               |
| **MCP1700 VOUT (3.3 V)**| → MCU VCC, MOSFET Source, SD VCC                           |
| **GND**                | → Common ground (MCU, SD, MOSFET, battery)                  |
| **MOSFET gate (D4)**   | ← MCU D4 → 100 Ω series → gate; 10 kΩ pull-up to 3.3 V      |
| **RESET (PC6)**        | 10 kΩ pull-up to 3.3 V; 0.1 µF cap RESET→GND :contentReference[oaicite:12]{index=12} |
| **Crystal XTAL1/2**    | 8 MHz crystal + 22 pF caps to GND                           |
| **SD CS (D10)**        | ← SD module CS                                              |
| **SD MOSI (D11)**      | ← SD module MOSI                                            |
| **SD MISO (D12)**      | → SD module MISO                                            |
| **SD SCK (D13)**       | ← SD module SCK                                             |

---

## 🔌 Power-Saving Techniques

1. **Barebones board (no reg/LED):** saves ~30.5 mA [1]  
2. **Sleep mode (SLEEP_MODE_PWR_DOWN):** saves ~15.7 mA [1]  
3. **Disable ADC (ADCSRA = 0):** saves ~334 µA [1]  
4. **Disable BOD (software via MCUCR):** saves ~25 µA [1]  
5. **Power Reduction Register (power_all_disable()):** saves ~7.6 mA [1]  
6. **Pins OUTPUT/LOW:** unused pins as outputs low save ~2 µA [1]  
7. **Watchdog overhead:** ~6.2 µA during sleep [2]  
8. **8 MHz crystal clock:** balanced speed/stability [1]  

---

## 📄 Code Snippet

```cpp
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <SdFat.h>

SdFat sd;
const uint8_t MOSFET_PIN = 4, SD_CS = 10;
volatile uint8_t wdtCount = 0, logFlag = 0;

ISR(WDT_vect) {
  if (++wdtCount >= 75) { logFlag = 1; wdtCount = 0; }
}

void setup() {
  // Gate OFF (SD & MCU power gated by MOSFET)
  DDRD |= (1<<MOSFET_PIN);
  PORTD |= (1<<MOSFET_PIN);

  // ADC off
  ADCSRA = 0;
  // BOD off
  MCUCR = (1<<BODS)|(1<<BODSE);
  MCUCR = (1<<BODS);
  // Disable peripherals
  power_all_disable();

  // WDT interrupt every 8s
  MCUSR &= ~(1<<WDRF);
  WDTCSR |= (1<<WDCE)|(1<<WDE);
  WDTCSR = (1<<WDIE)|(1<<WDP3)|(1<<WDP0);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
}

void loop() {
  if (logFlag) {
    logFlag = 0;
    // Power SD on
    PORTD &= ~(1<<MOSFET_PIN);
    delay(50);

    if (sd.begin(SD_CS)) {
      char fname[12];
      for (uint16_t i=0; i<1000; i++) {
        sprintf(fname, "DATA%03u.CSV", i);
        if (!sd.exists(fname)) break;
      }
      File f = sd.open(fname, FILE_WRITE);
      if (f) {
        for (uint8_t j=0; j<5; j++) f.println("Sample data");
        f.close();
      }
    }
    // Power SD off
    PORTD |= (1<<MOSFET_PIN);
  }
  sleep_cpu();
}
```
---
## ⚡ Power Consumption Breakdown (Version B)

|   Phase                | Current Estimate | Description                    |
|-------------------------|------------------|--------------------------------|
| Boot & SD write (2 s)    | ~40 mA            | SD card initialized and 5 rows written |
| WDT sleep overhead       | ~6.2 µA           | Watchdog Timer interrupt active |
| Deep sleep (598 s)       | ~0.36 µA          | MCU in SLEEP_MODE_PWR_DOWN, ADC off, BOD off, all peripherals disabled |

---

### 📈 Energy Usage Estimation

- **Active time** (Boot + Write) ≈ ~2 seconds per 600 seconds (10 minutes)
- **Sleep time** ≈ ~598 seconds
- **Average Current Consumption**:

$$
\text{Average} = \frac{(2\text{s} \times 40\text{mA}) + (598\text{s} \times 0.36\mu\text{A})}{600\text{s}} \approx 133\mu\text{A}
$$

---

## 🔋 Battery Life Estimates

| Battery Type           | Capacity (mAh) | Estimated Runtime (based on ~133 µA avg) |
|-------------------------|----------------|------------------------------------------|
| 3×AA NiMH (Eneloop)     | 1900 mAh        | ≈ 14,285 hours (≈595 days ≈1.6 years)    |
| Single-cell LiPo (3.7V) | 1500 mAh        | ≈ 11,278 hours (≈470 days ≈1.3 years)    |

---

✅ **Result:**  
Even on modest battery packs, **over 1–1.5 years** of continuous unattended operation is realistically achievable!

---

## 📝 References

1. [Gammon: Power-saving techniques for ATmega328P](http://www.gammon.com.au/power)  
2. [Gammon: Watchdog Timer deep-sleep overhead](http://gammon.com.au/interrupts)  
3. [Microchip MCP1700-33 LDO datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/MCP1700-Low-Quiescent-Current-LDO-20001826E.pdf)  
4. [Infineon IRLML6402 datasheet (RDS(on), VGS(th))](https://www.micros.com.pl/mediaserver/TIRLML6402_TEC_TECH_PUBLIC_0001.pdf)  
5. [DigiKey: IRLML6402 product page](https://www.digikey.com/en/products/detail/umw/IRLML6402/16705881)  
6. [NXP Community: Typical SD card power consumption](https://community.nxp.com/t5/LPCXpresso-IDE/typical-SD-card-power-consumption/m-p/528444)  
7. [Arduino StackExchange: RESET pull-up resistor guidelines](https://arduino.stackexchange.com/questions/93447/barebones-avr-connecting-reset-pin-to-vcc)  
8. [Instructables: Optiboot burning guide for ATmega328P](https://www.instructables.com/Atmega328P-PU-Bootloader-Optiboot-Burning-Guide/)  
9. [Adafruit: Disabling DS3231 SQW/32kHz outputs](https://forums.adafruit.com/viewtopic.php?t=45933)  
10. [Gough’s Tech Zone: microSD card write current experiments](https://goughlui.com/2021/02/27/experiment-microsd-card-power-consumption-spi-performance/)  
