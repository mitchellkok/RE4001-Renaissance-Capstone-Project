#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>
#include <Arduino.h>

// FOR 3IN1
#include <Wire.h>
#include <Adafruit_MS8607.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_Sensor.h>

#ifndef GPS_ATM
  #define GPS_ATM
  #define RXPin 16
  #define TXPin 17

  union atm_union {
      float fl[3]; // float size is 4 Bytes
      uint8_t buf[12];
  };

  // union gps_union {
  //     struct readings {
  //       uint32_t satellites;
  //       uint32_t hdop;
  //       double lat;
  //       double lng;
  //       double meters;
  //       double deg;
  //       double mps;
  //       double value;
  //     } readings;
  //     uint8_t buf[56];
  // };

  union gps_union {
    struct readings {
      uint32_t satellites;
      uint32_t hdop;
      float lat;
      float lng;
      float meters;
      float deg;
      float mps;
      float value;
    } readings;
    uint8_t buf[32];
  };


  // Global Variables
  int GPSBaud = 9600;
  int timeZone = 8; 
  unsigned long previousMillis = 0;
  const long blinkInterval = 200;
  bool ledState = LOW;


  SoftwareSerial gpsSerial(RXPin, TXPin);
  TinyGPSPlus gps; // gps
  Adafruit_MS8607 ms8607; // 3in1 Sensor


  void blinkLed(){
    unsigned long currentMillis = millis(); // milliseconds from power up
    if(currentMillis-previousMillis>=blinkInterval){
      previousMillis=currentMillis;
      ledState = (ledState == LOW) ? HIGH : LOW;
      digitalWrite(BUILTIN_LED, ledState);
      Serial.println(String(ledState));
    }
  }

  gps_union displayNMEAMessage(){
    if(true){
      Serial.print("Number of Satellites Detected: ");
      Serial.println(gps.satellites.value());
    }
    if (gps.location.isValid()){
      Serial.print("Latitude: ");
      Serial.print(gps.location.lat(), 6);

      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);

      Serial.print("Altitude: ");
      Serial.println(gps.altitude.meters());

      Serial.print("Course: ");
      Serial.println(gps.course.deg());

      Serial.print("Ground Speed: ");
      Serial.println(gps.speed.mps());

      Serial.print("HDOP: ");
      Serial.println(gps.hdop.value());
    }
    else{
      Serial.println("Location: Not Available");
    }
    
    Serial.print("Date: ");
    if (gps.date.isValid()){
      Serial.print(gps.date.month());
      Serial.print("/");
      Serial.print(gps.date.day());
      Serial.print("/");
      Serial.println(gps.date.year());
    }
    else{
      Serial.println("Not Available");
    }
    Serial.print("Time: ");
    if (gps.time.isValid()){
      if (gps.time.hour() < 10) Serial.print(F("0"));
      Serial.print(gps.time.hour());
      Serial.print(":");
      if (gps.time.minute() < 10) Serial.print(F("0"));
      Serial.print(gps.time.minute());
      Serial.print(":");
      if (gps.time.second() < 10) Serial.print(F("0"));
      Serial.print(gps.time.second());
      Serial.print(".");
      if (gps.time.centisecond() < 10) Serial.print(F("0"));
      Serial.println(gps.time.centisecond());
    }
    else{
      Serial.println("Not Available");
    }

    Serial.println();
    Serial.println();

    gps_union gps_readings;
    gps_readings.readings.satellites =  (float) gps.satellites.value();
    gps_readings.readings.lat =         (float) gps.location.lat();
    gps_readings.readings.lng =         (float) gps.location.lng();
    gps_readings.readings.meters =      (float) gps.altitude.meters();
    gps_readings.readings.deg =         (float) gps.course.deg();
    gps_readings.readings.mps =         (float) gps.speed.mps();
    gps_readings.readings.hdop =        (float) gps.hdop.value();
    // gps_readings.readings.satellites =  (float) 10;
    // gps_readings.readings.lat =         (float) 11.56;
    // gps_readings.readings.lng =         (float) 12.35;
    // gps_readings.readings.meters =      (float) 100;
    // gps_readings.readings.deg =         (float) 10;
    // gps_readings.readings.mps =         (float) 5;
    // gps_readings.readings.hdop =        (float) 5;
    return gps_readings;
  }

  void init3in1(){
    if(!ms8607.begin()){
      Serial.println("Failed to find MS8607 chip!");
      delay(1000);
      if(!ms8607.begin()) Serial.println("Failed to find MS8607 chip again!");
    } else {
      Serial.println("MS8607 initialised!");
    }

    ms8607.setHumidityResolution(MS8607_HUMIDITY_RESOLUTION_OSR_8b);
    Serial.print("Humidity resolution set to ");
    switch (ms8607.getHumidityResolution()){
      case MS8607_HUMIDITY_RESOLUTION_OSR_12b: Serial.println("12-bit"); break;
      case MS8607_HUMIDITY_RESOLUTION_OSR_11b: Serial.println("11-bit"); break;
      case MS8607_HUMIDITY_RESOLUTION_OSR_10b: Serial.println("10-bit"); break;
      case MS8607_HUMIDITY_RESOLUTION_OSR_8b: Serial.println("8-bit"); break;
    }
    // ms8607.setPressureResolution(MS8607_PRESSURE_RESOLUTION_OSR_4096);
    Serial.print("Pressure and Temperature resolution set to ");
    switch (ms8607.getPressureResolution()){
      case MS8607_PRESSURE_RESOLUTION_OSR_256: Serial.println("256"); break;
      case MS8607_PRESSURE_RESOLUTION_OSR_512: Serial.println("512"); break;
      case MS8607_PRESSURE_RESOLUTION_OSR_1024: Serial.println("1024"); break;
      case MS8607_PRESSURE_RESOLUTION_OSR_2048: Serial.println("2048"); break;
      case MS8607_PRESSURE_RESOLUTION_OSR_4096: Serial.println("4096"); break;
      case MS8607_PRESSURE_RESOLUTION_OSR_8192: Serial.println("8192"); break;
    }
    Serial.println("");
  }

  atm_union dispAtmData(){
      sensors_event_t temp, pressure, humidity; // 36 byte data struct
      ms8607.getEvent(&pressure, &temp, &humidity);
      Serial.print("Temperature: ");Serial.print(temp.temperature); Serial.println(" degrees C");
      Serial.print("Pressure: ");Serial.print(pressure.pressure); Serial.println(" hPa");
      Serial.print("Humidity: ");Serial.print(humidity.relative_humidity); Serial.println(" %rH");
      Serial.println("");

      atm_union atm_readings;
      atm_readings.fl[0] = temp.temperature;
      atm_readings.fl[1] = pressure.pressure;
      atm_readings.fl[2] = humidity.relative_humidity;
      return atm_readings;
  }

  gps_union GPS(){
    gps_union gps_readings;
    Serial.println("Attempting GPS");
    while (gpsSerial.available() > 0){
      if (gps.encode(gpsSerial.read())){
        gps_readings = displayNMEAMessage();
        break; // TODO: See if need to break here to avoid infinite loop
      }
      // If there are no characters coming in
      // over the software serial port, show a "No GPS detected" error
      if (gps.charsProcessed() < 10){
        Serial.println("No GPS detected");
        break;
      }
    }
    // gps_readings = displayNMEAMessage(); // For testing
    return gps_readings;
  }

#endif