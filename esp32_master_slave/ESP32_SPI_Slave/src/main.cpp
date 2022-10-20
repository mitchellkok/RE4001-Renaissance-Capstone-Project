#include <Arduino.h>
#include <ESP32SPISlave.h>

#include <atm.h>
#include <gravity_so2.h>

#define BUF_LEN 14
#define LED 2

ESP32SPISlave slave;

union data_union {
    char s[BUF_LEN];
    uint8_t buf[BUF_LEN];
};
data_union readings_union[5];   // TODO: implement circular counter, store the last 5 readings
data_union rx_union;
data_union tx_union;

void print_rxtx(data_union rx, data_union tx) {
  // Slave does RX then TX
  Serial.print("RX BUFFER: ");
  for(int i=0;i<BUF_LEN;i++){
    Serial.print(rx.buf[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.print("TX BUFFER: ");
  for(int i=0;i<BUF_LEN;i++){
    Serial.print(tx.buf[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
}

void spi_setup() {
    // HSPI = CS: 15, CLK: 14, MOSI: 13, MISO: 12 -> default
    slave.setDataMode(SPI_MODE0);
    slave.begin();
}

void buffer_setup(data_union rx, data_union tx, bool print) {
  memset(tx.buf, 0, BUF_LEN); // clear buffers
  memset(rx.buf, 0, BUF_LEN);
  if (print) {print_rxtx(rx, tx);}
}

void setup() {
    Serial.begin(9600);
    pinMode(LED, OUTPUT);
    spi_setup();
    buffer_setup(rx_union, tx_union, true);  

    init3in1();
    gravity_so2_setup();

    Serial.println("Ready");
}

void loop() {
    // Serial.println("\nReading Gravity SO2");
    // gravity_so2();

    slave.wait(rx_union.buf, tx_union.buf, BUF_LEN); // block until the transaction comes from master

    digitalWrite(LED, true);
    while (slave.available()) {
        uint8_t command = rx_union.buf[0];
        Serial.printf("SPI Slave Command Received: %d", command);Serial.println("");
        print_rxtx(rx_union, tx_union);  

        buffer_setup(rx_union, tx_union, false);
        if (command == 0xAA) {
            // Trigger readings, prepare readings buffer
            tx_union.buf[0] = 1;  // symbol to represent readings packet
            tx_union.buf[2]++;    // counter to track trigger number

            Serial.println("\nReading 3 in 1");
            dispAtmData();

            Serial.println("\nReading Gravity SO2");
            gravity_so2();
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