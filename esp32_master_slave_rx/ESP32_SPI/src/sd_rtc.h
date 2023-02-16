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

#define R_LED 12
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
void sd_save_data(lora_union *data) {
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

    lora_union tx = *data;
    myFile = SD.open("/test.txt", FILE_APPEND);
    if (myFile) {
      // Writes raw data to SD Card
      // myFile.print("testing 1, 2, 3");

      // Writes corresponding timestamp to SD Card --> Time of writing
      myFile.print(now.year(), DEC);
      myFile.print('/'); myFile.print(now.month(), DEC);
      myFile.print('/'); myFile.print(now.day(), DEC);
      myFile.print(" "); myFile.print(now.hour(), DEC);
      myFile.print(':'); myFile.print(now.minute(), DEC);
      myFile.print(':'); myFile.print(now.second(), DEC);

      // myFile.print(", "); myFile.print(tx.data_struct.datetime);
      myFile.print(", "); myFile.print(tx.data_struct.gravity_so2.fl[0]); // gas conc
      myFile.print(", "); myFile.print(tx.data_struct.gravity_so2.fl[1]); // temp

      myFile.print(", "); myFile.print(tx.data_struct.atm_master.fl[0]);  // temp
      myFile.print(", "); myFile.print(tx.data_struct.atm_master.fl[1]);  // pressure
      myFile.print(", "); myFile.print(tx.data_struct.atm_master.fl[2]);  // humidity

      myFile.print(", "); myFile.print(tx.data_struct.imu.fl[0]);     // temp
      myFile.print(", "); myFile.print(tx.data_struct.imu.fl[1]);     // accx
      myFile.print(", "); myFile.print(tx.data_struct.imu.fl[2]);     // accy
      myFile.print(", "); myFile.print(tx.data_struct.imu.fl[3]);     // accz
      myFile.print(", "); myFile.print(tx.data_struct.imu.fl[4]);     // gyrx
      myFile.print(", "); myFile.print(tx.data_struct.imu.fl[5]);     // gyry
      myFile.print(", "); myFile.print(tx.data_struct.imu.fl[6]);     // gyrz

      myFile.print(", "); myFile.print(tx.data_struct.thermocouple.fl[0]);  // thermocouple
      myFile.print(", "); myFile.print(tx.data_struct.thermocouple.fl[1]);  // ambient
      myFile.print(", "); myFile.print(tx.data_struct.thermocouple.fl[2]);  // adc

      myFile.print(", "); myFile.print(tx.data_struct.ecsense_so2.readings.ecsense_so2);  // gas conc
      myFile.print(", "); myFile.print(tx.data_struct.ecsense_so2.readings.ecsense_temp); // temp
      myFile.print(", "); myFile.print(tx.data_struct.ecsense_so2.readings.ecsense_hum);  // humidity
      myFile.print(','); for (int i=0; i<(sizeof(tx.data_struct.ecsense_so2.readings.full_reading)/sizeof(uint8_t)); i++){
        myFile.print(", "); myFile.print(tx.data_struct.ecsense_so2.readings.full_reading[i]);  // debug info
      }

      myFile.print(", "); myFile.print(tx.data_struct.atm_slave.fl[0]);     // temp
      myFile.print(", "); myFile.print(tx.data_struct.atm_slave.fl[1]);     // pressure
      myFile.print(", "); myFile.print(tx.data_struct.atm_slave.fl[2]);     // humidity

      myFile.print(", "); myFile.print(tx.data_struct.gps_slave.readings.satellites);
      myFile.print(", "); myFile.print(tx.data_struct.gps_slave.readings.hdop);
      myFile.print(", "); myFile.print(tx.data_struct.gps_slave.readings.lat);
      myFile.print(", "); myFile.print(tx.data_struct.gps_slave.readings.lng);
      myFile.print(", "); myFile.print(tx.data_struct.gps_slave.readings.meters);
      myFile.print(", "); myFile.print(tx.data_struct.gps_slave.readings.deg);
      myFile.print(", "); myFile.print(tx.data_struct.gps_slave.readings.mps);
      myFile.print(", "); myFile.print(tx.data_struct.gps_slave.readings.value);

      myFile.print(", "); myFile.print(tx.data_struct.co2); // co2 ppm
      myFile.print(", "); myFile.print(tx.data_struct.s_battery_adc);
      myFile.print(", "); myFile.print(tx.data_struct.s_battery_voltage);
      myFile.print(", "); myFile.print(tx.data_struct.s_battry_percent);
      myFile.print(", "); myFile.print(tx.data_struct.m_battery_adc);
      myFile.print(", "); myFile.print(tx.data_struct.m_battery_voltage);
      myFile.print(", "); myFile.print(tx.data_struct.m_battry_percent);
      myFile.print(", "); myFile.print(tx.data_struct.tx_rssi);
      myFile.println();
      Serial.println("Finished writing..");
    }
    else {
      Serial.println("Could not open file for writing");
    }
    myFile.close();
  }

void sd_mount() {
  digitalWrite(R_LED, HIGH); // Turn the R_LED on
  // Define CS pin in SD.begin function
  if (!SD.begin(RTC_CS, SPI, 1000000)) {  // pin 13 cs
    Serial.println("SD Card mounting failed!");
  } else {
    Serial.println("SD Card mounted.");
  }
  digitalWrite(R_LED, LOW); // Turn the R_LED off
}

void sd_unmount() {
  SD.end();
  Serial.println("SD Card unmounted.");
}

void sd_setup() {
  pinMode(R_LED, OUTPUT); // Declare the R_LED as an output
  Serial.println("Initializing SD card...");
  digitalWrite(R_LED, HIGH); // Turn the R_LED on
  // Define CS pin in SD.begin function

  uint8_t count = 0; 
  while (!SD.begin(RTC_CS, SPI, 1000000)) {  // pin 13 cs
    if (count++ == 5) {
      Serial.println("SD Card initialization failed! Moving on...");
      break;
    }
    Serial.println("SD Card initialization failed!");
    delay(1000);
  }
  if (count < 5) {
    Serial.println("SD Card initialization done.");
  }
  sd_unmount();
  delay(20);
  digitalWrite(R_LED, LOW); // Turn the R_LED off
  Serial.println("SD Card setup done!");
}

void rtc_setup() {
  Serial.println("\nInitializing RTC...");
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
  } else {
    Serial.println("RTC initialized\n");
  }
  rtc.start();
}


  // */


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
  /*
  float drift = 43; // seconds plus or minus over oservation period - set to 0 to cancel previous calibration.
  float period_sec = (7 * 86400);  // total obsevation period in seconds (86400 = seconds in 1 day:  7 days = (7 * 86400) seconds )
  float deviation_ppm = (drift / period_sec * 1000000); //  deviation in parts per million (μs)
  float drift_unit = 4.34; // use with offset mode PCF8523_TwoHours
  */
  // float drift_unit = 4.069; //For corrections every min the drift_unit is 4.069 ppm (use with offset mode PCF8523_OneMinute)
  // int offset = round(deviation_ppm / drift_unit);
  // rtc.calibrate(PCF8523_TwoHours, offset); // Un-comment to perform calibration once drift (seconds) and observation period (seconds) are correct
  // rtc.calibrate(PCF8523_TwoHours, 0); // Un-comment to cancel previous calibrations


// void loop() {
//   // nothing happens after setup
//   save_data();
// //   delay(2000);
// }