#include <Arduino.h>

// FOR 3IN1
#include <Wire.h>
#include <Adafruit_MS8607.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_Sensor.h>

union atm_union {
    float fl[3]; // float size is 4 Bytes
    uint8_t buf[12];
};

union gps_union {
    double db[4]; // double size is 8 Bytes
    uint8_t buf[32];
};

Adafruit_MS8607 ms8607; // 3in1 Sensor

void init3in1(){
    if(!ms8607.begin()){
    Serial.println("Failed to find MS8607 chip!");
    // while(true){
    //   Serial.println("Failed to find MS8607 chip!");
    // };
    delay(1000);
    if(!ms8607.begin()){
      Serial.println("Failed to find MS8607 chip again!");
      // while(true){
      //   Serial.println("Cannot start chip");
      //   delay(500);
      // }
    }
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

void dispAtmData(){
    sensors_event_t temp, pressure, humidity;
    ms8607.getEvent(&pressure, &temp, &humidity);
    Serial.print("Temperature: ");Serial.print(temp.temperature); Serial.println(" degrees C");
    Serial.print("Pressure: ");Serial.print(pressure.pressure); Serial.println(" hPa");
    Serial.print("Humidity: ");Serial.print(humidity.relative_humidity); Serial.println(" %rH");
    Serial.println("");
}



// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(9600);
//   gpsSerial.begin(GPSBaud);
//   pinMode(BUILTIN_LED, OUTPUT);
//   init3in1();
//   Serial.println("Start up!");

// }

// void loop() {
//   // put your main code here, to run repeatedly:
//     // digitalWrite(BUILTIN_LED, HIGH);
//     // Serial.println("Bright");
//     // delay(1000);
//     // digitalWrite(BUILTIN_LED, LOW);
//     // Serial.println("Dark");
//     // delay(1000);
//   // This sketch displays information every time a new sentence is correctly encoded.
//   GPS();
//   //blinkLed();

// }