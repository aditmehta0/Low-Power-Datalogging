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
