#include <Arduino.h>
#include <MovingAverageFloat.h>

MovingAverageFloat <8> readings;

#define VBAT_PIN    35
#define VBAT_PIN_S  34
#define PINV_MAX     3.3
#define BATTV_MAX    4.2     // maximum voltage of battery
#define BATTV_MIN    3.3     // what we regard as an empty battery
#define BATTV_LOW    3.4     // voltage considered to be low battery
#define ADC_STEPS    4095
#define SCALER       0.96
#define LED 4

// 3.98V actual, 3.39V read
// 4.01V Actual, 3.42V reading

int get_master_adc_reading() {
    return analogRead(VBAT_PIN);
}

int get_slave_adc_reading() {
    return analogRead(VBAT_PIN_S);
}

float get_battery_voltage(int adc_reading) {
    float battv = 0;
    if (adc_reading <= 3000) {
        battv = (0.0008*adc_reading + 0.1372) / PINV_MAX * BATTV_MAX; 
    } else {
        battv = (0.0005*adc_reading + 1.0874) / PINV_MAX * BATTV_MAX; 
    }
    // if (battv <= BATTV_LOW) {
    //     digitalWrite(LED, HIGH); // Turn the LED on for low battery
    // }
    Serial.printf("adc_reading: %d", adc_reading);Serial.println("");
    Serial.printf("battv: %f", battv);Serial.println("");
    readings.add(battv);
    float output = readings.get();
    return output;
}

float get_battery_percent(float battv) {
    float battpc = (((battv - BATTV_MIN) / (BATTV_MAX - BATTV_MIN)) * 100);
    return battpc;
}
