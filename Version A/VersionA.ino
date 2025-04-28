#include <Wire.h>
#include <SPI.h>
#include <SdFat.h>
#include <RTClib.h>

#define MOSFET_GATE_PIN 4
#define SD_CS_PIN 10

SdFat sd;
File dataFile;
RTC_DS3231 rtc;

volatile bool alarmTriggered = false;

void setup() {
  pinMode(MOSFET_GATE_PIN, OUTPUT);
  digitalWrite(MOSFET_GATE_PIN, HIGH); // Keep SD+Arduino OFF initially

  delay(50); // Stabilize power lines

  Wire.begin();
  rtc.begin();

  // Configure RTC Control Registers
  // Disable Square Wave output, Enable Alarm 1 interrupt
  rtc.disable32K();
  rtc.writeSqwPinMode(DS3231_OFF);  // Set SQW to "interrupt only" mode
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
  rtc.disableAlarm(2);

  // Set Alarm 1 to trigger 10 minutes from now
  DateTime now = rtc.now();
  DateTime alarmTime = now + TimeSpan(0, 0, 10, 0); // 10 minutes later
  rtc.setAlarm1(alarmTime, DS3231_A1_Minute); // Match minutes

  rtc.armAlarm(1, true);
  rtc.armAlarm(2, false);

  // Power on SD and Arduino fully
  digitalWrite(MOSFET_GATE_PIN, LOW);
  delay(100); // Allow SD card to stabilize

  if (!sd.begin(SD_CS_PIN)) {
    while (true); // Critical failure
  }

  String filename = generateFilename();
  dataFile = sd.open(filename, FILE_WRITE);

  if (dataFile) {
    for (int i = 0; i < 5; i++) {
      dataFile.println("Sample data row " + String(i + 1));
    }
    dataFile.close();
  }

  // Clear and disable Alarm to prevent stuck ON
  rtc.clearAlarm(1);
  rtc.armAlarm(1, false);

  // Cut power via hardware (RTC will go back to backup battery)
  digitalWrite(MOSFET_GATE_PIN, HIGH);
  delay(100); // Give time to cut off
}

void loop() {
  // Nothing to do — MCU will lose power after setup
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
