#include <Arduino.h>
#include <MovingAverageFloat.h>

MovingAverageFloat <8> readings;

#define VBAT_PIN 35
#define PINV_MAX     3.3
#define BATTV_MAX    4.2     // maximum voltage of battery
#define BATTV_MIN    3.3     // what we regard as an empty battery
#define BATTV_LOW    3.4     // voltage considered to be low battery
#define ADC_STEPS    4095
#define SCALER       0.96

// 3.91V Actual, 3.81V Reading

// 3.3V Acrutal, 3.99 Reading
/*
For ADC output values from 100 to 3000:
Vin = 0.0008 * byte + 0.1372 (volts)

For ADC output values above 3000:
Vin = 0.0005 * byte + 1.0874 (volts)
*/

float get_battery_voltage() {
    float battv = 0;
    int adc_reading = analogRead(VBAT_PIN);
    if (adc_reading <= 3000) {
        Serial.println("BATT < 3000:");
        battv = (0.0008*adc_reading + 0.1372) / PINV_MAX * BATTV_MAX; 
    } else {
        Serial.println("BATT > 3000:");
        battv = (0.0005*adc_reading + 1.0874) / PINV_MAX * BATTV_MAX; 
    }
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
