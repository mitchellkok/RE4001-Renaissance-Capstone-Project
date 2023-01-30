#include <Arduino.h>
#include <MovingAverageFloat.h>

MovingAverageFloat <8> readings;

#define VBAT_PIN 35
#define BATTV_MAX    4.1     // maximum voltage of battery
#define BATTV_MIN    3.2     // what we regard as an empty battery
#define BATTV_LOW    3.4     // voltage considered to be low battery
#define ADC_STEPS    4095
#define SCALER       0.96
#define LED 4

float get_battery_voltage() {
    float battv = ((float)analogRead(VBAT_PIN) / ADC_STEPS) * BATTV_MAX * SCALER;
    if (battv <= BATTV_MIN) {
        digitalWrite(LED, HIGH); // Turn the LED on for low battery
    }
    readings.add(battv);
    return readings.get();
}

float get_battery_percent(float battv) {
    float battpc = (((battv - BATTV_MIN) / (BATTV_MAX - BATTV_MIN)) * 100);
    return battpc;
}
