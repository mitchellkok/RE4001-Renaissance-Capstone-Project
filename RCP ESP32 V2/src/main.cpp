#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>
#include <Arduino.h>

// Pins
#define RXPin 3
#define TXPin 1

// Global Variables
int GPSBaud = 9600;
int timeZone = 8; 


SoftwareSerial gpsSerial(RXPin, TXPin);
TinyGPSPlus gps;

void displayNMEAMessage(){
  if(true){
    Serial.print("Number of Satellites Detected: ");
    Serial.println(gps.satellites.value());
    
  }
  if (gps.location.isValid()){
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
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
  delay(1000);
}

void GPS(){
  while (gpsSerial.available() > 0){
    if (gps.encode(gpsSerial.read())){
      displayNMEAMessage();
    }
    // If 5000 milliseconds pass and there are no characters coming in
    // over the software serial port, show a "No GPS detected" error
    if (millis() > 5000 && gps.charsProcessed() < 10){
      Serial.println("No GPS detected");
      while(true){};
    }
  }
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.println("Start up!");

}

void loop() {
  // put your main code here, to run repeatedly:
    // digitalWrite(BUILTIN_LED, HIGH);
    // Serial.println("Bright");
    // delay(1000);
    // digitalWrite(BUILTIN_LED, LOW);
    // Serial.println("Dark");
    // delay(1000);
  // This sketch displays information every time a new sentence is correctly encoded.
  GPS();
}