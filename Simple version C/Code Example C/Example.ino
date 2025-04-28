#include <SPI.h>
#include <SdFat.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

SdFat sd;
File dataFile;

const int MOSFET_CONTROL_PIN = 4; // MOSFET Gate control pin
const int SD_CS_PIN = 10;         // SD Card CS pin
const int WDT_INTERVALS = 75;     // 8s * 75 = 600s = 10 minutes

volatile int wdtCounter = 0;
volatile bool shouldWakeUp = false;

void setup() {
  // Setup MOSFET control pin
  pinMode(MOSFET_CONTROL_PIN, OUTPUT);
  digitalWrite(MOSFET_CONTROL_PIN, HIGH); // Turn off SD initially

  // Disable ADC to save power
  ADCSRA &= ~(1 << ADEN);

  // Configure Watchdog Timer for 8 seconds interrupt
  MCUSR &= ~(1 << WDRF);
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = (1 << WDIE) | (1 << WDP3); // 8s interrupt

  // Set sleep mode
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

    // 1. Power ON SD card
    digitalWrite(MOSFET_CONTROL_PIN, LOW); 
    delay(50); // Allow SD card voltage to stabilize

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

    // 2. Prepare SD pins for safe power off
    pinMode(10, INPUT); // CS
    pinMode(11, INPUT); // MOSI
    pinMode(12, INPUT); // MISO
    pinMode(13, INPUT); // SCK

    // 3. Disable SPI hardware
    SPCR &= ~_BV(SPE);

    // 4. Power OFF SD card
    digitalWrite(MOSFET_CONTROL_PIN, HIGH);
  }

  // Sleep deeply
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
