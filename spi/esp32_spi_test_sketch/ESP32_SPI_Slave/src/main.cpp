#include <Arduino.h>
#include <ESP32SPISlave.h>

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

void setup() {
    Serial.begin(9600);
    pinMode(LED, OUTPUT);

    // HSPI = CS: 15, CLK: 14, MOSI: 13, MISO: 12 -> default
    slave.setDataMode(SPI_MODE0);
    slave.begin();

    // clear buffers
    memset(tx_union.buf, 0, BUF_LEN);
    memset(rx_union.buf, 0, BUF_LEN);
    Serial.println("Ready");
}

bool state = true; uint8_t count = 0;
void loop() {
    // TODO: Move this line to after slave.wait; read in from memory buffer
    memset(tx_union.buf, 0, BUF_LEN); tx_union.buf[0] = count;     
    slave.wait(rx_union.buf, tx_union.buf, BUF_LEN); // block until the transaction comes from master

    digitalWrite(LED, state);
    while (slave.available()) {
        // show received data
         Serial.print("Command Received: ");
         Serial.println(count++);

        // TODO: Command order
        // Check command value (rx_union.rx_buf[0])
        // if trigger readings: trigger readings into buffer
        // if send readings: send readings
        for (int i = 0; i < BUF_LEN; i++) {
            Serial.print(rx_union.buf[i]);
            Serial.print("\t");
            Serial.println(rx_union.s[i]);
            rx_union.buf[i] = 0;
        }
        slave.pop();
    }
    
    Serial.println("");
    state = !state;
}