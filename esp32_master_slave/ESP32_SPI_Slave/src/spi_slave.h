#include <ESP32SPISlave.h>
#include <gps_atm.h>

#define BUF_LEN 43

union data_union {
    struct readings {
      atm_union atm;
      gps_union gps;
    } readings;
    char s[BUF_LEN];
    uint8_t buf[BUF_LEN];
};

ESP32SPISlave slave;

void print_rxtx(data_union rx, data_union tx) {
  // Slave does RX then TX
  Serial.println("RX BUFFER: ");
  for(int i=0;i<BUF_LEN;i++){
    Serial.print(rx.buf[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.println("TX BUFFER: ");
  for(int i=0;i<BUF_LEN;i++){
    Serial.print(tx.buf[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
}

void spi_setup() {
    // HSPI = CS: 15, CLK: 14, MOSI: 13, MISO: 12 -> default
    slave.setDataMode(SPI_MODE0);
    bool status = slave.begin();
    // bool status = slave.begin(VSPI, SCK, MISO, MOSI, 14);
}

void buffer_setup(data_union* rx, data_union* tx, bool print) {
  memset((*tx).buf, 0, BUF_LEN); // clear buffers
  memset((*rx).buf, 0, BUF_LEN);
  if (print) {print_rxtx((*rx), (*tx));}
}
