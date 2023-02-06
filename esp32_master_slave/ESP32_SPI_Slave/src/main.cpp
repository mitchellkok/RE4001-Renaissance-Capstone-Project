#include <Arduino.h>

#include <spi_slave.h>
#include <gps_atm.h>
#include <co2.h>
#include <batt.h>

#define LED 2

data_union readings_union[5];   // TODO: implement circular counter, store the last 5 readings
data_union rx_union;
data_union tx_union;

void setup() {
    Serial.begin(9600);
    pinMode(LED, OUTPUT);
    spi_setup();
    buffer_setup(&rx_union, &tx_union, true);  

    Serial.println("Starting GPS...");
    gpsSerial.begin(GPSBaud);

    Serial.println("Starting 3 in 1...");
    init3in1();
    // CO2 SENSOR: NO SETUP NEEDED

    Serial.println("Slave Ready");
}

void loop() {
    slave.wait(rx_union.buf, tx_union.buf, BUF_LEN); // block until the transaction comes from master

    digitalWrite(LED, true);
    while (slave.available()) {
        uint8_t command = rx_union.buf[0];
        Serial.printf("SPI Slave Command Received: 0x%x (%d)", command, command);Serial.println("");
        print_rxtx(rx_union, tx_union);  

        buffer_setup(&rx_union, &tx_union, false);
        if (command == 0xAA) {
            Serial.println("\nReading 3 in 1");
            tx_union.readings.atm = dispAtmData();
            tx_union.readings.gps = GPS();
            tx_union.readings.co2 = co2_reading();
            float batt_voltage = get_battery_voltage();
            float batt_percent = get_battery_percent(batt_voltage);
        } else if (rx_union.buf[0] == 0xBB) {
            // READINGS HAVE ALREADY BEEN SENT IN SLAVE.WAIT
            tx_union.buf[0] = 255; // load in ACK symbol to send on 0xAA
        } else {
            // Unidentified command
            tx_union.buf[0] = 254; // load in NACK symbol
        }
        tx_union.buf[1] = rx_union.buf[1]; // Replace with readings to send on 0xBB

        slave.pop();
        delay(10);
    }
    
    Serial.println("");
    digitalWrite(LED, false);
}
//*/
