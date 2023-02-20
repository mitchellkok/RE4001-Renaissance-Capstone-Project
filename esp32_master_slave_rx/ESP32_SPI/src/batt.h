#include <Arduino.h>
#include <MovingAverageFloat.h>

MovingAverageFloat <8> readings;

#define VBAT_PIN_M  35
#define MAXV_ADC_M  3631
#define GRADIENT_M  0.000638
#define INTERCEPT_M 1.882

#define VBAT_PIN_S  34
#define MAXV_ADC_S  3231
#define GRADIENT_S  0.0005
#define INTERCEPT_S 2.5845

#define PINV_MAX     3.3
#define BATTV_MAX    4.2     // maximum voltage of battery
#define BATTV_MIN    3.3     // what we regard as an empty battery
#define BATTV_LOW    3.4     // voltage considered to be low battery

#define SCALER       0.96
#define LED 4

int get_master_adc_reading() {
    return analogRead(VBAT_PIN_M);
}

int get_slave_adc_reading() {
    return analogRead(VBAT_PIN_S);
}

float get_master_battery_voltage(int adc_reading) {
    float battv_m = (adc_reading * GRADIENT_M) + INTERCEPT_M;
    Serial.printf("adc_reading: %d", adc_reading);Serial.println("");
    Serial.printf("battv_m: %f", battv_m);Serial.println("");
    readings.add(battv_m);
    float output = readings.get();
    return output;
}

float get_slave_battery_voltage(int adc_reading) {
    float battv_s = (adc_reading * GRADIENT_S) + INTERCEPT_S;
    Serial.printf("adc_reading: %d", adc_reading);Serial.println("");
    Serial.printf("battv_s: %f", battv_s);Serial.println("");
    readings.add(battv_s);
    float output = readings.get();
    return output;
}

float get_battery_percent(float battv) {
    float battpc = (((battv - BATTV_MIN) / (BATTV_MAX - BATTV_MIN)) * 100);
    return battpc;
}
