/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>

// Date and time functions using a PCF8523 RTC connected via I2C and Wire lib
#include "RTClib.h"
#include <data_structs.h>

#define LED 12
#define RTC_CS  13

// Initialize RTC
RTC_PCF8523 rtc;

// Initialise SD card myFile
File myFile;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

datetime_union get_datetime() {
  DateTime now = rtc.now();

  datetime_union datetime;
  datetime.datetime.year = now.year();
  datetime.datetime.b[0] = now.month();
  datetime.datetime.b[1] = now.day();
  datetime.datetime.b[2] = now.hour();
  datetime.datetime.b[3] = now.minute();
  datetime.datetime.b[4] = now.second();
  return datetime;
}

// Function to save data into SD card, should pass in parameters of sensor data
void sd_save_data() {
    // Get current Date Time
    DateTime now = rtc.now();
    Serial.print(now.year(), DEC); // uint16_t
    Serial.print('/');
    Serial.print(now.month(), DEC); // uint8_t
    Serial.print('/');
    Serial.print(now.day(), DEC); // uint8_t
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC); // uint8_t
    Serial.print(':');
    Serial.print(now.minute(), DEC); // uint8_t
    Serial.print(':');
    Serial.print(now.second(), DEC); // uint8_t
    Serial.println();

    myFile = SD.open("/test4.txt", FILE_APPEND);
    if (myFile) {
      // Writes raw data to SD Card
      myFile.print("testing 1, 2, 3");

      // Writes corresponding timestamp to SD Card
      myFile.print(now.year(), DEC);
      myFile.print('/');
      myFile.print(now.month(), DEC);
      myFile.print('/');
      myFile.print(now.day(), DEC);
      myFile.print(" (");
      myFile.print(daysOfTheWeek[now.dayOfTheWeek()]);
      myFile.print(") ");
      myFile.print(now.hour(), DEC);
      myFile.print(':');
      myFile.print(now.minute(), DEC);
      myFile.print(':');
      myFile.print(now.second(), DEC);
      myFile.println();
      Serial.println("Finished writing..");
    }
    else {
      Serial.println("Could not open file for writing");
    }
    myFile.close();
  }

void sd_rtc_setup() {
  pinMode(LED, OUTPUT); // Declare the LED as an output
  Serial.print("Initializing SD card...");
  digitalWrite(LED, HIGH); // Turn the LED on
  // Define CS pin in SD.begin function

  uint8_t count = 0;
  while (!SD.begin(RTC_CS)) {  // pin 13 cs
    if (count++ == 10) {
      Serial.println("SD Card initialization failed! Moving on...");
      break;
    }
    Serial.println("SD Card initialization failed!");
    delay(1000);
  }
  Serial.println("SD Card initialization done.");
  digitalWrite(LED, LOW); // Turn the LED on

  Serial.println("Initializing RTC...");
  if (!rtc.begin()) {
      Serial.println("Couldn't find RTC");
      Serial.flush();
      while (1) delay(10);
    }

  rtc.start();
  Serial.println("RTC Started.\n");
   // The PCF8523 can be calibrated for:
  //        - Aging adjustment
  //        - Temperature compensation
  //        - Accuracy tuning
  // The offset mode to use, once every two hours or once every minute.
  // The offset Offset value from -64 to +63. See the Application Note for calculation of offset values.
  // https://www.nxp.com/docs/en/application-note/AN11247.pdf
  // The deviation in parts per million can be calculated over a period of observation. Both the drift (which can be negative)
  // and the observation period must be in seconds. For accuracy the variation should be observed over about 1 week.
  // Note: any previous calibration should cancelled prior to any new observation period.
  // Example - RTC gaining 43 seconds in 1 week
  float drift = 43; // seconds plus or minus over oservation period - set to 0 to cancel previous calibration.
  float period_sec = (7 * 86400);  // total obsevation period in seconds (86400 = seconds in 1 day:  7 days = (7 * 86400) seconds )
  float deviation_ppm = (drift / period_sec * 1000000); //  deviation in parts per million (μs)
  float drift_unit = 4.34; // use with offset mode PCF8523_TwoHours
  // float drift_unit = 4.069; //For corrections every min the drift_unit is 4.069 ppm (use with offset mode PCF8523_OneMinute)
  int offset = round(deviation_ppm / drift_unit);
  // rtc.calibrate(PCF8523_TwoHours, offset); // Un-comment to perform calibration once drift (seconds) and observation period (seconds) are correct
  // rtc.calibrate(PCF8523_TwoHours, 0); // Un-comment to cancel previous calibrations

}

// void loop() {
//   // nothing happens after setup
//   save_data();
// //   delay(2000);
// }