#include <Arduino.h>

#include <spi_slave.h>
#include <gps_atm.h>

#define LED 2

data_union readings_union[5];   // TODO: implement circular counter, store the last 5 readings
data_union rx_union;
data_union tx_union;

void setup() {
    Serial.begin(9600);
    pinMode(LED, OUTPUT);
    spi_setup();
    buffer_setup(&rx_union, &tx_union, true);  

    gpsSerial.begin(GPSBaud);
    init3in1();

    Serial.println("Slave Ready");
}

void loop() {
    slave.wait(rx_union.buf, tx_union.buf, BUF_LEN); // block until the transaction comes from master

    digitalWrite(LED, true);
    while (slave.available()) {
        uint8_t command = rx_union.buf[0];
        Serial.printf("SPI Slave Command Received: %d", command);Serial.println("");
        print_rxtx(rx_union, tx_union);  

        buffer_setup(&rx_union, &tx_union, false);
        if (command == 0xAA) {
            // Trigger readings, prepare readings buffer
            // tx_union.buf[0] = 1;  // symbol to represent readings packet
            // tx_union.buf[2]++;    // counter to track trigger number

            Serial.println("\nReading 3 in 1");
            
            // atm_union atm_readings = dispAtmData();
            // for (int i = 0; i < 12; i++) {
            //     tx_union.buf[i] = atm_readings.buf[i];  // load atm readings into buffer
            // }

            tx_union.readings.atm = dispAtmData();
            tx_union.readings.gps = GPS();
            // Add in readings to send on 0xBB
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
