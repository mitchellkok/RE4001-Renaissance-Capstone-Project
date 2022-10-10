#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MS8607.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_Sensor.h>
//#include <.pio/libdeps/pico/Adafruit MS8607/Adafruit_MS8607.h>
//#include <.pio/libdeps/pico/Adafruit Unified Sensor/Adafruit_Sensor.h>

// PINS
#define BUILTIN_LED 25

// VARIABLES
unsigned long previousMillis = 0;
const long blinkInterval = 200;
bool ledState = LOW;

// SENSOR OBJECTS
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

void setup() {
  // PINMODES
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);

  //while(!Serial) // Pause the board for as long as the serial monitor is closed

  //  INITIALIZE MS8607 
  bool setSDA();
  if(!ms8607.begin()){
    Serial.println("Failed to find MS8607 chip!");
    // while(true){
    //   Serial.println("Failed to find MS8607 chip!");
    // };
    delay(1000);
    if(!ms8607.begin()){
      Serial.println("Failed to find MS8607 chip again!");
      while(true){
        Serial.println("Cannot start chip");
        delay(500);
      }
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


void loop() {
  // put your main code here, to run repeatedly:
    blinkLed();
    sensors_event_t temp, pressure, humidity;
    ms8607.getEvent(&pressure, &temp, &humidity);
    Serial.print("Temperature: ");Serial.print(temp.temperature); Serial.println(" degrees C");
    Serial.print("Pressure: ");Serial.print(pressure.pressure); Serial.println(" hPa");
    Serial.print("Humidity: ");Serial.print(humidity.relative_humidity); Serial.println(" %rH");
    Serial.println("");

    delay(500);
}