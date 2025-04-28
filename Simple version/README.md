# Ultra-Low Power Arduino Datalogger - Version C (Simple)

## Overview
This project implements a simple low-power datalogger using:
- Arduino Pro Mini (3.3V, 8MHz)
- P-Channel MOSFET (NDP6020P)
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
| Resistor 1            | 10kΩ pull-up for MOSFET gate               |
| Resistor 2            | 100Ω in series to gate (optional protection) |
| Capacitor (optional)  | 0.1µF across SD module VCC/GND             |

---

## Wiring

| Arduino Pin   | Connected To                          |
|---------------|----------------------------------------|
| D4            | Gate of MOSFET (through 100Ω resistor) |
| 3.3V Battery +| Source of MOSFET                      |
| MOSFET Drain  | SD Module VCC                         |
| GND           | GND (Arduino, SD module, Battery -)    |
| D10           | SD Card CS pin                         |
| D11           | SD Card MOSI                           |
| D12           | SD Card MISO                           |
| D13           | SD Card SCK                            |

- Add a 10kΩ resistor between MOSFET Gate and Source.
- 3.3V directly powers Pro Mini VCC pin.

---

## Complete Arduino Code (VersionC.ino)

```cpp
#include <SPI.h>
#include <SdFat.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

SdFat sd;
File dataFile;

const int MOSFET_CONTROL_PIN = 4;
const int SD_CS_PIN = 10;
const int WDT_INTERVALS = 75; // 8s * 75 = 10 minutes

volatile int wdtCounter = 0;
volatile bool shouldWakeUp = false;

void setup() {
  pinMode(MOSFET_CONTROL_PIN, OUTPUT);
  digitalWrite(MOSFET_CONTROL_PIN, HIGH); // Turn off SD initially

  ADCSRA &= ~(1 << ADEN);

  MCUSR &= ~(1 << WDRF);
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = (1 << WDIE) | (1 << WDP3);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
}

ISR(WDT_vect) {
  wdtCounter++;
  if (wdtCounter >= WDT_INTERVALS) {
    shouldWakeUp = true;
    wdtCounter = 0;
  }
}

void loop() {
  if (shouldWakeUp) {
    shouldWakeUp = false;

    digitalWrite(MOSFET_CONTROL_PIN, LOW);
    delay(50);

    if (sd.begin(SD_CS_PIN)) {
      String filename = generateFilename();
      dataFile = sd.open(filename, FILE_WRITE);

      if (dataFile) {
        for (int i = 0; i < 5; i++) {
          dataFile.println("Sample data line " + String(i + 1));
        }
        dataFile.close();
      }
    }

    digitalWrite(MOSFET_CONTROL_PIN, HIGH);
  }

  sleep_cpu();
}

String generateFilename() {
  int fileNumber = 0;
  String filename;

  do {
    filename = "DATA" + String(fileNumber) + ".CSV";
    fileNumber++;
  } while (sd.exists(filename) && fileNumber < 1000);

  return filename;
}
```

---

## Setup Instructions

1. Wire Pro Mini, MOSFET, and SD card as described.
2. Flash the code via an FTDI adapter (use 3.3V logic).
3. Insert a formatted FAT32 microSD card.
4. Power the system with a 3.2V LiFePO4 battery.
5. Observe new files being created every 10 minutes.

---

## Notes
- Use SdFat instead of SD.h to avoid SD card initialization failure after reboot.
- Always ensure the MOSFET cleanly turns off SD card power between events.
- Use a pull-up resistor on the MOSFET gate to prevent accidental turn-on.
