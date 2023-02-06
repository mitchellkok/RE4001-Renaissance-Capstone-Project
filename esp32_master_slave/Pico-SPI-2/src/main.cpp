#include <Arduino.h>
#include <SPI.h>

#define LED 12
#define BUF_LEN 14

union buffer {
  uint32_t num_val;
  byte buf[BUF_LEN];
};        // 4-byte buffer
union buffer buf_in, buf_out;
int in_buf[BUF_LEN];

void setup() {
  pinMode(LED, OUTPUT);
  // SPIClassRP2040 pico_SPI = SPIClassRP2040(SPI, 16, 17, 18, 19);
  Serial.println("Hello worldA!");
  SPI.setRX(16);
  SPI.setCS(17);
  SPI.setSCK(18);
  SPI.setTX(19);
  SPI.begin(true);
  Serial.println("Hello worldB!");

  uint32_t number = 32;
  buf_out.num_val = number;
}

void loop() {
  // Serial.println("Hello world!");
  // digitalWrite(LED, HIGH);
  // delay(1000);
  // digitalWrite(LED, LOW);
  // delay(1000);
  // Serial.println("Hello worldC!");
  // SPI.transfer(nullptr, buf_in.c, BUF_LEN);
  digitalWrite(LED, HIGH);
  SPI.beginTransaction(SPISettings(1000 * 1000, MSBFIRST, SPI_MODE0));
  SPI.transfer(nullptr, in_buf, BUF_LEN);
  SPI.endTransaction();

  // Serial.println("SPI slave says: read page from the MOSI line:");
  if (in_buf[0] != 0) {
    for(int j = 0; j < BUF_LEN; j++){
      char c = in_buf[j];
      Serial.println(c);
    }
    Serial.println("\n");
  }
  // Serial.println("\n");
  digitalWrite(LED, LOW);
}